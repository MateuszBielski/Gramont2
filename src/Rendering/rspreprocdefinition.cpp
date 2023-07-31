#include "rspreproc.h"

//https://cplusplus.com/articles/2wA0RXSz/
const vector<string> explode(const string& s, const char& c)
{
    string buff { "" };
    vector<string> v;

    for (auto n : s) {
        if (n != c) buff += n;
        else if (n == c && buff != "") {
            size_t start = buff.find_first_not_of(" ");
            buff = (start == std::string::npos) ? buff : buff.substr(start);
//            size_t start = s.find_first_not_of(WHITESPACE);
//            return (start == std::string::npos) ? "" : s.substr(start);
            v.push_back(buff);
            buff = "";
        }
    }
    if (buff != "") {
        v.push_back(buff);
    }

    return v;
}

const vector<string> trimLast(const vector<string>&& v)
{
	int newSize = v.size() - 1;
    vector<string> result(newSize);
    for(int i = 0 ; i < newSize ; i++)result[i] = std::move(v[i]);
    return result;
}