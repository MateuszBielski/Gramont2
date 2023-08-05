#ifndef PomPreproc_H
#define PomPreproc_H
#include <vector>
#include <string>
#include "rspreproc.h"

#define NORMAL_SH_ATTR aPos, aNormal, aTangent, aBitangent, aTexCoords, normalShAttrSize //  
#define NORMAL_SH_UNIF model, mMVP, mToViewSpace, lightColour, lightProps, lightPos, viewPos, diffuseMap, normalMap, normalEnabled, normalShUnifSize //depthMap, 
MA_CreateEnum(normalShAttr, NORMAL_SH_ATTR)
MA_CreateEnum(normalShUnif, NORMAL_SH_UNIF)

#endif
