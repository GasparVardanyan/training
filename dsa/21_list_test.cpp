//   ____                _       _                                       _           _
//  / ___| ___ _ __ ___ (_)_ __ (_)       __ _  ___ _ __   ___ _ __ __ _| |_ ___  __| |
// | |  _ / _ \ '_ ` _ \| | '_ \| |_____ / _` |/ _ \ '_ \ / _ \ '__/ _` | __/ _ \/ _` |
// | |_| |  __/ | | | | | | | | | |_____| (_| |  __/ | | |  __/ | | (_| | ||  __/ (_| |
//  \____|\___|_| |_| |_|_|_| |_|_|      \__, |\___|_| |_|\___|_|  \__,_|\__\___|\__,_|
//                                       |___/

# include <gtest/gtest.h>
# include "21_list.h"
# include <string>
# include <vector>
# include <numeric>
# include <memory>
# include <iterator>


// --- Helper for verifying list content ---
template <typename T>
void ExpectListContents(const list<T>& l, const std::vector<T>& expected) {
	EXPECT_EQ(l.size(), expected.size());

	// Check forward iteration
	size_t i = 0;
	for (const auto& item : l) {
		ASSERT_LT(i, expected.size()) << "List size mismatch during iteration";
		EXPECT_EQ(item, expected[i]);
		i++;
	}

	// Check backward iteration (using decrement)
	if (!l.empty()) {
		auto it = l.end();
		i = expected.size();
		while (it != l.begin()) {
			--it;
			--i;
			EXPECT_EQ(*it, expected[i]);
		}
	}
}

// --- Construction Tests ---

TEST(ListTest, DefaultConstructor) {
	list<int> l;
	EXPECT_EQ(l.size(), 0);
	EXPECT_TRUE(l.empty());
	EXPECT_EQ(l.begin(), l.end());
}

TEST(ListTest, SizeConstructor) {
	list<int> l(5);
	EXPECT_EQ(l.size(), 5);
	// Elements should be default initialized (0 for int)
	for (auto x : l) {
		EXPECT_EQ(x, 0);
	}
}

TEST(ListTest, InitializerListConstructor) {
	list<int> l = {10, 20, 30};
	ExpectListContents(l, {10, 20, 30});
}

TEST(ListTest, RangeConstructorStdIterators) {
	std::vector<int> source = {1, 2, 3, 4, 5};
	// This triggers the template<input_it> constructor
	list<int> l(source.begin(), source.end());
	ExpectListContents(l, {1, 2, 3, 4, 5});
}

// --- Element Access & Modification ---

TEST(ListTest, PushFrontPopFront) {
	list<int> l;
	l.push_front(1);
	l.push_front(2);

	EXPECT_EQ(l.front(), 2);
	EXPECT_EQ(l.back(), 1);
	EXPECT_EQ(l.size(), 2);

	l.pop_front();
	EXPECT_EQ(l.front(), 1);
	EXPECT_EQ(l.size(), 1);
}

TEST(ListTest, PushBackPopBack) {
	list<int> l;
	l.push_back(10);
	l.push_back(20);

	EXPECT_EQ(l.front(), 10);
	EXPECT_EQ(l.back(), 20);

	l.pop_back();
	EXPECT_EQ(l.back(), 10);
	EXPECT_EQ(l.size(), 1);
}

TEST(ListTest, InsertMiddle) {
	list<int> l = {1, 3};
	auto it = l.begin();
	++it; // Point to 3

	l.insert(it, 2); // Insert 2 before 3

	ExpectListContents(l, {1, 2, 3});
}

TEST(ListTest, EraseMiddle) {
	list<int> l = {1, 2, 3, 4};
	auto it = l.begin();
	++it; // Point to 2

	auto ret = l.erase(it); // Erase 2

	EXPECT_EQ(*ret, 3); // Should return iterator to next element
	ExpectListContents(l, {1, 3, 4});
}

TEST(ListTest, Clear) {
	list<int> l = {1, 2, 3};
	l.clear();
	EXPECT_EQ(l.size(), 0);
	EXPECT_TRUE(l.empty());
	EXPECT_EQ(l.begin(), l.end());

	// Ensure we can push after clear
	l.push_back(10);
	EXPECT_EQ(l.front(), 10);
}

// --- Iterator Tests ---

TEST(ListTest, IteratorIncrementDecrement) {
	list<int> l = {1, 2, 3};
	auto it = l.begin();
	EXPECT_EQ(*it, 1);
	EXPECT_EQ(*(++it), 2);
	EXPECT_EQ(*(it++), 2);
	EXPECT_EQ(*it, 3);

	EXPECT_EQ(*(--it), 2);
	EXPECT_EQ(*(it--), 2);
	EXPECT_EQ(*it, 1);
}

