# include <algorithm>
# include <iostream>
# include <iterator>
# include <tuple>
# include <type_traits>

namespace {

struct Circle {
	struct {
		float x, y;
	} center = { .x = 0, .y = 0 };

	// struct { float f; };	// error. at least one of struct name or object name
							// must be present

	float radius = 0;
};

void foo () {
	struct {
		float x = 10, y = 20;
	} point;

	point.x = 100;

	auto [x, y] = point;

	std::cout << x << ", " << y << '\n';

	decltype (point) p2;

	p2.x = point.x;
	p2.y = point.y;

	Circle c = { .center = { .x = p2.x, .y = p2.y }, .radius = 15};

	auto [x1, y1] = c.center;

	std::cout << x1 << ", " << y1 << '\n';
}

void bar () {
	struct { float x, y; } points [] = {
		{-1, -1}, {-1, 1}, {1, -1}, {1, 1}
	};

	for (const auto & p : points) {
		std::cout << p.x << ", " << p.y << '\n';
	}

	std::remove_reference_t <decltype (points [0])> tr {1, 1};

	auto * it = std::find_if (std::begin (points), std::end (points), [&tr] (const auto & other) -> bool {
		return other.x == tr.x && other.y == tr.y;
	});

	if (std::end (points) != it) {
		std::cout << "found tr at index " << std::distance (std::begin (points), it) << '\n';
	}
}

} // end anonymous namespace

int main () {
	foo ();
	bar ();
}
