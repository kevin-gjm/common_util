#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_
#include <exception>
#include <string>
#include <string.h>

using std::exception;
using std::string;

class Noncopyable {
protected:
	Noncopyable() {}
	~Noncopyable() {}
private:
	Noncopyable(const Noncopyable&);
	const Noncopyable& operator=(const Noncopyable&);
};

class SysCallException : public exception {
public:
	SysCallException(int errCode, const string& errMsg, bool detail = true) : _errCode(errCode), _errMsg(errMsg) {
		if (detail) {
			_errMsg.append(", ").append(::strerror(errCode));
		}
	}

	virtual ~SysCallException() throw () {}

	int getErrorCode() const throw () {
		return _errCode;
	}

	const char* what() const throw () {
		return _errMsg.c_str();
	}

protected:
	int     _errCode;
	string  _errMsg;
};


#endif // !UTIL_UTIL_H_

