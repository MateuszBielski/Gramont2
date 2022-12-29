#version 150
vec3 Illuminate(in vec4 LiProps, in vec3 LiColour, in vec4 PColour,
                in vec3 PNormal, in vec3 PPos)
{
    // Ambient illumination. Hardcoded
    vec3 liAmbient = vec3(0.2, 0.2, 0.2);
    // Operations in View Space
    vec3 lightDirec = LiProps.xyz - PPos;
    float lightDist = length(lightDirec);
    // Normalize. Attention: No lightDist > 0 check
    lightDirec = lightDirec / lightDist;
    // Attenuation. Hardcoded for this sample distances
    float attenu = 260.0 / lightDist;
    attenu = attenu * attenu;
    // Lambertian diffuse illumination
    float diffuse = dot(lightDirec, PNormal);
    diffuse = max(0.0, diffuse);
    vec3 liDiffuse = LiColour * LiProps.w * diffuse * attenu;
    // Gaussian specular illumination. Harcoded values again
    // We avoid it for interior faces
    vec3 viewDir = vec3(0.0, 0.0, 1.0);
    vec3 halfDir = normalize(lightDirec + viewDir);
    float angleHalf = acos(dot(halfDir, PNormal));
    float exponent = angleHalf / 0.05;
    float specular = 0.0;
    if (diffuse > 0.0)
        specular = exp(-exponent * exponent);
    vec3 lightRes = PColour.rgb * ( liAmbient + liDiffuse );
    // Specular colour is quite similar as light colour
    lightRes += (0.2 * PColour.xyz + 0.8 * LiColour) * specular * attenu;
    lightRes = clamp(lightRes, 0.0, 1.0);
    return lightRes;
}
