# include <cstddef>

# include <iostream>
# include <memory>
# include <vector>

struct TreeNode : std::enable_shared_from_this <TreeNode> {
	int value;

	explicit TreeNode (int value = 0) : value (value) {
		std::cout << "creating node " << value << std::endl;
	}

	~TreeNode () {
		std::cout << "deleting node " << value << std::endl;
	}

	std::weak_ptr <TreeNode> parent;
	std::vector <std::shared_ptr <TreeNode>> children;

	void addChild (std::shared_ptr <TreeNode> & node) {
		// node->parent = weak_from_this ();
		node->parent = shared_from_this (); // throws when the instance isn't managed
											// by any shared_ptr
		children.push_back (node);
	}
};

int main () {
	std::shared_ptr <TreeNode> root (new TreeNode);

	for (std::size_t i = 0; i < 10; i++) {
		std::shared_ptr <TreeNode> node (new TreeNode (10 + i));
		root->addChild (node);

		if (node->parent.lock () != root || root->children.back () != node) {
			std::cout << "smth is wrong" << std::endl;
		}
	}

	root.reset ();

	struct S : std::enable_shared_from_this <S> {
		explicit S (int n = 0) : value (n) {
			std::cout << "S (" << value << ")" << std::endl;
		}
		~S () {
			std::cout << "~S (" << value << ")" << std::endl;
		}

		int value;
		std::shared_ptr <S> other;
	};

	{
		S n (1);
		std::cout << n.weak_from_this ().expired () << std::endl;
		// n.shared_from_this (); // throws
		S * np = new S (2);
		std::cout << np->weak_from_this ().expired () << std::endl;
		// np->shared_from_this ();  // throws
		delete np;
	}

	{
		std::shared_ptr <S> s1 (new S (10));
		std::shared_ptr <S> s2 (new S (20));
		s1->other = s2; s2->other = s1; // memory leak: no ~S() gets called
	}

	{
		std::shared_ptr <TreeNode> n1 (new TreeNode (100));
		std::shared_ptr <TreeNode> n2 (new TreeNode (101));
		n1->parent = n2; n2->parent = n1; // no memory leak: parent is weak_ptr
	}

	{
		std::shared_ptr <TreeNode> n1 (new TreeNode (1000));
		std::shared_ptr <TreeNode> n2 (new TreeNode (1001));

		// memory leak
		n1->children.push_back (n2); n2->children.push_back (n1);
	}

	{
		std::weak_ptr <S> wk;
		{
			std::shared_ptr <S> sh = std::make_shared <S> (555);
			wk = sh;
		}
		std::cout << "===" << std::endl;
	}

	{
		std::shared_ptr <S> sp;
		{
			std::shared_ptr <S> sh = std::make_shared <S> (333);
			std::weak_ptr <S> wk = sh;
			sp = wk.lock ();
		}
		std::cout << "===" << std::endl;
	}
}
