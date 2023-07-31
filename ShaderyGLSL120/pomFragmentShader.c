#version 120

//varying vec3 FragPos;
varying vec2 TexCoords;
varying vec3 TangentLightPos;
varying vec3 TangentViewPos;
varying vec3 TangentFragPos;
varying vec3 theNormal; /*experiment*/
varying vec3 pointPos; /*experiment*/

uniform vec4 lightProps; // Position in View space, and intensity
uniform vec3 lightColour;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform int pomEnabled;

uniform float heightScale = 0.1;

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
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture2D(depthMap, currentTexCoords).r;

    int count = 1;
//
    while(currentLayerDepth < currentDepthMapValue || count < 25) {
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

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
//    return texCoords;
    return finalTexCoords;
}
vec3 Illuminate(vec4 LiProps, vec3 LiColour, vec4 PColour,
                vec3 PNormal, vec3 PPos);//experim

void main()
{
    vec4 colo4;
    if(pomEnabled == 0) {

        colo4 = texture2D(diffuseMap, TexCoords);
        vec3 lightRes = Illuminate(lightProps, lightColour, colo4,
                                   theNormal, pointPos);
        gl_FragColor = colo4;
    } else {

//        gl_FragColor = vec4(0.3, 0.2, 0.84, 0.5);
        vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
        vec2 texCoords = TexCoords;

//        texCoords = ParallaxMapping(TexCoords,  viewDir);
        texCoords = TexCoords;
        if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
            discard;

        // obtain normal from normal map
        vec3 normal = texture2D(normalMap, texCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);

        // get diffuse color
        vec3 color = texture2D(diffuseMap, texCoords).rgb;
        // ambient
        vec3 ambient = 0.1 * color;
        // diffuse
        vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * color;
        // specular
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

        vec3 specular = vec3(0.2) * spec;
//    FragColor = vec4(ambient + diffuse + specular, 1.0);330 version
        gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
    }

}
