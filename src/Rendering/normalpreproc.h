#ifndef NormalPreproc_H
#define NormalPreproc_H
#include <vector>
#include <string>
#include "rspreproc.h"

#define NORMAL_SH_ATTR aPos, aNormal, aTangent, aBitangent, aTexCoords, normalShAttrSize //  
MA_CreateEnum(normalShAttr, NORMAL_SH_ATTR)

#define NORMAL_SH_UNIF model, mMVP, mToViewSpace, lightColour, lightProps, lightPos, viewPos, diffuseMap, normalMap, normalEnabled, normalShUnifSize
MA_CreateEnum(normalShUnif, NORMAL_SH_UNIF)


#endif
