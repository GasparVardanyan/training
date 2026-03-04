# include <stdint.h>
# include <uchar.h>

# define IS_SAME_TYPE(T1, T2) _Generic (((T1) 0), T2 : 1, default : 0)

_Static_assert (IS_SAME_TYPE (char16_t, uint_least16_t), "ERR");
_Static_assert (IS_SAME_TYPE (char32_t, uint_least32_t), "ERR");

int main (void) {

}
