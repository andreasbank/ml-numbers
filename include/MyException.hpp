#include <exception>

class MyException: public std::exception {
public:
	MyException(const std::string& what): m_what(what) {}
	MyException(const char *what): m_what(what) {}
	virtual const char* what() const throw() {
		return m_what.c_str();
	}

private:
	std::string m_what;
};