TEST(ListTest, ConstIterator) {
	const list<int> l = {1, 2};
	auto it = l.cbegin();
	EXPECT_EQ(*it, 1);
	// *it = 5; // Should fail to compile
}

// --- Rule of Five (Deep Copy & Move) ---

TEST(ListTest, CopyConstructor) {
	// Note: This tests the logic inside list(const list&)
	list<int> l1 = {1, 2, 3};
	list<int> l2(l1);

	ExpectListContents(l2, {1, 2, 3});

	// Verify Deep Copy
	l2.front() = 99;
	EXPECT_EQ(l1.front(), 1);
	EXPECT_EQ(l2.front(), 99);
}

TEST(ListTest, CopyAssignment) {
	list<int> l1 = {1, 2, 3};
	list<int> l2 = {10, 20};

	l2 = l1;
	ExpectListContents(l2, {1, 2, 3});

	// Self assignment check
	l2 = l2;
	ExpectListContents(l2, {1, 2, 3});
}

TEST(ListTest, MoveConstructor) {
	list<std::string> l1;
	l1.push_back("Hello");
	l1.push_back("World");

	list<std::string> l2(std::move(l1));

	EXPECT_EQ(l2.size(), 2);
	EXPECT_EQ(l2.front(), "Hello");
	EXPECT_EQ(l2.back(), "World");

	// l1 should be empty (size 0)
	EXPECT_EQ(l1.size(), 0);
}

TEST(ListTest, MoveAssignment) {
	list<int> l1 = {1, 2, 3};
	list<int> l2 = {10, 20};

	l2 = std::move(l1);

	EXPECT_EQ(l2.size(), 3);
	EXPECT_EQ(l2.front(), 1);
	EXPECT_EQ(l2.back(), 3);

	// l1 should have the old contents of l2 (due to swap implementation)
	// or be empty, depending on specific move logic.
	// Your implementation uses swap, so l1 gets {10, 20}.
	EXPECT_EQ(l1.size(), 2);
	EXPECT_EQ(l1.front(), 10);
}

// --- Complex Types ---

struct ComplexObj {
	int id;
	std::string data;
	bool operator==(const ComplexObj& other) const {
		return id == other.id && data == other.data;
	}
};

TEST(ListTest, ComplexObjects) {
	list<ComplexObj> l;
	l.push_back({1, "A"});
	l.push_back({2, "B"});

	EXPECT_EQ(l.size(), 2);
	EXPECT_EQ(l.front().data, "A");
}


// ==========================================
//           TEST HELPERS
// ==========================================

// Helper to verify forward AND backward linking
// This ensures that for every node, node->next->prev == node
template <typename T>
void SanityCheckLinks(const list<T>& l) {
	if (l.empty()) return;

	size_t count = 0;
	auto it = l.begin();

	// Check Forward and Internal Consistency
	while (it != l.end()) {
		auto current = it;
		auto next = ++it;

		// If we are not at the end, the next node's previous should be current
		if (next != l.end()) {
			// We can't access pointers directly, so we check values via decrement
			auto check_back = next;
			check_back--;
			ASSERT_EQ(*check_back, *current) << "Broken Link: next->prev != current";
		}
		count++;
	}
	ASSERT_EQ(count, l.size()) << "Iterator count mismatch with size()";
}

// Helper to compare against std::vector
template <typename T>
void ExpectListEq(const list<T>& l, const std::vector<T>& expected) {
	ASSERT_EQ(l.size(), expected.size());
	SanityCheckLinks(l); // Run structural check every time

	// Forward check
	size_t i = 0;
	for (const auto& item : l) {
		ASSERT_EQ(item, expected[i]) << "Mismatch at index " << i;
		i++;
	}

	// Backward check (Bidirectional iterator test)
	if (!l.empty()) {
		auto l_it = l.end();
		auto v_it = expected.end();
		while (l_it != l.begin()) {
			--l_it;
			--v_it;
			ASSERT_EQ(*l_it, *v_it) << "Reverse iteration mismatch";
		}
	}
}

// ==========================================
//           CONSTRUCTION TESTS
// ==========================================

TEST(ListDeepTest, DefaultConstruction) {
	list<int> l;
	EXPECT_EQ(l.size(), 0);
	EXPECT_TRUE(l.empty());
	EXPECT_EQ(l.begin(), l.end());
	EXPECT_EQ(l.cbegin(), l.cend());
}

