# include <algorithm>
# include <stdexcept>
# include <string>
# include <vector>

enum class Color { Red, Green, Blue };
enum class Size { Small, Medium, Large };

struct Product {
	std::string name;
	Color color;
	Size size;
};




//              _   _   ____  __ ___ __ _     ___ ____
//   __ _ _ __ | |_(_) / ___|| _/ _ \_ | |   |_ _|  _ \
//  / _` | '_ \| __| | \___ \| | | | | | |    | || | | |
// | (_| | | | | |_| |  ___) | | |_| | | |___ | || |_| |
//  \__,_|_| |_|\__|_| |____/| |\___/| |_____|___|____/
//                           |__|   |__|
//

struct ProductFilter {
	typedef std::vector <Product *> Items;

	static Items by_color (const Items & items, const Color color) {
		Items r;

		std::copy_if (items.cbegin (), items.cend (), std::back_inserter (r), [& color] (const Product * product) -> bool {
			if (color == product->color) {
				return true;
			}
			else {
				return false;
			}
		});

		return r;
	}

	static Items by_size (const Items & items, const Size size) {
		Items r;

		std::copy_if (items.cbegin (), items.cend (), std::back_inserter (r), [& size] (const Product * product) -> bool {
			if (size == product->size) {
				return true;
			}
			else {
				return false;
			}
		});

		return r;
	}

	static Items by_color_and_size (const Items & items, const Color color, const Size size) {
		throw std::runtime_error ("ProductFilter::by_color_and_size violates S[O]LID");
	}
};




//  _
// | |__   __ _ ___  ___
// | '_ \ / _` / __|/ _ \
// | |_) | (_| \__ \  __/
// |_.__/ \__,_|___/\___|
//

template <typename T>
struct Specification {
	virtual bool is_satisfied (const T *) const { return false; }
};

template <typename T>
struct Filter;

template <typename T>
struct BasicFilter
{
	typedef std::vector <T *> Items;

	static Items filter (const Items & items, const Specification <T> & specification) {
		Items r;

		std::copy_if (items.cbegin (), items.cend (), std::back_inserter (r), [& specification] (const T * item) -> bool  {
			if (true == specification.is_satisfied (item)) {
				return true;
			}
			else {
				return false;
			}
		});

		return r;
	}
};




//            _                 _
//   _____  _| |_ ___ _ __  ___(_) ___  _ __
//  / _ \ \/ / __/ _ \ '_ \/ __| |/ _ \| '_ \
// |  __/>  <| ||  __/ | | \__ \ | (_) | | | |
//  \___/_/\_\\__\___|_| |_|___/_|\___/|_| |_|
//

template <>
struct Filter <Product> : BasicFilter <Product> {};

struct ColorSpecification : Specification <Product> {
private:
	enum Color _color;

public:
	void color (Color color) { _color = color; }
	enum Color color () const { return _color; }

	explicit ColorSpecification (Color color) {
		this->color (color);
	}

	virtual bool is_satisfied (const Product * product) const override {
		if (product->color == color ()) {
			return true;
		}
		else {
			return false;
		}
	}
};

struct SizeSpecification : Specification <Product> {
private:
	enum Size _size;

public:
	void size (Size size) { _size = size; }
	enum Size size () const { return _size; }

	explicit SizeSpecification (Size size) {
		this->size (size);
	}

	virtual bool is_satisfied (const Product * product) const override {
		if (product->size == size ()) {
			return true;
		}
		else {
			return false;
		}
	}
};

template <typename T>
struct AndSpecification : Specification <T> {
private:
	Specification <T> first;
	Specification <T> second;

public:
	AndSpecification (const Specification <T> & first, const Specification <T> & second)
		: first (first), second (second) {}

	virtual bool is_satisfied (const T * item) const override {
		if (true == first.is_satisfied (item) && true == second.is_satisfied (item)) {
			return true;
		}
		else {
			return false;
		}
	}
};

template <typename T>
struct OrSpecification : Specification <T> {
private:
	Specification <T> first;
	Specification <T> second;

public:
	OrSpecification (const Specification <T> & first, const Specification <T> & second)
		: first (first), second (second) {}

	virtual bool is_satisfied (const T * item) const override {
		if (true == first.is_satisfied (item) || true == second.is_satisfied (item)) {
			return true;
		}
		else {
			return false;
		}
	}
};

template <typename T>
AndSpecification <T> operator&& (const Specification <T> & first, const Specification <T> & second) {
	return AndSpecification <T> (first, second);
}

template <typename T>
OrSpecification <T> operator|| (const Specification <T> & first, const Specification <T> & second) {
	return OrSpecification <T> (first, second);
}



int main () {
	Product prods [10] = {{}};
	std::vector <Product *> products;

	std::transform (
		std::begin (prods),
		std::end (prods),
		std::back_inserter (products),
		[] (Product & p) { return & p; }
	);

	std::vector <Product *> r;

	r = ProductFilter::by_color (products, Color::Red);
	r = ProductFilter::by_size (products, Size::Medium);

	r = Filter <Product>::filter (products, ColorSpecification (Color::Red));
	r = Filter <Product>::filter (products, SizeSpecification (Size::Medium));

	r = Filter <Product>::filter (products, AndSpecification (ColorSpecification (Color::Red), SizeSpecification (Size::Medium)));
	r = Filter <Product>::filter (products, ColorSpecification (Color::Red) && SizeSpecification (Size::Medium));
}
