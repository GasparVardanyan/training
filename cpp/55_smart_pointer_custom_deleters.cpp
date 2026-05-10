# include <sys/stat.h>
# include <cstdio>
# include <iostream>
# include <memory>
# include <type_traits>



template <auto DeleteFn>
struct FunctionDeleter {
	template <typename T>
	requires std::is_invocable_v <decltype (DeleteFn), T *>
	void operator() (T * ptr) const {
		std::cout << "=== DELETE ===" << std::endl;
		DeleteFn (ptr);
	}
};

int main () {
	const char * const file = "/etc/os-release";

	if (
		struct stat s
		;
		   0 == stat (file, & s)
		&& S_IFREG == (s.st_mode & S_IFMT)
	) {
		std::cout << "=== OPENING ===" << std::endl;

		std::unique_ptr <FILE, decltype ([] (FILE * f) -> void {
			std::cout << "=== CLOSING ===" << std::endl;
			if (NULL != f) {
				fclose (f);
			}
		})> fh (fopen (file, "r"));

		if (NULL != fh.get ()) {
			std::shared_ptr <char []> buf = std::make_shared <char []> (s.st_size + 1);

			fread (buf.get (), s.st_size, 1, fh.get ());
			buf [s.st_size] = 0;

			std::cout << "buf {{{\n" << buf << (buf [s.st_size - 1] == '\n' ? "" : "\n") << "}}}" << std::endl;
		}

		fh.reset ();

		std::cout << "=== CREATE ===" << std::endl;
		std::unique_ptr <FILE, FunctionDeleter <fclose>> fh1 (fopen (file, "r"));
	}
	else {
		perror ("Failed to access the file");
		fflush (stdout);
	}
}