TEST(ListDeepTest, SizeConstruction) {
	list<int> l(10);
	EXPECT_EQ(l.size(), 10);
	for (auto i : l) {
		EXPECT_EQ(i, 0);
	}
	SanityCheckLinks(l);
}

TEST(ListDeepTest, InitializerList) {
	list<int> l = {10, 20, 30, 40};
	ExpectListEq(l, {10, 20, 30, 40});
}

TEST(ListDeepTest, RangeConstruction) {
	std::vector<int> vec = {1, 2, 3, 4, 5};

	// Test Input Iterator Tag logic (if implemented)
	list<int> l1(vec.begin(), vec.end());
	ExpectListEq(l1, vec);

	// Test with empty range
	list<int> l2(vec.begin(), vec.begin());
	EXPECT_TRUE(l2.empty());
}

// ==========================================
//        MODIFICATION & RETURN VALUES
// ==========================================

TEST(ListDeepTest, PushPopFrontBack) {
	list<int> l;

	l.push_back(10);  // {10}
	l.push_front(5);  // {5, 10}
	l.push_back(15);  // {5, 10, 15}
	l.push_front(0);  // {0, 5, 10, 15}

	ExpectListEq(l, {0, 5, 10, 15});

	l.pop_back();     // {0, 5, 10}
	EXPECT_EQ(l.back(), 10);

	l.pop_front();    // {5, 10}
	EXPECT_EQ(l.front(), 5);

	ExpectListEq(l, {5, 10});
}

TEST(ListDeepTest, InsertExactPositionAndReturn) {
	list<int> l = {10, 30};
	auto it = l.begin();
	++it; // Point to 30

	// Insert 20 before 30
	auto ret = l.insert(it, 20);

	// Check return value points to new element
	EXPECT_EQ(*ret, 20);

	// Verify structure
	ExpectListEq(l, {10, 20, 30});

	// Insert at beginning via iterator
	ret = l.insert(l.begin(), 5);
	EXPECT_EQ(*ret, 5);
	EXPECT_EQ(l.front(), 5);

	// Insert at end via iterator
	ret = l.insert(l.end(), 40);
	EXPECT_EQ(*ret, 40);
	EXPECT_EQ(l.back(), 40);

	ExpectListEq(l, {5, 10, 20, 30, 40});
}

TEST(ListDeepTest, EraseSingleAndReturn) {
	list<int> l = {1, 2, 3, 4, 5};

	// Erase middle (3)
	auto it = l.begin();
	std::advance(it, 2);
	auto ret = l.erase(it);

	EXPECT_EQ(*ret, 4); // Should return iterator to element AFTER erased one
	ExpectListEq(l, {1, 2, 4, 5});

	// Erase head
	ret = l.erase(l.begin());
	EXPECT_EQ(*ret, 2);
	ExpectListEq(l, {2, 4, 5});

	// Erase tail
	// Note: l.end() is invalid to erase, we must erase prev(end)
	it = l.end();
	--it;
	ret = l.erase(it);
	EXPECT_EQ(ret, l.end()); // Should return end()
	ExpectListEq(l, {2, 4});
}

TEST(ListDeepTest, EraseRange) {
	// Case 1: Erase middle segment
	list<int> l1 = {1, 2, 3, 4, 5, 6};
	auto start = l1.begin(); ++start;      // 2
	auto end = l1.end(); --end; --end;     // 5

	// Erase [2, 5) -> Erases 2, 3, 4
	auto ret = l1.erase(start, end);

	EXPECT_EQ(*ret, 5);
	ExpectListEq(l1, {1, 5, 6});

	// Case 2: Erase All
	list<int> l2 = {1, 2, 3};
	ret = l2.erase(l2.begin(), l2.end());
	EXPECT_EQ(ret, l2.end());
	EXPECT_TRUE(l2.empty());

	// Case 3: Erase Nothing (Equal iterators)
	list<int> l3 = {1, 2};
	ret = l3.erase(l3.begin(), l3.begin());
	EXPECT_EQ(ret, l3.begin());
	ExpectListEq(l3, {1, 2});
}

TEST(ListDeepTest, ClearResetsCapacityAndState) {
	list<int> l = {1, 2, 3};
	l.clear();

	EXPECT_TRUE(l.empty());
	EXPECT_EQ(l.begin(), l.end());

	// Ensure we can reuse the list after clear
	l.push_back(100);
	ExpectListEq(l, {100});
}

// ==========================================
//      RULE OF 5 & MEMORY MANAGEMENT
// ==========================================

