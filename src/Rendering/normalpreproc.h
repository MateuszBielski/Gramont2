#ifndef PomPreproc_H
#define PomPreproc_H
#include <vector>
#include <string>
#include "rspreproc.h"

#define NORMAL_SH_ATTR aPos, aNormal, aTexCoords, normalShAttrSize // aTangent, aBitangent, 
#define NORMAL_SH_UNIF mMVP, mToViewSpace, lightColour, lightProps, diffuseMap, normalMap, normalEnabled, normalShUnifSize //depthMap, viewPos, lightPos, model, 
MA_CreateEnum(normalShAttr, NORMAL_SH_ATTR)
MA_CreateEnum(normalShUnif, NORMAL_SH_UNIF)

#endif
