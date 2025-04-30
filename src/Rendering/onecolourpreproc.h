#ifndef OneColPreproc_H
#define OneColPreproc_H
#include <vector>
#include <string>
#include "rspreproc.h"
//https://cplusplus.com/articles/2wA0RXSz/
//const vector<string> explode(const string& s, const char& c);
//const vector<string> trimLast(const vector<string>&& );

#define ONECOL_SH_ATTR aPos, aNormal, oneColShAttrSize
#define ONECOL_SH_UNIF mMVP, mToViewSpace, lightColour, lightProps, oneColShUnifSize


MA_CreateEnum(oneColShAttr, ONECOL_SH_ATTR)
MA_CreateEnum(oneColShUnif, ONECOL_SH_UNIF)

#endif
