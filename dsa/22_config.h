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
	template <typename ...> typename Vector__ = vector,
	template <typename ...> typename List__ = list,
	template <typename ...> typename Stack__ = stack,
	template <typename ...> typename Queue__ = queue,
	template <typename> typename StackContainer__ = Vector__,
	template <typename> typename QueueContainer__ = List__
>
struct TypeConfig {
public:
	template <typename ... TS> using Vector = Vector__ <TS ...>;
	template <typename ... TS> using List = List__ <TS ...>;
	template <typename T> using Stack = Stack__ <T, StackContainer__ <T>>;
	template <typename T> using Queue = Queue__ <T, QueueContainer__ <T>>;

	using TypeConfigCustom = TypeConfig <vector, list, stack, queue>;
	using TypeConfigStd = TypeConfig <std::vector, std::list, std::stack, std::queue>;
	using TypeConfigMix1 = TypeConfig <vector, list, stack, queue, std::vector, std::list>;
	using TypeConfigMix2 = TypeConfig <vector, list, std::stack, std::queue, vector, list>;
};

# endif // CONFIG_H_22
