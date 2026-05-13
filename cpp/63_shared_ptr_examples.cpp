# include <concepts>
# include <cstddef>
# include <iostream>
# include <memory>
# include <new>
# include <type_traits>
# include <utility>
# include <vector>



namespace example_1 { void run (); }
namespace example_2 { void run (); }
namespace example_3 { void run (); }



int main () {
	example_1::run ();
	std::cout << "====================" << std::endl;
	example_2::run ();
	std::cout << "====================" << std::endl;
	example_3::run ();
}



namespace example_1 { // aliasing shared ptr
class Base {
protected:
	Base (const Base &) = default;
	Base & operator= (const Base &) = default;
	Base (Base && o) noexcept = default;
	Base & operator= (Base &&) noexcept = default;

	virtual ~Base () noexcept {
		std::cout << "cleanup for " << x () << std::endl;
	}

	Base (int x) : m_x (x) {};

public:
	int & x () { return m_x; }
	virtual int & y () = 0;
	int xy () { return x () * y (); }

private:
	int m_x;
};

class Derived : public Base {
public:
	Derived (const Derived &) = default;
	Derived & operator= (const Derived &) = default;
	Derived (Derived && o) noexcept = default;
	Derived & operator= (Derived &&) noexcept = default;

	virtual ~Derived () noexcept override = default;

	Derived (int x, int y)
		: Base (x), m_y (y) {}

	virtual int & y () override { return m_y; }

private:
	int m_y;
};

template <std::derived_from<Base> T>
void func (const std::vector <std::shared_ptr <T>> & dv) {
	for (const auto & d : dv) {
		std::cout << d->x () << ", " << d->y () << ", " << d->xy () << std::endl;
	}
};

void run () {
	std::size_t s = 10;

	std::vector <std::shared_ptr <Derived>> v; {
		auto r = std::make_shared <std::vector <Derived>> ();
		r->reserve (s);

		for (std::size_t i = 0; i < s / 2; i++) {
			r->push_back (Derived (i, 10 * i));
		}

		for (std::size_t i = s / 2; i < s; i++) {
			r->emplace_back (i, 100 * i);
		}

		v.reserve (s);

		for (std::size_t i = 0; i < s; i++) {
			v.emplace_back (r, & r->at (i));
		}
	}

	func (v);
}
}



template <typename Derived>
class Base {
public:
	Base (const Base &) = default;
	Base (Base &&) noexcept = default;
	Base &operator= (const Base &) = default;
	Base &operator= (Base &&) noexcept = default;
	~Base() {
		std::cout << "cleanup for " << x() << std::endl;
	}

protected:
	Base (int x) : m_x (x) {};

public:
	int & x () { return m_x; }
	int xy () { return x () * static_cast <Derived *> (this)->y (); }

private:
	int m_x;
};

class Derived : protected Base <Derived> {
	friend class Base <Derived>;
public:
	using Base::x;
	using Base::xy;

	Derived (int x, int y)
		: Base (x), m_y (y) {}

	int & y () { return m_y; }

private:
	int m_y;
};

template <typename T, typename = void>
struct SupportsBaseInterface : std::false_type {};

template <typename T>
struct SupportsBaseInterface <T, std::enable_if_t <
	std::is_convertible_v <decltype (std::declval <T> ().x ()), int> &&
	std::is_convertible_v <decltype (std::declval <T> ().y ()), int> &&
	std::is_convertible_v <decltype (std::declval <T> ().xy ()), int>
>> : std::true_type {};

template <typename T>
inline constexpr bool SupportsBaseInterfaceV = SupportsBaseInterface <T>::value;

template <typename D>
requires SupportsBaseInterfaceV <D>
void func (const std::vector <std::shared_ptr <D>> & dv) {
	for (const auto & d : dv) {
		std::cout << d->x () << ", " << d->y () << ", " << d->xy () << std::endl;
	}
};



namespace example_2 { // aliasing shared ptr + crtp
void run () {
	std::size_t s = 10;

	std::vector <std::shared_ptr <Derived>> v; {
		static_assert (std::is_nothrow_destructible_v <Derived>);

		auto r = std::make_shared <std::vector <Derived>> ();
		r->reserve (s);

		for (std::size_t i = 0; i < s / 2; i++) {
			r->push_back (Derived (i, 10 * i));
		}

		for (std::size_t i = s / 2; i < s; i++) {
			r->emplace_back (i, 100 * i);
		}

		v.reserve (s);

		for (std::size_t i = 0; i < s; i++) {
			v.emplace_back (r, & r->at (i));
		}
	}

	func (v);
}
}

namespace example_3 { // aliasing shared ptr + crtp + manual mem management
void run () {
	std::size_t s = 10;

	std::vector <std::shared_ptr <Derived>> v; {
		static_assert (std::is_nothrow_destructible_v <Derived>);

		Derived * _r = static_cast <Derived *> (::operator new (s * sizeof (Derived)));

		std::size_t i = 0;
		try {
			for (; i < s; i++) {
				new (_r + i) Derived (i, 10 * i);
			}
		}
		catch (...) {
			for (std::size_t j = 0; j < i; j++) {
				_r [j].~Derived ();
			}

			::operator delete (_r);
			throw;
		}

		std::shared_ptr <Derived> r (
			_r,
			[s=s] (Derived * dmem) -> void {
				std::cout << "DELETER" << std::endl;
				for (std::size_t i = 0; i < s; i++) {
					dmem [i].~Derived ();
				}
				::operator delete (dmem);
			}
		);

		v.reserve (s);

		for (std::size_t i = 0; i < s; i++) {
			v.emplace_back (r, r.get () + i);
		}
	}

	func (v);
}
}
