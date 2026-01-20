# include <algorithm>
# include <iostream>
# include <iterator>
# include <list>
# include <ostream>
# include <queue>
# include <stack>
# include <string>
# include <vector>

# include "20_vector.h"
# include "21_list.h"
# include "22_stack.h"
# include "22_queue.h"



template <typename T>
// using MyVector = std::vector <T>;
using MyVector = vector <T>;

template <typename T>
// using MyList = std::list <T>;
using MyList = list <T>;

template <typename T>
// using MyStack = std::stack <T, MyVector <T>>;
using MyStack = stack <T, MyVector>;

template <typename T>
// using MyQueue = std::queue <T, MyList <T>>;
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
		reset ();
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
		reset ();

		for (std::size_t i = 0; i < m_string.size (); i++) {
			unsigned char symbol = m_string [i];
			SymbolRole role = s_symbolRoles [symbol];
			SymbolType type = s_symbolTypes [symbol];

			if (true == m_openerCloser) {
				if (SymbolRole::OpenerCloser == role) { // closer
					if (m_stack.top ().symbol == symbol) {
						m_stack.pop ();
						regCloser (type, i);

						m_openerCloser = false;
					}
				}
			}
			else {
				if (SymbolRole::OpenerCloser == role) { // opener
					m_stack.push (Data {
						.symbol = symbol,
						.position = i,
					});
					m_openerCloser = true;

					regOpener (type, i);
				}
				else if (SymbolRole::Opener == role) { // opener
					m_stack.push (Data {
						.symbol = symbol,
						.position = i
					});

					regOpener (type, i);
				}
				else if (SymbolRole::Closer == role) { // closer
					if (false == m_stack.empty () && m_stack.top ().symbol == s_openerSymbols [symbol]) {
						m_stack.pop ();
						regCloser (type, i);
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

	void regOpener (SymbolType type, std::size_t position) {
		std::size_t _type = static_cast <std::size_t> (type);

		if (m_pairInfos [_type].size () < 1 + m_currentLevels [_type]) {
			m_pairInfos [_type].push_back (MyVector <PairInfo> {});
		}

		m_pairInfos [_type] [m_currentLevels [_type]].push_back ({
			.opener_pos = position,
			.closer_pos = position,
		});

		m_currentLevels [_type]++;
	}

	void regCloser (SymbolType type, std::size_t position) {
		std::size_t _type = static_cast <std::size_t> (type);
		m_currentLevels [_type]--;
		m_pairInfos [_type] [m_currentLevels [_type]].back ().closer_pos = position;
	}

	void reset () {
		m_openerCloser = false;
		// m_stack.clear ();
		m_stack = {};
		std::fill (
			std::begin (m_pairInfos),
			std::end (m_pairInfos),
			MyVector <MyVector <PairInfo>> {}
		);
		std::fill (
			std::begin (m_currentLevels),
			std::end (m_currentLevels),
			0
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
	// symbol type -> level -> info
	MyVector <MyVector <PairInfo>> m_pairInfos [(std::size_t) SymbolType::SymbolTypeCount];
	std::size_t m_currentLevels [(std::size_t) SymbolType::SymbolTypeCount];
};



// NOTE: no negative numbers, no parenthesis, integer division only
struct PostfixNotationConverter {
private:
	enum class TokenType : int {
		Operator,
		Operand,
		GroupOpener,
		GroupCloser,
		Separator,
	};
	struct Token {
		TokenType type;
		std::string value;
	};

	static inline const TokenType s_symbolType [256] = {
		[' '] = TokenType::Separator,
		['('] = TokenType::GroupOpener,
		[')'] = TokenType::GroupCloser,
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
		MyQueue <Token> tokens = Tokenize (s);

		MyStack <MyStack <int>> values;           values.push (MyStack <int> ());
		MyStack <MyStack <char>> operators;       operators.push (MyStack <char> ());
		MyStack <std::string> results;            results.push ("");

		auto processLast = [&] () -> void {
			MyStack <int> & curr_values = values.top ();
			MyStack <char> & curr_ops = operators.top ();
			std::string & curr_res = results.top ();

			char op = curr_ops.top ();
			curr_ops.pop ();
			int v2 = curr_values.top ();
			curr_values.pop ();
			int v1 = curr_values.top ();
			curr_values.pop ();
			curr_values.push (OperatorOperations [(unsigned char) op] (v1, v2));

			curr_res += op + std::string {' '};

			// std::cout << "valueStack: {";
			// for (const auto & c : values.container ()) {
			// 	std::cout << '\t';
			// 	for (int i : c.container ()) {
			// 		std::cout << i << ", ";
			// 	}
			// 	std::cout << std::endl;
			// }
			// std::cout << '}' << std::endl;
		};

		auto processValue = [&] (int value, bool newValue = false) -> void {
			MyStack <int> & curr_vals = values.top ();
			MyStack <char> & curr_ops = operators.top ();
			std::string & curr_res = results.top ();
			curr_vals.push (value);

			if (false == curr_ops.empty ()) {
				char op = curr_ops.top ();

				if ('*' == op || '/' == op) {
					if (true == newValue) {
						curr_res += std::to_string (value) + ' ';
						newValue = false;
					}

					processLast ();
				}

				if (curr_ops.size () > 1) {
					unsigned char last_op = curr_ops.top ();
					curr_ops.pop ();
					int last_v = curr_vals.top ();
					curr_vals.pop ();

					while (false == curr_ops.empty ()) {
						processLast ();
					}

					curr_ops.push (last_op);
					curr_vals.push (last_v);
				}
			}

			if (true == newValue) {
				curr_res += std::to_string (value) + ' ';
			}
		};

		while (false == tokens.empty ()) {
			Token t = tokens.front ();
			tokens.pop ();

			if (TokenType::GroupOpener == t.type) {
				values.push (MyStack <int> ());
				operators.push (MyStack <char> ());
				results.push ("");
			}
			else if (TokenType::Operator == t.type) {
				operators.top ().push (t.value [0]);
			}
			else if (TokenType::Operand == t.type) {
				processValue (std::stoi (t.value), true);
			}
			else if (TokenType::GroupCloser == t.type) {
				if (false == operators.top ().empty ()) {
					processLast ();
				}

				int v = values.top ().top ();

				values.pop ();
				operators.pop ();
				std::string last = results.top ();
				results.pop ();
				results.top () += last;

				processValue (v);
			}
		}

		MyStack <int> & curr_values = values.top ();
		MyStack <char> & curr_ops = operators.top ();

		if (false == curr_ops.empty ()) {
			processLast ();
		}

		return results.top () + " = " + std::to_string (curr_values.top ());
	}

	static std::string EncodeShuntingYard (const std::string & s) {
		std::string res = "";

		MyStack <char> opStack;

		static const char opPriority [256] = {
			['('] = 0,
			['*'] = 1,
			['/'] = 1,
			['+'] = 2,
			['-'] = 2,
		};

		static const MyVector <char> opList {'*', '/', '+', '-'};

		MyStack <int> valueStack;

		std::string currOperand;

		auto updateValueStack = [&] (char op) -> void {
			int v2 = valueStack.top ();
			valueStack.pop ();
			int v1 = valueStack.top ();
			valueStack.pop ();

			valueStack.push (OperatorOperations [(unsigned char) op] (v1, v2));
		};

		for (char c : s) {
			if ('0' <= c && c <= '9') {
				currOperand += c;
			}
			else {
				if (false == currOperand.empty ()) {
					res += currOperand + ' ';
					valueStack.push (std::stoi (currOperand));
					currOperand.clear ();
				}
				if ('(' == c) {
					opStack.push (c);
				}
				else if (')' == c) {
					while (false == opStack.empty ()) {
						char t = opStack.top ();
						opStack.pop ();

						if ('(' == t) {
							break;
						}
						else {
							res += t;
							res += ' ';
							updateValueStack (t);
						}
					}
				}
				else if (opList.cend () != std::find (opList.cbegin (), opList.cend (), c)) {
					while (false == opStack.empty ()) {
						char t = opStack.top ();

						int cP = opPriority [(unsigned char) c];
						int tP = opPriority [(unsigned char) t];

						if ('(' == t || cP < tP) {
							break;
						}
						else {
							res += t;
							res += ' ';
							updateValueStack (t);
							opStack.pop ();
						}
					}

					opStack.push (c);
				}
			}
		}

		if (false == currOperand.empty ()) {
			res += currOperand + ' ';
			valueStack.push (std::stoi (currOperand));
		}

		while (false == opStack.empty ()) {
			res += opStack.top ();
			res += ' ';
			updateValueStack (opStack.top ());
			opStack.pop ();
		}

		res += " = " + std::to_string (valueStack.top ());

		return res;
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

					expressions.push (eo1 + ' ' + t.value + ' ' + eo2);
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

				if (TokenType::Separator != t) {
					tokens.push (Token {
						.type = t,
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

	std::string res = "(6 * (5 + 3 + 2)) * 10 / 2 - 42";
	std::cout << res << std::endl;

	res = PostfixNotationConverter::EncodeShuntingYard (res);
	std::cout << res << std::endl;

	res.resize (res.find ('='));
	res = PostfixNotationConverter::Decode (res);
	std::cout << res << std::endl;

	res.resize (res.find ('='));
	res = PostfixNotationConverter::Encode (res);
	std::cout << res << std::endl;

	res.resize (res.find ('='));
	res = PostfixNotationConverter::Decode (res);
	std::cout << res << std::endl;
}
