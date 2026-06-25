# include <iostream>
# include <type_traits>

// TODO: continue understanding this

namespace {

namespace detail {

template <typename T>
// NOLINTNEXTLINE(llvm-prefer-static-over-anonymous-namespace)
constexpr T & FUN (T & t) noexcept { return t; }

template <typename T>
// NOLINTNEXTLINE(llvm-prefer-static-over-anonymous-namespace)
void FUN (T &&) = delete;

} // end namespace detail

template <typename T>
struct RefWrapper {
	using type = T;

	template <
		typename U,
		typename = decltype (detail::FUN (std::declval <U> ()), std::enable_if_t <false == std::is_same_v <RefWrapper, std::remove_cvref_t <U>>> ()
		)
	>
	// enable_if_t above prevents copy/move constructor hijacking
	// NOLINTNEXTLINE(bugprone-forwarding-reference-overload)
	constexpr RefWrapper (U && u)
		noexcept (noexcept (detail::FUN <T> (std::forward <U> (u))))
		: _ptr (std::addressof (detail::FUN <T> (std::forward <U> (u))))
	{

	}

private:
	T * _ptr;
};

} // end anonymous namespace

int main () {

}
