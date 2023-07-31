#ifndef PomPreproc_H
#define PomPreproc_H
#include <vector>
#include <string>
#include "rspreproc.h"

#define NORMAL_SH_ATTR aPos, aNormal, aTexCoords, aTangent, aBitangent, pomShAttrSize
#define NORMAL_SH_UNIF model, mMVP, mToViewSpace, lightPos, lightColour, lightProps, viewPos, diffuseMap, normalMap, depthMap, pomEnabled, pomShUnifSize

MA_CreateEnum(normalShAttr, NORMAL_SH_ATTR)
MA_CreateEnum(normalShUnif, NORMAL_SH_UNIF)

#endif
