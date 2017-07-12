#ifndef UTIL_PARSER_H_
#define UTIL_PARSER_H_

#include <string>
#include <boost/property_tree/ptree.hpp>

using namespace boost::property_tree;
using std::string;

class Json 
{
public:
	static int WriteJsonToFile(const string& strFilename, const ptree& json);
	static int ReadJsonFromFile(const string& strFilename, ptree& json);
	static int JsonToString(const ptree& json, string& out);
	static int StringToJson(const string& in, ptree& out);
};

class IniParser
{
public:
	explicit IniParser(string strFilename)
		:m_strFilename(strFilename)
	{
		Init();
	}
	int GetString(const string strNodeID, string& strOut);
	int GetInt(const string strNodeID, int& iOut);

private:
	int Init();
	string m_strFilename;
	boost::property_tree::ptree m_ptContent;
};



#endif //UTIL_PARSER_H_