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

template <
	template <typename ...> typename _Vector = vector,
	template <typename ...> typename _List = list,
	template <typename ...> typename _Stack = stack,
	template <typename ...> typename _Queue = queue,
	template <typename> typename _StackContainer = _Vector,
	template <typename> typename _QueueContainer = _List
>
struct TypeConfig {
public:
	template <typename ... TS> using Vector = _Vector <TS ...>;
	template <typename ... TS> using List = _List <TS ...>;
	template <typename T> using Stack = _Stack <T, _StackContainer <T>>;
	template <typename T> using Queue = _Queue <T, _QueueContainer <T>>;

	using TypeConfigCustom = TypeConfig <vector, list, stack, queue>;
	using TypeConfigStd = TypeConfig <std::vector, std::list, std::stack, std::queue>;
	using TypeConfigMix1 = TypeConfig <vector, list, stack, queue, std::vector, std::list>;
	using TypeConfigMix2 = TypeConfig <vector, list, std::stack, std::queue, vector, list>;
};

# endif // CONFIG_H_22
