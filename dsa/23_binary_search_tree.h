# ifndef BINARY_SEARCH_TREE_23
# define BINARY_SEARCH_TREE_23

# include <concepts>
# include <utility>

# include "23_binary_node.h"

template <typename T, auto Comparator = [] (const T & t1, const T & t2) -> bool { return t1 < t2; }>
requires requires (T t1, T t2) {
	{ Comparator (t1, t2) } -> std::convertible_to <bool>;
}
class binary_search_tree {
public:
	using node = binary_node <T>;

	binary_search_tree ()
		: m_root (nullptr)
		, m_size (0)
	{}

	binary_search_tree (const binary_search_tree & other)
	{
		if (nullptr != other.m_root) {
			m_root = new node (* other.m_root);
			m_size = other.m_size;
		}
		else {
			m_root = nullptr;
			m_size = 0;
		}
	}

	binary_search_tree & operator= (const binary_search_tree & other) {
		if (this != & other) {
			delete m_root;

			if (nullptr != other.m_root) {
				m_root = new node (* other.m_root);
				m_size = other.m_size;
			}
			else {
				m_root = nullptr;
				m_size = 0;
			}
		}

		return * this;
	}

	binary_search_tree (binary_search_tree && other) noexcept
		: m_root (other.m_root)
		, m_size (other.m_size)
	{
		other.m_root = nullptr;
		other.m_size = 0;
	}

	binary_search_tree & operator= (binary_search_tree && other) noexcept {
		if (this != & other) {
			std::swap (m_root, other.m_root);
			std::swap (m_size, other.m_size);
		}

		return * this;
	}

	~binary_search_tree () {
		delete m_root;
		m_size = 0;
	}

private:
	node * m_root;
	std::size_t m_size;
};

# endif // BINARY_SEARCH_TREE_23
