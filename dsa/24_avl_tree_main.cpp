# include <iostream>
# include <iterator>
# include <sstream>

# include "20_vector.h"
# include "22_stack.h"
# include "24_avl_tree.h"

int main () {
	avl_tree <int> avlt;
	avlt.insert (50);
	avlt.insert (25);
	avlt.insert (75);
	avlt.insert (20);
	avlt.insert (30);
	avlt.insert (100);
	avlt.insert (27);
	decltype (avlt) avlt2 = avlt;
	avlt.insert (28);

	std::cout << avlt << std::endl;
	std::cout << (avlt == avlt2) << std::endl;

	std::cout << "====================" << std::endl;

	std::cout << avlt << std::endl;

	int x = 10;
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (x);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (20);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (5);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (25);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (15);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (5);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (2);
	std::cout << avlt.contains (10)
		<< " " << avlt.contains (20)
		<< " " << avlt.contains (5)
		<< " " << avlt.contains (25)
		<< " " << avlt.contains (15) << std::endl;
	avlt.insert (7);
	avlt.insert (1);
	avlt.insert (3);

	// avlt.remove (5); // 10, 20, 25, 15, 5
	std::stringstream ss;
	ss << "TREE:\n" << avlt << std::endl;
	std::cout << ss.str () << std::endl;
	std::cout << avlt << std::endl;
	for (int i : (vector <int>) avlt) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	if (false == avlt.empty ()) {
		std::cout << avlt.find_min ()->data << " - " << avlt.find_max ()->data << std::endl;
	}

	std::vector <int> v;
	avlt.dump_sorted (std::back_inserter(v));
	for (int i : v) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	std::cout << avlt.at (20)->data << std::endl;

	std::cout << "\n====================\n" << std::endl;

	struct Traverse {
	public:
		enum class Algorithm {
			preorder, inorder, postorder
		};

	private:
		const Algorithm algorithm;
		std::size_t lpo = 0;

		inline void print_tree (const decltype (avlt)::node * n, std::size_t lpo) {
			for (std::size_t i = 1; i < lpo; i++) {
				std::cout << "    ";
			}
			std::cout << n->data << std::endl;
		};

	public:
		Traverse (Algorithm TT) : algorithm (TT) {}

		void operator () (const decltype (avlt)::node * n) {
			lpo++;

			if (Algorithm::preorder == algorithm) {
				print_tree (n, lpo);
			}

			if (nullptr != n->left) {
				this->operator () (n->left);
			}

			if (Algorithm::inorder == algorithm) {
				print_tree (n, lpo);
			}

			if (nullptr != n->right) {
				this->operator () ( n->right);
			}

			if (Algorithm::postorder == algorithm) {
				print_tree (n, lpo);
			}

			lpo--;
		}
	};

	Traverse t (Traverse::Algorithm::postorder);
	t (avlt.root ());
}
