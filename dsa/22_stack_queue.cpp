# include <algorithm>
# include <iostream>
# include <list>
# include <ostream>
# include <string>
# include <vector>

# include "22_stack.h"
# include "22_queue.h"

template <typename T>
// using MyVector = std::vector <T>;
using MyVector = vector <T>;

template <typename T>
// using MyList = std::list <T>;
using MyList = list <T>;

template <typename T>
using MyStack = stack <T, MyVector>;

template <typename T>
using MyQueue = queue <T, MyList>;

class BalanceChecker {
public:
	enum class SymbolType {
		Regular = 0,
		DoubleQuote,
		SingleQuote,
		Parenthesis,
		AngleBracket,
		Bracket,
		BackTick,
		Brace,
		SymbolTypeCount
	};

	struct PairInfo {
		std::size_t opener_pos;
		std::size_t closer_pos;
	};

private:
	enum class SymbolRole : int {
		Regular = 0, Opener, Closer, OpenerCloser
	};

	static const inline SymbolType s_symbolTypes [256] = {
		['"'] = SymbolType::DoubleQuote,
		['\''] = SymbolType::SingleQuote,
		['('] = SymbolType::Parenthesis,
		[')'] = SymbolType::Parenthesis,
		['<'] = SymbolType::AngleBracket,
		['>'] = SymbolType::AngleBracket,
		['['] = SymbolType::Bracket,
		[']'] = SymbolType::Bracket,
		['`'] = SymbolType::BackTick,
		['{'] = SymbolType::Brace,
		['}'] = SymbolType::Brace,
	};

	static const inline SymbolRole s_symbolRoles [256] = {
		['"'] = SymbolRole::OpenerCloser,
		['\''] = SymbolRole::OpenerCloser,
		['('] = SymbolRole::Opener,
		[')'] = SymbolRole::Closer,
		['<'] = SymbolRole::Opener,
		['>'] = SymbolRole::Closer,
		['['] = SymbolRole::Opener,
		[']'] = SymbolRole::Closer,
		['`'] = SymbolRole::OpenerCloser,
		['{'] = SymbolRole::Opener,
		['}'] = SymbolRole::Closer,
	};

	static const inline unsigned char s_openerSymbols [256] = {
		['"'] = '"',
		['\''] = '\'',
		[')'] = '(',
		['>'] = '<',
		[']'] = '[',
		['`'] = '`',
		['}'] = '{',
	};

	struct Data {
		unsigned char symbol;
		std::size_t position;
	};

public:
	struct Status {
		bool success;
		ssize_t error_position;
	};

public:
	BalanceChecker () {
		clear ();
	}

	bool setString (std::string s) {
		m_string = std::move (s);
		return processString ();
	}

	const MyVector <MyVector <PairInfo>> & pairInfo (SymbolType t) {
		return m_pairInfos [static_cast <std::size_t> (t)];
	}

	friend std::ostream & operator<< (std::ostream & os, BalanceChecker & bc) {
		Status s = bc.m_status;

		if (true == s.success) {
			os << "BalanceChecker [success, " << bc.m_string << "]";
		}
		else {
			std::string es = bc.m_string;
			if (0 <= s.error_position && (std::size_t) s.error_position < es.size ()) {
				os << "BalanceChecker [error, " << es.substr (0, s.error_position) << "HERE" << es.substr (s.error_position) << "]";
			}
		}

		return os;
	}

private:
	bool processString () {
		clear ();

		std::size_t currentLevels [(std::size_t) SymbolType::SymbolTypeCount] = {0};

		for (std::size_t i = 0; i < m_string.size (); i++) {
			unsigned char symbol = m_string [i];
			SymbolRole role = s_symbolRoles [symbol];
			std::size_t type = static_cast <std::size_t> (s_symbolTypes [symbol]);
			// std::size_t level = m_pairInfos [static_cast <std::size_t> (type)].size ();

			if (true == m_openerCloser) {
				if (SymbolRole::OpenerCloser == role) { // closer
					if (m_stack.top ().symbol == symbol) {
						m_stack.pop ();
						m_openerCloser = false;
					}
				}
			}
			else {
				if (SymbolRole::OpenerCloser == role) { // closer
					m_stack.push (Data {
						.symbol = symbol,
						.position = i,
					});
					m_openerCloser = true;
				}
				else if (SymbolRole::Opener == role) { // opener
					m_stack.push (Data {
						.symbol = symbol,
						.position = i
					});

					if (m_pairInfos [type].size () < 1 + currentLevels [type]) {
						m_pairInfos [type].push_back (MyVector <PairInfo> {});
					}

					m_pairInfos [type] [currentLevels [type]].push_back ({
						.opener_pos = i,
						.closer_pos = i,
					});

					currentLevels [type]++;
				}
				else if (SymbolRole::Closer == role) { // closer
					if (false == m_stack.empty () && m_stack.top ().symbol == s_openerSymbols [symbol]) {
						m_stack.pop ();
						currentLevels [type]--;
						// std::cout << "SIZE: " << m_pairInfos [type].size () << ", ACCESS: " << currentLevels [type] << std::endl;
						m_pairInfos [type] [currentLevels [type]].back ().closer_pos = i;
					}
					else {
						m_status.success = false;
						m_status.error_position = i;
						break;
					}
				}

			}
		}

		if (false == m_stack.empty ()) {
			m_status.success = false;
			m_status.error_position = m_stack.top ().position;
		}

		// m_status = s;
		return m_status.success;
	}

