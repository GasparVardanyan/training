# include <cmath>
# include <iostream>
# include <ostream>
# include <utility>



class Shape {
public:
	virtual ~Shape () = default;
	virtual double get_surface () const = 0;
	virtual void describe_object () const {
		std::cout << "this is a shape" << std::endl;
	}

	double get_doubled_surface () const {
		return 2 * get_surface ();
	}
};

class Square : public Shape {
	std::pair <double, double> top_left;
	double side_length;
public:
	Square (const std::pair <double, double> & top_left, double side)
		: top_left (top_left)
		, side_length (side)
	{}

	Square (std::pair <double, double> && top_left, double side)
		: top_left (std::move (top_left))
		, side_length (side)
	{}

	virtual double get_surface () const override {
		return side_length * side_length;
	}

	virtual void describe_object () const override {
		std::cout << * this << std::endl;
	}

	friend std::ostream & operator<< (std::ostream & os, const Square & s) {
		return os
			<< "Square [ x = "
			<< s.top_left.first
			<< ", y = "
			<< s.top_left.second
			<< ", side = "
			<< s.side_length
			<< " ]";
	}

	double get_doubled_surface () {
		std::cout << "!!!" << std::endl;
		return this->Shape::get_doubled_surface ();
	}
};

class Circle : public Shape {
	std::pair <double, double> center;
	double radius;

public:
	Circle (const std::pair <double, double> & center, double radius)
		: center (center)
		, radius (radius)
	{}

	Circle (std::pair <double, double> && center, double radius)
		: center (std::move (center))
		, radius (radius)
	{}

	virtual double get_surface () const override {
		return radius * radius * M_PI;
	}

	virtual void describe_object () const override {
		std::cout << * this << std::endl;
	}

	friend std::ostream & operator<< (std::ostream & os, const Circle & c) {
		return os
			<< "Square [ x = "
			<< c.center.first
			<< ", y = "
			<< c.center.second
			<< ", radius = "
			<< c.radius
			<< " ]";
	}

	double get_radius () const {
		return this->radius;
	}
};



namespace polymorphic_classes { void run (); }
namespace safe_downcasting { void run (); }
namespace destructors { void run (); }



int main () {
	polymorphic_classes::run ();
	safe_downcasting::run ();
	destructors::run ();
}



namespace destructors {
class VirtualDestructor {
public:
	virtual ~VirtualDestructor () {
		std::cout << "VirtualDestructor" << std::endl;
	}
};

class VirtualDerived : public VirtualDestructor {
public:
	virtual ~VirtualDerived () {
		std::cout << "VirtualDerived" << std::endl;
	}
};

class ProtectedDestructor {
protected:
	virtual ~ProtectedDestructor () {
		std::cout << "ProtectedDestructor" << std::endl;
	}
};

class ProtectedDerived : public ProtectedDestructor {
public:
	virtual ~ProtectedDerived () {
		std::cout << "ProtectedDerived" << std::endl;
	}
};

void run () {
	{ VirtualDerived d1; }
	VirtualDerived * d2 = new VirtualDerived;
	delete d2;
	VirtualDestructor * d3 = new VirtualDerived;
	delete d3;

	{ ProtectedDerived d4; }
	ProtectedDerived * d5 = new ProtectedDerived;
	delete d5;
	ProtectedDestructor * d6 = new ProtectedDerived;
	// delete d6; // error Calling a protected destructor of class 'ProtectedDestructor'
	delete dynamic_cast <ProtectedDerived *> (d6);
}
}

namespace safe_downcasting {
void run () {
	Square s ({10, 20}, 30);
	Circle c ({1, 2}, 3);

	Shape * sp = & s;

	if (nullptr != dynamic_cast <Circle *> (sp)) {
		std::cout << "radius: " << static_cast <Circle *> (sp)->get_radius () << std::endl;
	}
	sp = & c;
	if (nullptr != dynamic_cast <Circle *> (sp)) {
		std::cout << "radius: " << static_cast <Circle *> (sp)->get_radius () << std::endl;
	}
}
}

namespace polymorphic_classes {
void run () {
	Square s ({10, 20}, 30);
	s.describe_object ();
	std::cout << s << std::endl;
	std::cout << s.get_doubled_surface () << std::endl;

	Shape * sp = & s;
	sp->describe_object ();
	std::cout << sp->get_surface () << std::endl;
	std::cout << sp->get_doubled_surface () << std::endl;

	Circle c ({1, 2}, 3);

	if (nullptr != dynamic_cast <Circle *> (sp)) {
		std::cout << "radius: " << dynamic_cast <Circle *> (sp)->get_radius () << std::endl;
	}
	sp = & c;
	if (nullptr != dynamic_cast <Circle *> (sp)) {
		std::cout << "radius: " << dynamic_cast <Circle *> (sp)->get_radius () << std::endl;
	}
}
}
