# include <iostream>




//              _   _   ____   ___  __ _    __ ___ ____
//   __ _ _ __ | |_(_) / ___| / _ \| _| |  |_ |_ _|  _ \
//  / _` | '_ \| __| | \___ \| | | | || |   | || || | | |
// | (_| | | | | |_| |  ___) | |_| | || |___| || || |_| |
//  \__,_|_| |_|\__|_| |____/ \___/| ||_____| |___|____/
//                                 |__|    |__|
//

class Rectangle {
private:
	int m_width;
	int m_height;

public:
	Rectangle (int width, int height)
		: m_width (width), m_height (height)
	{}

	virtual void setWidth (int width) { m_width = width; }
	int getWidth () const { return m_width; }

	virtual void setHeight (int height) { m_height = height; }
	int getHeight () const { return m_height; }

	int area () const { return m_width * m_height; }
};

class Square : public Rectangle {
public:
	explicit Square (int size)
		: Rectangle (size, size)
	{}

	virtual void setWidth (int width) override {
		this->Rectangle::setWidth (width);
		this->Rectangle::setHeight (width);
	}

	virtual void setHeight (int height) override {
		this->Rectangle::setWidth (height);
		this->Rectangle::setHeight (height);
	}
};

void process (Rectangle & r) {
	int w = r.getWidth ();
	r.setHeight (20);

	std::cout << "expected area: " << 20 * w << std::endl;
	std::cout << "area: " << r.area () << std::endl;
}




//  ____   ___  __ _    __ ___ ____
// / ___| / _ \| _| |  |_ |_ _|  _ \
// \___ \| | | | || |   | || || | | |
//  ___) | |_| | || |___| || || |_| |
// |____/ \___/| ||_____| |___|____/
//             |__|    |__|
//

class Rectangle1 : public Rectangle {
public:
	Rectangle1 (int width, int height) : Rectangle (width, height) {}
	bool isSquare () const { return getWidth () == getHeight (); }
};

struct RectangleFactory {
	static Rectangle1 createRectangle (int w, int h) { return Rectangle1 (w, h); }
	static Rectangle1 createSquare (int s) { return Rectangle1 (s, s); }
};

int main () {
	Rectangle r {5, 5};
	Square s {5};

	process (r);
	process (s);

	std::cout << "====================" << std::endl;

	Rectangle1 r1 = RectangleFactory::createRectangle (5, 5);
	Rectangle1 s1 = RectangleFactory::createSquare (5);

	process (r1);
	process (s1);
}