	void clear () {
		m_openerCloser = false;
		m_stack.clear ();
		std::fill (
			std::begin (m_pairInfos),
			std::end (m_pairInfos),
			MyVector <MyVector <PairInfo>> {}
		);
		m_status = {
			.success = true,
			.error_position = 0
		};
	}

private:
	bool m_openerCloser;
	std::string m_string;
	MyStack <Data> m_stack;
	Status m_status;
	// symbol -> level -> info
	MyVector <MyVector <PairInfo>> m_pairInfos [(std::size_t) SymbolType::SymbolTypeCount];
};

struct PostfixNotationConverter {
private:
	enum class TokenType : int {
		Operator,
		Operand,
		Separator,
	};
	struct Token {
		TokenType type;
		std::string value;
	};

	static inline const TokenType s_symbolType [256] = {
		[' '] = TokenType::Separator,
		['*'] = TokenType::Operator,
		['+'] = TokenType::Operator,
		['-'] = TokenType::Operator,
		['/'] = TokenType::Operator,
		['0'] = TokenType::Operand,
		['1'] = TokenType::Operand,
		['2'] = TokenType::Operand,
		['3'] = TokenType::Operand,
		['4'] = TokenType::Operand,
		['5'] = TokenType::Operand,
		['6'] = TokenType::Operand,
		['7'] = TokenType::Operand,
		['8'] = TokenType::Operand,
		['9'] = TokenType::Operand,
	};

	struct Operations {
		static int Add (int a, int b) { return a + b; }
		static int Subtract (int a, int b) { return a - b; }
		static int Multiply (int a, int b) { return a * b; }
		static int Divide (int a, int b) { return a / b; }
	};

	typedef int (* Operation) (int, int);

	static inline const Operation OperatorOperations [/* 256 */]  = {
		['+'] = & Operations::Add,
		['-'] = & Operations::Subtract,
		['*'] = & Operations::Multiply,
		['/'] = & Operations::Divide,
	};


public:
	static std::string Encode (const std::string & s) {
		(void) s;
		throw "NOT IMPLEMENTED";
	}

	static std::string Decode (const std::string & s) {
		MyQueue <Token> tokens = Tokenize (s);
		MyStack <int> values;
		MyStack <std::string> expressions;

		while (false == tokens.empty ()) {
			Token t = tokens.front ();
			tokens.pop ();
			if (TokenType::Operand == t.type) {
				values.push (std::stoi (t.value));
				expressions.push (t.value);
			}
			else {
				if (values.size () >= 2) {
					int o2 = values.top ();
					values.pop ();
					int o1 = values.top ();
					values.pop ();
					values.push (OperatorOperations [(unsigned) t.value [0]] (o1, o2));

					std::string eo2 = expressions.top ();
					expressions.pop ();
					std::string eo1 = expressions.top ();
					expressions.pop ();

					if ('*' == t.value [0] || '/' == t.value [0]) {
						if (true == NeedParenthesis (eo1)) {
							eo1 = '(' + eo1 + ')';
						}
						if (true == NeedParenthesis (eo2)) {
							eo2 = '(' + eo2 + ')';
						}
					}

					expressions.push (eo2 + ' ' + t.value + ' ' + eo1);
				}
			}
		}

		return expressions.top () + " = " + std::to_string (values.top ());
	}

private:
	static MyQueue <Token> Tokenize (const std::string & s) {
		MyQueue <Token> tokens;

		bool operand = false;
		std::string curr_operand;

		for (char c : s) {
			TokenType t = s_symbolType [(unsigned char) c];
			if (TokenType::Operand == t) {
				if (false == operand) {
					operand = true;
					curr_operand = c;
				}
				else {
					curr_operand += c;
				}
			}
			else {
				if (true == operand) {
					operand = false;
					tokens.push (Token {
						.type = TokenType::Operand,
						.value = curr_operand
					});
				}

				if (TokenType::Operator == t) {
					tokens.push (Token {
						.type = TokenType::Operator,
						.value = std::string {} + c
					});
				}
			}
		}

		if (true == operand) {
			tokens.push (Token {
				.type = TokenType::Operand,
				.value = curr_operand
			});
		}

		return tokens;
	}

	static bool NeedParenthesis (std::string str) {
		BalanceChecker ch;
		ch.setString (str);
		const auto & parenInfo = ch.pairInfo(BalanceChecker::SymbolType::Parenthesis);
		if (false == parenInfo.empty ()) {
			const auto & firstLevelInfo = parenInfo [0];
			for (const auto & info : firstLevelInfo) {
				std::cout << "I: " << info.opener_pos << " - " << info.closer_pos << std::endl;
				std::fill (str.begin () + info.opener_pos, str.begin () + info.closer_pos, ' ');
			}
		}

		if (
			   str.cend () != std::find (str.cbegin (), str.cend (), '+')
			|| str.cend () != std::find (str.cbegin (), str.cend (), '-')
		) {
			return true;
		}
		else {
			return false;
		}
	}
};

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

	std::cout << PostfixNotationConverter::Decode ("6 5 2 3 + 8 * + 3 + *") << std::endl;;
}
