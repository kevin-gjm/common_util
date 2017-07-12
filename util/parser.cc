#include "parser.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp> 
#include <boost/property_tree/ini_parser.hpp>

using std::shared_ptr;
using std::ofstream;
using std::ifstream;

int Json::WriteJsonToFile(const string& strFilename, const ptree& json)
{
	try {
		std::shared_ptr<ofstream> save(
			new ofstream(strFilename),
			[](ofstream * file) {
			if (file)
			{
				if (file->is_open())
					file->close();
				delete file;
			}
		});
		if (save->is_open())
		{
			write_json(*save.get(), json);
			return 0;
		}
		else
			return -1;
	}
	catch (...)
	{
		return -2;
	}
}

int Json::ReadJsonFromFile(const string& strFilename, ptree& json)
{
	try
	{
		std::shared_ptr<ifstream> filein(
			new ifstream(strFilename),
			[](ifstream * file)
		{
			if (file->is_open())
				file->close();
			delete file;
		});

		if (filein->is_open() != false)
		{
			read_json(*filein, json);
			return 0;
		}
		else
			return -1;
	}
	catch (...)
	{
		return -2;
	}
}

int Json::JsonToString(const ptree& json, string& out)
{
	try
	{
		std::stringstream out_ss;
		write_json(out_ss, json);
		out = string(out_ss.str().c_str(), out_ss.str().size());
		return 0;
	}
	catch (...)
	{
		return -1;
	}

}
int Json::StringToJson(const string& in, ptree& out)
{
	try
	{
		std::stringstream in_ss;
		in_ss << in;
		read_json(in_ss, out);
		return 0;
	}
	catch (...)
	{
		return -1;
	}

}


static bool is_file_exit_and_regular(const std::string& filename)
{
	boost::filesystem::path p(filename);
	if (exists(p))
	{
		if (is_regular_file(p))
		{
			return true;
		}
		return false;
	}
	return false;
}

int IniParser::Init()
{
	if (!is_file_exit_and_regular(m_strFilename)) return -1;
	try {
		boost::property_tree::ini_parser::read_ini(m_strFilename, m_ptContent);
	}
	catch (...)
	{
		return -1;
	}
	return 0;
}

int IniParser::GetString(const string strNodeID, string& strOut)
{
	try
	{
		strOut = m_ptContent.get<std::string>(strNodeID, "");
		return 0;
	}
	catch (...)
	{
		return -1;
	}

}
int IniParser::GetInt(const string strNodeID, int& iOut)
{
	try
	{
		iOut = m_ptContent.get<int>(strNodeID, 0);
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}


/*

#include <iostream>
#include <string>
#include <utility>
#include "json.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace  boost::property_tree::json_parser;
using namespace std;
using namespace boost::property_tree;
using std::string;


int main()
{
try {
string name = "hello.json";
ptree config;
config.put("name", "name1");
config.put("passwd", "passwd1");

ptree ptChildRead;
for (int i = 1; i <= 100000; i++)
{
ptree st;
st.put("", std::to_string(i));
ptChildRead.push_back(make_pair("", st));
//config.add_child("list", st);
}
config.add_child("list", ptChildRead);


Json::WriteJsonToFile(name, config);

ptree getjson;
Json::ReadJsonFromFile(name, getjson);

std::stringstream out_ss;
write_json(out_ss, getjson);
string jjj(out_ss.str());
std::cout << jjj;
cin.get();


if (getjson.count("list"))
{
ptree ptChildRead = getjson.get_child("list");
for (auto pos = ptChildRead.begin(); pos != ptChildRead.end(); ++pos)
{
string job = pos->second.get_value<string>();
std::cout << job << std::endl;
}
}
else
{
return -1;
}
}
catch (...)
{
int i = 0;
i++;
}

}

*/
