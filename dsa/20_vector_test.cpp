//   ____                _       _                                       _           _
//  / ___| ___ _ __ ___ (_)_ __ (_)       __ _  ___ _ __   ___ _ __ __ _| |_ ___  __| |
// | |  _ / _ \ '_ ` _ \| | '_ \| |_____ / _` |/ _ \ '_ \ / _ \ '__/ _` | __/ _ \/ _` |
// | |_| |  __/ | | | | | | | | | |_____| (_| |  __/ | | |  __/ | | (_| | ||  __/ (_| |
//  \____|\___|_| |_| |_|_|_| |_|_|      \__, |\___|_| |_|\___|_|  \__,_|\__\___|\__,_|
//                                       |___/

# include <gtest/gtest.h>
# include "20_vector.h"
# include <string>
# include <vector> // for comparison
# include <numeric>

// --- Construction Tests ---

TEST(VectorTest, DefaultConstructor) {
	vector<int> v;
	EXPECT_EQ(v.size(), 0);
	EXPECT_GE(v.capacity(), 1); // Your make_capacity starts at 1
	EXPECT_TRUE(v.empty());
}

TEST(VectorTest, SizeConstructor) {
	vector<int> v(5);
	EXPECT_EQ(v.size(), 5);
	EXPECT_GE(v.capacity(), 8); // make_capacity(5) -> 8 (powers of 2)

	// Check default initialization (int should be 0)
	for(size_t i = 0; i < v.size(); ++i) {
		EXPECT_EQ(v[i], 0);
	}
}

TEST(VectorTest, InitializerListConstructor) {
	vector<int> v = {10, 20, 30, 40, 50};
	EXPECT_EQ(v.size(), 5);
	EXPECT_EQ(v[0], 10);
	EXPECT_EQ(v[4], 50);
}

TEST(VectorTest, IteratorRangeConstructor) {
	std::vector<int> std_vec = {1, 2, 3, 4, 5};
	vector<int> v(std_vec.begin(), std_vec.end());

	EXPECT_EQ(v.size(), 5);
	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[4], 5);
}

// --- Element Access & Modification Tests ---

TEST(VectorTest, PushBackAndAccess) {
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	EXPECT_EQ(v.size(), 3);
	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[1], 2);
	EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, PopBack) {
	vector<int> v = {1, 2, 3};
	v.pop_back();
	EXPECT_EQ(v.size(), 2);
	EXPECT_EQ(v.back(), 2);

	v.pop_back();
	v.pop_back();
	EXPECT_EQ(v.size(), 0);
	EXPECT_TRUE(v.empty());

	// Ensure no crash on popping empty
	v.pop_back();
	EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, BackAccess) {
	vector<int> v = {10, 20};
	EXPECT_EQ(v.back(), 20);

	v.back() = 30; // Check reference modification
	EXPECT_EQ(v[1], 30);
}

// NOTE: This tests the specific (and slightly dangerous) behavior
// defined in your const back() method where it modifies m_size.
TEST(VectorTest, ConstBackSideEffect) {
	const vector<int> v;
	// v is size 0 initially.
	// Your code: if (0 == m_size) m_size = 1;

	const int& val = v.back();

	// Verify the side effect occurred (casting away const to check private member via public size())
	EXPECT_EQ(v.size(), 1);

	// Since data was value initialized with `new T[]()`, int should be 0
	EXPECT_EQ(val, 0);
}

// --- Capacity & Resizing Tests ---

TEST(VectorTest, Reserve) {
	vector<int> v;
	v.reserve(10);

	EXPECT_GE(v.capacity(), 16); // make_capacity(10) -> 16
	EXPECT_EQ(v.size(), 0);

	v.push_back(1);
	EXPECT_EQ(v[0], 1);
}

TEST(VectorTest, ResizeGrow) {
	vector<int> v = {1, 2};
	v.resize(5);

	EXPECT_EQ(v.size(), 5);
	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[1], 2);
	EXPECT_EQ(v[2], 0); // Default initialized new elements
}

