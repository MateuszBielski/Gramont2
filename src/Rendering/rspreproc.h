#ifndef RsPreproc_H
#define RsPreproc_H
#include <vector>
#include <string>

using std::string, std::vector;

const vector<string> explode(const string& s, const char& c);
const vector<string> trimLast(const vector<string>&& );

#define MA_CreateEnum( className, ...) enum class className { __VA_ARGS__};
#define Q(...) #__VA_ARGS__
#define MA_CreateStrings( varName, ...) const vector<string> varName = trimLast(explode( Q(__VA_ARGS__), ','));

#endif
