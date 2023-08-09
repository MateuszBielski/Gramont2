#version 120
uniform vec4 lightProps;
uniform vec3 lightColour;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform int normalEnabled;


varying vec3 theNormal;
varying vec3 pointPos;
varying vec2 textCoord;
varying vec3 TangentLightPos;
varying vec3 TangentViewPos;
varying vec3 TangentFragPos;



vec3 Illuminate(vec4 LiProps, vec3 LiColour, vec4 PColour,
                vec3 PNormal, vec3 PPos);
void main(void)
{
    if (normalEnabled == 1)
    {
        /*
         * nie sprawdza się
        vec3 normal = texture2D(normalMap, textCoord).rgb;
        // transform normal vector to range [-1,1]
        normal = normalize(normal * 2.0 - 1.0);
        vec4 colo4 = texture2D(diffuseMap, textCoord);
        vec3 lightRes = Illuminate(lightProps, lightColour, colo4,
                                   normal, TangentFragPos);
        gl_FragColor = vec4(lightRes, colo4.a);
         */
        // obtain normal from normal map in range [0,1]
        vec3 normal = texture2D(normalMap, textCoord).rgb;
        // transform normal vector to range [-1,1]
        normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
        // get diffuse color
        vec3 color = texture2D(diffuseMap, textCoord).rgb;
        // ambient
        vec3 ambient = 0.1 * color;
        // diffuse
//        vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
        vec3 lightDir = normalize(lightProps.xyz - TangentFragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * color;
        // specular
        vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        //vec3 halfwayDir = normalize(lightDir - viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

        vec3 specular = vec3(0.2) * spec;
//        gl_FragColor = vec4(ambient + specular, 1.0);
        //gl_FragColor = vec4(diffuse, 1.0);
        //gl_FragColor = vec4(diffuse + specular, 1.0);
        gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
        //gl_FragColor = vec4(normal, 0.9);
        
    }
    else {
        vec4 colo4 = texture2D(diffuseMap, textCoord);
        vec3 lightRes = Illuminate(lightProps, lightColour, colo4,
                                   theNormal, pointPos);
        gl_FragColor = vec4(lightRes, colo4.a);
    }
    
    
}