TEST(ListDeepTest, CopyConstructorDeepCopy) {
	list<int> original = {1, 2, 3};
	list<int> copy(original);

	ExpectListEq(copy, {1, 2, 3});

	// Modify copy, ensure original is untouched
	*copy.begin() = 999;
	EXPECT_EQ(*original.begin(), 1);
	EXPECT_EQ(*copy.begin(), 999);
}

TEST(ListDeepTest, CopyAssignmentSelfAssignment) {
	list<int> l = {1, 2, 3};
	list<int> backup = l;

	l = l; // Self assignment

	ExpectListEq(l, {1, 2, 3});

	// Normal assignment
	list<int> other = {10, 20};
	l = other;
	ExpectListEq(l, {10, 20});
}

TEST(ListDeepTest, MoveConstructor) {
	list<std::string> source;
	source.push_back("Hello");
	source.push_back("World");

	list<std::string> dest(std::move(source));

	// Dest check
	EXPECT_EQ(dest.size(), 2);
	EXPECT_EQ(dest.front(), "Hello");
	EXPECT_EQ(dest.back(), "World");
	SanityCheckLinks(dest);

	// Source check (Valid but unspecified state, usually empty)
	EXPECT_EQ(source.size(), 0);
	// Even if empty, iterators should be consistent
	EXPECT_EQ(source.begin(), source.end());
}

TEST(ListDeepTest, MoveAssignment) {
	list<int> source = {1, 2, 3};
	list<int> dest = {10, 20};

	dest = std::move(source);

	ExpectListEq(dest, {1, 2, 3});

	// Depending on implementation (swap vs steal), source is either empty or has {10, 20}
	// Your implementation performs a swap.
	EXPECT_EQ(source.size(), 2);
	EXPECT_EQ(source.front(), 10);
	SanityCheckLinks(source);
}

// ==========================================
//      ADVANCED: MOVE-ONLY TYPES
// ==========================================
// This ensures your list uses std::move internally and doesn't try to copy
// types that are movable but not copyable (like std::unique_ptr).

TEST(ListDeepTest, SupportsMoveOnlyTypes) {
	list<std::unique_ptr<int>> l;

	// Test push_back(T&&)
	l.push_back(std::make_unique<int>(10));
	l.push_back(std::make_unique<int>(20));

	EXPECT_EQ(*l.front(), 10);
	EXPECT_EQ(*l.back(), 20);

	// Test emplace/insert behavior with move
	auto it = l.begin();
	l.insert(it, std::make_unique<int>(5)); // Insert 5 at front

	EXPECT_EQ(*l.front(), 5);
	EXPECT_EQ(l.size(), 3);

	// Test Move Constructor of the list itself with move-only types
	list<std::unique_ptr<int>> l2(std::move(l));
	EXPECT_EQ(*l2.front(), 5);
	EXPECT_EQ(l.size(), 0); // Source should be empty
}

// ==========================================
//      ITERATOR TRAITS & ALGORITHMS
// ==========================================

TEST(ListDeepTest, IteratorTraits) {
	using Iter = list<int>::iterator;

	// Verify it claims to be a Bidirectional Iterator
	bool is_bidirectional = std::is_same<
		std::iterator_traits<Iter>::iterator_category,
		std::bidirectional_iterator_tag
	>::value;

	EXPECT_TRUE(is_bidirectional);
}

TEST(ListDeepTest, StdAlgorithmCompatibility) {
	list<int> l = {1, 2, 3, 4, 5};

	// Test std::find
	auto it = std::find(l.begin(), l.end(), 3);
	ASSERT_NE(it, l.end());
	EXPECT_EQ(*it, 3);

	// Test std::reverse (requires bidirectional iterators)
	std::reverse(l.begin(), l.end());
	ExpectListEq(l, {5, 4, 3, 2, 1});

	// Test std::distance
	EXPECT_EQ(std::distance(l.begin(), l.end()), 5);
}

// ==========================================
//        STRESS / COMPLEXITY CHECK
// ==========================================

TEST(ListDeepTest, LargeScaleOperations) {
	list<int> l;
	const int N = 1000;

	// Push Back N
	for(int i=0; i<N; ++i) l.push_back(i);
	EXPECT_EQ(l.size(), N);
	EXPECT_EQ(l.front(), 0);
	EXPECT_EQ(l.back(), N-1);

	// Pop Front N/2
	for(int i=0; i<N/2; ++i) l.pop_front();
	EXPECT_EQ(l.size(), N/2);
	EXPECT_EQ(l.front(), N/2);

	// Clear
	l.clear();
	EXPECT_TRUE(l.empty());
}
