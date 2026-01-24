# include <iostream>
# include <string>

# include "22_balance_checker.h"
# include "22_notation_converter.h"

int main () {
	BalanceChecker c;
	c.setString ("abc<def>[");
	std::cout << c << std::endl;
	c.setString ("abc<>{}[]()");
	std::cout << c << std::endl;
	c.setString ("abc<{[()]}>");
	std::cout << c << std::endl;
	c.setString ("abc<{[(]}>");
	std::cout << c << std::endl;
	c.setString ("abc'<{[(");
	std::cout << c << std::endl;
	c.setString ("abc'<{[('");
	std::cout << c << std::endl;

	std::string res = "(6 * (5 + 3 + 2)) * 10 / 2 - 42";
	std::cout << res << std::endl;

	res = NotationConverter::InfixToPostfix_ShuntingYard (res);
	std::cout << res << std::endl;

	res.resize (res.find ('='));
	res = NotationConverter::PostfixToInfix (res);
	std::cout << res << std::endl;

	res.resize (res.find ('='));
	res = NotationConverter::InfixToPostfix (res);
	std::cout << res << std::endl;

	res.resize (res.find ('='));
	res = NotationConverter::PostfixToInfix (res);
	std::cout << res << std::endl;
}
