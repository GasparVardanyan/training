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
	template <typename ...> typename Vector_ = vector,
	template <typename ...> typename List_ = list,
	template <typename ...> typename Stack_ = stack,
	template <typename ...> typename Queue_ = queue,
	template <typename> typename StackContainer_ = Vector_,
	template <typename> typename QueueContainer_ = List_
>
struct TypeConfig {
public:
	template <typename ... TS> using Vector = Vector_ <TS ...>;
	template <typename ... TS> using List = List_ <TS ...>;
	template <typename T> using Stack = Stack_ <T, StackContainer_ <T>>;
	template <typename T> using Queue = Queue_ <T, QueueContainer_ <T>>;

	using TypeConfigCustom = TypeConfig <vector, list, stack, queue>;
	using TypeConfigStd = TypeConfig <std::vector, std::list, std::stack, std::queue>;
	using TypeConfigMix1 = TypeConfig <vector, list, stack, queue, std::vector, std::list>;
	using TypeConfigMix2 = TypeConfig <vector, list, std::stack, std::queue, vector, list>;
};

# endif // CONFIG_H_22
