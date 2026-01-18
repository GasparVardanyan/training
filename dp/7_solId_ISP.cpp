# include <iostream>
# include <vector>


//              _   _   ____   ___  _     __ ___ __ ____
//   __ _ _ __ | |_(_) / ___| / _ \| |   | _|_ _|_ |  _ \
//  / _` | '_ \| __| | \___ \| | | | |   | | | | | | | | |
// | (_| | | | | |_| |  ___) | |_| | |___| | | | | | |_| |
//  \__,_|_| |_|\__|_| |____/ \___/|_____| ||___|| |____/
//                                       |__|   |__|
//

struct Document {};

struct IMachine {
	virtual void print (const std::vector <Document *> &) =0;
	virtual void scan (const std::vector <Document *> &) =0;
	virtual void fax (const std::vector <Document *> &) =0;
};

struct MyMachine : IMachine {
	virtual void print (const std::vector <Document *> &) override {}
	virtual void scan (const std::vector <Document *> &) override {}
	virtual void fax (const std::vector <Document *> &) override {}
};




//  ____   ___  _     __ ___ __ ____
// / ___| / _ \| |   | _|_ _|_ |  _ \
// \___ \| | | | |   | | | | | | | | |
//  ___) | |_| | |___| | | | | | |_| |
// |____/ \___/|_____| ||___|| |____/
//                   |__|   |__|
//

struct IPrinter {
	virtual void print (const std::vector <Document *> &) = 0;
};

struct IScanner {
	virtual void scan (const std::vector <Document *> &) = 0;
};

struct IFaxMachine {
	virtual void fax (const std::vector <Document *> &) = 0;
};

struct IMachine1 : IPrinter, IScanner, IFaxMachine {};

struct MyPrinter : IPrinter {
	virtual void print (const std::vector <Document *> &) override {}
};

struct MyScanner : IScanner {
	virtual void scan (const std::vector <Document *> &) override {}
};

struct MyMachine1 : IMachine1 {
private:
	IScanner & scanner;
	IPrinter & printer;

public:
	MyMachine1 (IScanner & scanner, IPrinter & printer)
		: scanner (scanner), printer (printer)
	{}

	virtual void print (const std::vector <Document *> & docs) override {
		printer.print (docs);
	}
	virtual void scan (const std::vector <Document *> & docs) override {
		scanner.scan (docs);
	}
};


int main () {

}
