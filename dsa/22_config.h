# ifndef CONFIG_H_22
# define CONFIG_H_22

# include "20_vector.h"
# include "21_list.h"
# include "22_stack.h"
# include "22_queue.h"

# include <vector>
# include <list>
# include <queue>
# include <stack>

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

# endif // CONFIG_H_22
