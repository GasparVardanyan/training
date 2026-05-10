# include <cstddef>
# include <cstdint>
# include <iostream>
# include <memory>
# include <type_traits>

class Base {
	public:
		  Base () {}
		  Base (const Base & ) = delete;
		  Base (Base &&) = delete;
		  Base & operator= (const Base &) = delete;
		  Base & operator= (Base &&) = delete;
		  virtual ~Base () {}
};

class Derived : public Base {};

template <typename To, typename From>
std::shared_ptr <To> StaticPointerCast (const std::shared_ptr <From> & ptr) {
	return std::shared_ptr <To> (ptr, static_cast <std::shared_ptr <To>::element_type *> (ptr.get ()));
}

template <typename To, typename From>
std::shared_ptr <To> ConstPointerCast (const std::shared_ptr <From> & ptr) {
	return std::shared_ptr <To> (ptr, const_cast <std::shared_ptr <To>::element_type *> (ptr.get ()));
}

template <typename To, typename From>
std::shared_ptr <To> DynamicPointerCast (const std::shared_ptr <From> & ptr) {
	if (auto r = dynamic_cast <std::shared_ptr <To>::element_type *> (ptr.get ())) {
		return std::shared_ptr <To> (ptr, r);
	}
	else {
		return {};
	}
}

template <typename To, typename From>
std::shared_ptr <To> ReinterpretPointerCast (const std::shared_ptr <From> & ptr) {
	return std::shared_ptr <To> (ptr, reinterpret_cast <std::shared_ptr <To>::element_type *> (ptr.get ()));
}

int main () {
	{
		Derived * d = new Derived;

		auto printUseCounts = [] (const auto & ... ptrs) -> void {
			((std::cout << ptrs.use_count () << ", "), ...) << std::endl;
		};

		std::shared_ptr <const Base> baseConstPtr; {
			std::shared_ptr <Derived> derivedPtr (d, [] (Derived * ptr) -> void {
				std::cout << "DELETING" << std::endl;
				delete ptr;
			});
			printUseCounts (derivedPtr);
			std::shared_ptr <Base> basePtr = StaticPointerCast <Base> (derivedPtr);
				basePtr = std::static_pointer_cast <Base> (derivedPtr);
			printUseCounts (derivedPtr, basePtr);
			baseConstPtr = ConstPointerCast <const Base> (basePtr);
				baseConstPtr = std::const_pointer_cast <const Base> (basePtr);
			printUseCounts (derivedPtr, basePtr, baseConstPtr);
			std::shared_ptr <const Derived> derivedConstPtr = DynamicPointerCast <const Derived> (baseConstPtr);
				derivedConstPtr = std::dynamic_pointer_cast <const Derived> (baseConstPtr);
			printUseCounts (derivedPtr, basePtr, baseConstPtr, derivedConstPtr);
		}

		std::cout << "====================" << std::endl;
	}

	{
		using type = unsigned;
		constexpr std::size_t type_size = sizeof (type);

		std::shared_ptr <type> up (new type (-1 - 10));

		std::shared_ptr <unsigned char []> raw = std::reinterpret_pointer_cast <unsigned char []> (up);
		static_assert (std::is_same_v <std::shared_ptr <unsigned char []>::element_type, unsigned char>);

		for (std::size_t i = 0; i < type_size; i++) {
			std::cout << (unsigned) (raw [i]) << std::endl;
		}
	}
}
