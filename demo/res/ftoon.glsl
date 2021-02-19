#version 330 core
out vec4  fColor;

#import res/material.glsl
#import res/mapping.glsl
#import res/lights.glsl

in vec3 _ambientColor;
in float _ambient;
in vec3 _normal;
in vec3 _fragPos;
in vec3 _localPos;
in vec3 _localNormal;
in vec3 _cameraPosition;
in vec2 _texCoords;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D rough;
uniform sampler2D height;
uniform sampler2D ambient;

uniform bool useTextures = true;
uniform bool hasTexCoords = false;

vec3 calculateAdditionalLights(vec3 position, vec3 normal, vec3 viewDirection) {
    int lights = getNumberOfLights();
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);
    vec3 rimItensity = vec3(0,0,0);
    float shadow = 1-getMainLighthadowValue(position,normal);
    for(int i = 0; i < lights; i++) {
        Light l = getLight(i, position);
        float lightIntensity = smoothstep(0.0,0.01,dot(normal,l.direction));
        diffuse += lightIntensity*l.color*l.attenuiation*material.diffuseReflection;
        specular += smoothstep(0.005,0.01,lightSpecular(l.color*l.attenuiation*material.specularReflection,l.direction, material.shininess, normal, viewDirection));
        rimItensity += pow(fresnel(viewDirection, normal),3)*l.color*lightIntensity;
    }
    return (diffuse + specular + smoothstep(0.5,0.51,rimItensity))*shadow;
}

void main()
{
    vec4 normalCol = vec4(0,0,0,1);
    vec4 textureCol = vec4(1,1,1,1);
    if(useTextures) {
        vec2 st;
        if(!hasTexCoords) {
            st = getSphericalSTCoordinates(sphereRad, _localNormal, _localPos);
        } else {
            st = _texCoords;
        }

        //normalCol = normalize(texture(normal,st));
        textureCol = texture(albedo,st);
    }

    vec3 normal = normalize(_normal+normalCol.xyz);
    vec3 viewDir = normalize(_cameraPosition - _fragPos);
    vec3 ambient =  (ambientLuminance * material.ambientReflection);

    vec3 lightColor = vec3(0,0,0);//calculateDirectionalLightSurfaceColor(directionLight,normal,viewDir);
    lightColor += calculateAdditionalLights(_fragPos, normal, viewDir);

    float shadow = calculateShadow();
    lightColor += ambient;

    fColor = vec4(lightColor,1.0f)*textureCol;
}
