#version 330 core
out vec4  fColor;

#import res/lights.glsl
#import res/material.glsl
#import res/mapping.glsl

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
    float shadow = 1-getMainLighthadowValue(position,normal);
    for(int i = 0; i < lights; i++) {
        Light l = getLight(i, position);
        diffuse += lightLambertian(l.color*l.attenuiation*material.diffuseReflection,l.direction, normal);
        specular += lightSpecular(l.color*l.attenuiation*material.specularReflection,l.direction, material.shininess, normal, viewDirection);
    }
    return (diffuse + specular)*shadow;
}

void main()
{
    vec4 normalCol = vec4(0,0,0,1);
    vec4 textureCol = vec4(1,1,1,1);
    if(useTextures) {
        vec2 st;
        if(!hasTexCoords) {
            st = getSphericalSTCoordinates(sphereRad, _normal, _localPos);
        } else {
            st = _texCoords;
        }
        textureCol = texture(albedo,st);
    }

    vec3 normal = normalize(_normal+normalCol.xyz);
    vec3 viewDir = normalize(_cameraPosition - _fragPos);
    vec3 ambient =  (ambientLuminance * material.ambientReflection);

    vec3 lightColor = vec3(0,0,0);//calculateDirectionalLightSurfaceColor(directionLight,normal,viewDir);
    lightColor += calculateAdditionalLights(_fragPos, normal, viewDir);

    lightColor = (ambient+lightColor);

    fColor = vec4(1)*vec4(lightColor,1.0f)*textureCol;
}
