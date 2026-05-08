# include <iostream>
# include <string>
# include <utility>

namespace this_pointer {
	struct ThisPointer {
		int i;

		ThisPointer (int);

		virtual void func ();

		int get_i () const;
		void set_i (int);
	};

	ThisPointer::ThisPointer (int i) : i (i) {
		func ();
	}

	bool some_external_condition = true;

	void ThisPointer::func () {
		std::cout << "Parent" << std::endl;
	}

	int ThisPointer::get_i () const { return i; }

	void ThisPointer::set_i (int i) { this->i = i; }

	struct ThisPointerDerived final : ThisPointer {
		ThisPointerDerived (int i) : ThisPointer (i) {
			func ();
		}
		virtual void func () override;
	};

	void ThisPointerDerived::func () {
		std::cout << "Derived" << std::endl;
	}

	void run () {
		ThisPointerDerived d (10);
	}
}

namespace this_cv_qualifiers {
	struct CVAccess {
		void no_qualifier () {}
		void c_qualifier () const {}
		void v_qualifier () volatile {}
		void cv_qualifier () const volatile {}
	};

	struct CVOverload {
		int func () { return 3; }
		int func () const { return 33; }
		int func () volatile { return 333; }
		int func () const volatile { return 3333; }
	};

	void run () {
		CVAccess cva;
		cva.no_qualifier ();
		cva.c_qualifier ();
		cva.v_qualifier ();
		cva.cv_qualifier ();

		const CVAccess c_cva;
		// c_cva.no_qualifier (); // error
		c_cva.c_qualifier ();
		// c_cva.v_qualifier (); // error
		c_cva.cv_qualifier ();

		volatile CVAccess v_cva;
		// v_cva.no_qualifier (); // error
		// v_cva.c_qualifier (); // error
		v_cva.v_qualifier ();
		v_cva.cv_qualifier ();

		const volatile CVAccess cv_cva;
		// cv_cva.no_qualifier (); // error
		// cv_cva.c_qualifier (); // error
		// cv_cva.v_qualifier (); // error
		cv_cva.cv_qualifier ();

		CVOverload o;
		std::cout << o.func () << std::endl;
		const CVOverload c_o;
		std::cout << c_o.func () << std::endl;
		volatile CVOverload v_o;
		std::cout << v_o.func () << std::endl;
		const volatile CVOverload cv_o;
		std::cout << cv_o.func () << std::endl;
	}
}

namespace this_ref_qualifiers {
	struct RefQualifiers {
		std::string s;
		RefQualifiers (const std::string & s = "The nameless one.") : s (s) {}

		void func () & {
			std::cout << "Accessed on normal instance: " << s << std::endl;
		}

		void func () && {
			std::cout << "Accessed on temporary instance: " << s << std::endl;
		}

		const std::string & still_a_pointer () & { return this->s; }
		std::string && still_a_pointer () && { this->s = "Bob"; return std::move (s);}
	};

	struct CVRef {
		void func () & {}
		void func () const & {}
		void func () volatile & {}
		void func () const volatile & {}
		void func () && {}
		void func () const && {}
		void func () volatile && {}
		void func () const volatile && {}
	};

	void run () {
		RefQualifiers rf ("Fred");
		rf.func ();

		RefQualifiers ().func ();
	}
}

int main () {
	this_pointer::run ();
	std::cout << "====================" << std::endl;
	this_cv_qualifiers::run ();
	std::cout << "====================" << std::endl;
	this_ref_qualifiers::run ();
}
