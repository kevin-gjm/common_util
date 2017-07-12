#ifndef UTIL_HTTP_H_
#define UTIL_HTTP_H_

#include <string>
using std::string;

class Http
{
public:
	static int Post(const string& url, const string& postfields, string& out);
	static int Get(const string&url, string&out);
};

#endif //!UTIL_HTTP_H_





