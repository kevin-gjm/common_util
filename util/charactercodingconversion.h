#ifndef CHARACTERCODINGCONVERSION_H
#define CHARACTERCODINGCONVERSION_H

#include <string>

//gbk转UTF-8
std::string GbkToUtf8(const std::string& strGbk);// 传入的strGbk是GBK编码

//UTF-8转gbk
std::string Utf8ToGbk(const std::string& strUtf8);

//gbk转unicode,"UCS-2LE"代表unicode小端模式
std::string GbkToUnicode(const std::string& strGbk);// 传入的strGbk是GBK编码

//unicode转gbk
std::string UnicodeToGbk(const std::string& strGbk);// 传入的strGbk是GBK编码

#endif // CHARACTERCODINGCONVERSION_H
