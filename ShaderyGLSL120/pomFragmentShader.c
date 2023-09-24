#version 120
varying vec2 textCoord;
varying vec3 theNormal;
varying vec3 pointPos;
varying mat4 transform;
varying vec3 viewPosVary;
varying mat4 invTbnModelView4;

uniform vec4 lightProps;
uniform vec3 lightColour;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform int pomEnabled;

uniform float heightScale = 0.05;

//https://web.eecs.umich.edu/~sugih/courses/eecs487/lectures/18-Shaders.pdf

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    
   
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / -viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = abs(texture2D(depthMap, currentTexCoords).r);

    int count = 1;
//
    while(currentLayerDepth < currentDepthMapValue) {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture2D(depthMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
        count +=1;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture2D(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
    
//    if(currentDepthMapValue * viewDir.z < 0.15 && viewDir.z < 0.3) return vec2(1.1, 1.1);
//    if(currentDepthMapValue * viewDir.z < 0.10) return vec2(1.1, 1.1);
//    if(beforeDepth * viewDir.z < 0.15) return vec2(1.1, 1.1);

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
    
    return finalTexCoords;
}
vec3 Illuminate(vec4 LiProps, vec3 LiColour, vec4 PColour,
                vec3 PNormal, vec3 PPos);
void main(void)
{
//    if(gl_PointCoord.x > 0.2 && gl_PointCoord.y > 0.2) discard;
    vec3 normal;
    vec2 modTexCoords = textCoord;

//    vec3 viewDir = normalize(viewPos - pointPos);

    if (pomEnabled == 1) {
		
		vec3 viewDir = normalize (viewPosVary - pointPos);
		vec4 tempVd = invTbnModelView4 * vec4(viewDir, 1.0);// 
		viewDir = normalize(tempVd.xyz);

        modTexCoords = ParallaxMapping(textCoord,  viewDir);
        if(modTexCoords.x > 1.0 || modTexCoords.y > 1.0 || modTexCoords.x < 0.0 || modTexCoords.y < 0.0)
            discard;
        normal = texture2D(normalMap, modTexCoords).rgb;
        // transform normal vector to range [-1,1]
        normal = normalize((normal * 2.0 - 1.0));
        vec4 temp4 = transform * vec4(normal, 0.0);
        normal = normalize(temp4.xyz);
    } else {
        normal = theNormal;
    }

    vec4 colo4 = texture2D(diffuseMap, modTexCoords);
    vec3 lightRes = Illuminate(lightProps, lightColour, colo4,
                               normal, pointPos);
                               
    gl_FragColor = vec4(lightRes, colo4.a);
}
