#ifndef PomPreproc_H
#define PomPreproc_H
#include <vector>
#include <string>
#include "rspreproc.h"
//https://cplusplus.com/articles/2wA0RXSz/
//const vector<string> explode(const string& s, const char& c);
//const vector<string> trimLast(const vector<string>&& );

#define POM_SH_ATTR aPos, aNormal, aTexCoords, aTangent, aBitangent, pomShAttrSize
#define POM_SH_UNIF model, mMVP, mToViewSpace, mInvModelView, lightPos, lightColour, lightProps, viewPos, diffuseMap, normalMap, depthMap, pomEnabled, pomShUnifSize
//#define MA_CreateEnum( className, ...) enum class className { __VA_ARGS__};
//#define Q(...) #__VA_ARGS__
//#define MA_CreateStrings( varName, ...) const vector<string> varName = trimLast(explode( Q(__VA_ARGS__), ','));

MA_CreateEnum(pomShAttr, POM_SH_ATTR)
MA_CreateEnum(pomShUnif, POM_SH_UNIF)

#endif
