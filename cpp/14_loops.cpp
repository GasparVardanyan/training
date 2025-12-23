# include <iostream>
# include <vector>

void forLoop1 ();
void range1 ();

int main ()
{
	forLoop1 ();
	range1 ();
}



void forLoop1 ()
{
	int c = 0;

	std::vector<int> intVec (9);

	for (decltype (* std::begin (intVec)) i : intVec) {
		i = ++c;
		std::cout << i << ' ';
	}
	std::cout << std::endl;

	for (decltype (* std::begin (intVec)) i : intVec) {
		std::cout << i << ' ';
	}
	std::cout << std::endl;

	std::vector<bool> v (9);

	// for (bool & b : v) {
	// 	b = ++c % 2;
	// }

	for (bool && b : v) {
		b = ++c % 2;
		std::cout << b << ' ';
	}
	std::cout << std::endl;

	for (bool && b : v) {
		std::cout << b << ' ';
	}
	std::cout << std::endl;

	for (auto && b : v) {
		b = ++c % 2;
		std::cout << b << ' ';
	}
	std::cout << std::endl;

	for (auto && b : v) {
		std::cout << b << ' ';
	}
	std::cout << std::endl;

	for (decltype (* std::begin (v)) b : v) {
		b = ++c % 2;
		std::cout << b << ' ';
	}
	std::cout << std::endl;

	for (decltype (* std::begin (v)) b : v) {
		std::cout << b << ' ';
	}
	std::cout << std::endl;
}



struct Range1 {
	float arr [3] = {1, 2, 3};

	// friend float * begin (Range & rng) { return rng.arr; }
	// friend float * end (Range & rng) { return rng.arr + 3; }

	float * begin () { return arr; }
	float * end () { return arr + 3; }

	const float * cbegin () const { return arr; }
	const float * cend () const { return arr + 3; }

	const float * begin () const { return cbegin (); }
	const float * end () const { return cend (); }
};

struct Range2 {
	float arr [3] = {1, 2, 3};

	friend float * begin (Range2 & rng) { return rng.arr; }
	friend float * end (Range2 & rng) { return rng.arr + 3; }

	friend const float * cbegin (const Range2 & rng) { return rng.arr; }
	friend const float * cend (const Range2 & rng) { return rng.arr + 3; }

	friend const float * begin (const Range2 & rng) { return cbegin (rng); }
	friend const float * end (const Range2 & rng) { return cend (rng); }
};

struct Range3 {
	float arr [3] = {1, 2, 3};
};

float * begin (Range3 & rng) { return rng.arr; }
float * end (Range3 & rng) { return rng.arr + 3; }

const float * cbegin (const Range3 & rng) { return rng.arr; }
const float * cend (const Range3 & rng) { return rng.arr + 3; }

const float * begin (const Range3 & rng) { return cbegin (rng); }
const float * end (const Range3 & rng) { return cend (rng); }

void range1 ()
{
	Range1 r;

	for (auto & f : r) {
		f *= 2;
		std::cout << f << ' ';
	}
	std::cout << std::endl;

	for (const auto & f : r) {
		std::cout << f << ' ';
	}
	std::cout << std::endl;

	Range2 r2;

	for (auto & f : r2) {
		f *= 2;
		std::cout << f << ' ';
	}
	std::cout << std::endl;

	for (const auto & f : r2) {
		std::cout << f << ' ';
	}
	std::cout << std::endl;
}
