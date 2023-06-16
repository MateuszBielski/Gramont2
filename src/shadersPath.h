#ifndef SHADERS_PATH_H
#define SHADERS_PATH_H

//#define d_path "ShaderyCoreProfile/"

#define _d_path "ShaderyGLSL120/"

#ifdef TESTOWANIE_F
#define d_path "Gramont2Test/" _d_path
#else
#define d_path _d_path
#endif

#define d_vertexShaderPath d_path"triangVertexShader.c"
#define d_fragmentShaderPath d_path"triangFragmentShader.c"
#define d_textureVertexShaderPath d_path"textureVertexShader.c"
#define d_textureFragmentShaderPath d_path"textureFragmentShader.c"
#define d_illuminationShaderPath d_path"illuminationShader.c"
#define d_vertexPickingShaderPath d_path"pickingVertexShader.c"
#define d_fragmentPickingShaderPath d_path"pickingFragmentShader.c"
#define d_pomVertexShaderPath d_path"pomVertexShader.c"
#define d_pomFragmentShaderPath d_path"pomFragmentShader.c"

#endif