TEST(VectorTest, ResizeShrink) {
	vector<int> v = {1, 2, 3, 4, 5};
	v.resize(2);

	EXPECT_EQ(v.size(), 2);
	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[1], 2);
}

TEST(VectorTest, Clear) {
	vector<int> v = {1, 2, 3};
	v.clear();

	EXPECT_EQ(v.size(), 0);
	EXPECT_TRUE(v.empty());
	EXPECT_GE(v.capacity(), 1); // Capacity reset logic checks
}

// --- Rule of Five (Copy/Move) Tests ---

TEST(VectorTest, CopyConstructor) {
	vector<int> v1 = {1, 2, 3};
	vector<int> v2(v1);

	EXPECT_EQ(v2.size(), 3);
	EXPECT_EQ(v2[0], 1);

	// Deep copy check: modifying v2 should not affect v1
	v2[0] = 99;
	EXPECT_EQ(v1[0], 1);
	EXPECT_EQ(v2[0], 99);
}

TEST(VectorTest, CopyAssignment) {
	vector<int> v1 = {1, 2, 3};
	vector<int> v2 = {10, 20};

	v2 = v1;

	EXPECT_EQ(v2.size(), 3);
	EXPECT_EQ(v2[0], 1);

	// Self assignment check
	v2 = v2;
	EXPECT_EQ(v2.size(), 3);
	EXPECT_EQ(v2[0], 1);
}

TEST(VectorTest, MoveConstructor) {
	vector<int> v1 = {1, 2, 3};
	vector<int> v2(std::move(v1));

	EXPECT_EQ(v2.size(), 3);
	EXPECT_EQ(v2[0], 1);

	// Verify v1 is in valid "moved-from" state (empty/null)
	EXPECT_EQ(v1.size(), 0);
	EXPECT_EQ(v1.capacity(), 0);
}

TEST(VectorTest, MoveAssignment) {
	vector<int> v1 = {1, 2, 3};
	vector<int> v2 = {10, 20};

	v2 = std::move(v1);

	EXPECT_EQ(v2.size(), 3);
	EXPECT_EQ(v2[0], 1);

	EXPECT_EQ(v1.size(), 0);
}

// --- Iterator Tests ---

TEST(VectorTest, Iterators) {
	vector<int> v = {1, 2, 3, 4, 5};
	int sum = 0;

	for(auto it = v.begin(); it != v.end(); ++it) {
		sum += *it;
	}
	EXPECT_EQ(sum, 15);

	// Test const iterators
	const vector<int> cv = {1, 2, 3};
	int csum = 0;
	for(auto it = cv.cbegin(); it != cv.cend(); ++it) {
		csum += *it;
	}
	EXPECT_EQ(csum, 6);
}

// --- Complex Type Tests (Non-POD) ---

TEST(VectorTest, ComplexTypeString) {
	vector<std::string> v;
	v.push_back("Hello");
	v.push_back("World");

	EXPECT_EQ(v.size(), 2);
	EXPECT_EQ(v[0], "Hello");
	EXPECT_EQ(v[1], "World");

	v.resize(3); // Should default construct empty string
	EXPECT_EQ(v[2], "");

	// Test Move logic with strings
	vector<std::string> v2 = std::move(v);
	EXPECT_EQ(v2[0], "Hello");
	EXPECT_EQ(v.size(), 0);
}

// --- Specific Logic: Input Iterator vs Forward Iterator ---
// Your class has logic to detect input_iterator vs forward_iterator.
// Let's test specifically the Input Iterator branch.

# include <sstream>

TEST(VectorTest, InputIteratorConstructor) {
	std::stringstream ss("10 20 30");
	std::istream_iterator<int> begin(ss);
	std::istream_iterator<int> end;

	// This triggers the enable_if for input_iterator logic
	vector<int> v(begin, end);

	EXPECT_EQ(v.size(), 3);
	EXPECT_EQ(v[0], 10);
	EXPECT_EQ(v[1], 20);
	EXPECT_EQ(v[2], 30);
}
