# include <iostream>
# include <vector>

template <typename T>
concept Comparable = requires (T t1, T t2) {
	t1 == t2;
};

template <Comparable T>
ssize_t binSearch (const std::vector <T> & v, const T & value, ssize_t min, ssize_t max) {
	ssize_t mid = (min + max) / 2;
	if (max < min) {
		return -1;
	}
	if (v [mid] == value) {
		return mid;
	}
	else {
		if (min == max) {
			return -1;
		}
		else if (v [mid] > value) {
			return binSearch (v, value, min, mid - 1);
		}
		else {
			return binSearch (v, value, mid + 1, max);
		}
	}
}

template <Comparable T>
ssize_t binSearch (const std::vector <T> & v, const T & value) {
	return binSearch (v, value, 0, v.size () - 1);
}

template <Comparable T>
ssize_t binSearch2 (const std::vector <T> & v, const T & value) {
	ssize_t min = 0, max = v.size () - 1;

	while (min <= max) {
		ssize_t mid = (min + max) / 2;

		if (v [mid] == value) {
			return mid;
		}
		else if (v [mid] > value) {
			max = mid - 1;
		}
		else {
			min = mid + 1;
		}
	}

	return -1;
}

int main ()
{
	std::vector <int> v {1, 5, 8, 11};
	std::cout << binSearch (v, 0) << std::endl;
	std::cout << binSearch2 (v, 0) << std::endl;
}
