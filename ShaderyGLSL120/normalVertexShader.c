#version 120
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec3 aTangent;
attribute vec3 aBitangent;
attribute vec2 aTexCoords;

uniform mat4 model;
uniform mat4 mMVP;
uniform mat4 mToViewSpace;

uniform vec3 lightPos;
uniform vec3 viewPos;

varying vec3 theNormal;
varying vec3 pointPos;
varying vec2 textCoord;
varying vec3 FragPos;
varying vec3 TangentLightPos;
varying vec3 TangentViewPos;
 varying vec3 TangentFragPos;
void main(void)
{
    
    textCoord = aTexCoords;
    vec4 temp4 = mToViewSpace * vec4(aPos, 1.0);
    pointPos = temp4.xyz;
    temp4 = mToViewSpace * vec4(aNormal, 0.0);
    theNormal = normalize(temp4.xyz);

    /*mb proba*/
    temp4 = mToViewSpace * vec4(aTangent, 1.0);
    vec3 theTangent = temp4.xyz;
    temp4 = mToViewSpace * vec4(aBitangent, 1.0);
    vec3 theBitangent = temp4.xyz;
    temp4 = mToViewSpace * vec4(viewPos, 1.0);
    vec3 theViewPos = temp4.xyz;
    temp4 = mToViewSpace * vec4(lightPos, 1.0);
    vec3 theLightPos = temp4.xyz;
    /*normal*/
    FragPos = vec3(model * vec4(aPos, 1.0));
    

    vec3 T = normalize(mat3(model) * theTangent);
    //vec3 T = normalize(mat3(mToViewSpace) * aTangent);
    vec3 B = normalize(mat3(model) * theBitangent);
    //vec3 B = normalize(mat3(mToViewSpace) * aBitangent);
    vec3 N = normalize(mat3(model) * theNormal);
    //vec3 N = normalize(mat3(mToViewSpace) * aNormal);
    mat3 TBN = mat3(T, B, N);
    //mat3 TBN = mat3(B, T, N);
    //mat3 TBN = mat3(B, N, T);
    //mat3 TBN = transpose(mat3(T, B, N));

    TangentLightPos = TBN * lightPos;
    //TangentLightPos = TBN * theLightPos;//bad result
    TangentViewPos = TBN * viewPos;
    //TangentViewPos = TBN * theViewPos;//bad result
    TangentFragPos = TBN * FragPos;

    gl_Position = mMVP * vec4(aPos, 1.0);
}
