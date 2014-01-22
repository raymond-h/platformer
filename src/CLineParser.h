#include <string>
#include <sstream>

#ifndef CLINE_PARSER_H_
#define CLINE_PARSER_H_

class CLineParser {
public:
	CLineParser(int argc, char** argv):argc(argc),argv(argv),curarg(0) {}

	void skip() { if(curarg < argc) curarg++; };

	std::string next() { return (curarg < argc) ? argv[curarg++] : nullptr; };

	bool hasNext() { return curarg < argc; };

	template<typename T>
	T nextAs() {
		T t = T();
		nextAs(&t);
		return t;
	}

	template<typename T>
	void nextAs(T* t) {
		std::istringstream sin(next());
		sin >> *t;
	}

	void nextAs(std::string* s) {
		*s = next();
	}

private:
	int argc;
	char** argv;

	int curarg;
};

#endif /* CLINE_PARSER_H_ */