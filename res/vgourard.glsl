#version 330 core

#import res/lights.glsl
#import res/material.glsl

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTextureCoords;

out vec3 _ambientColor;
out float _ambient;
out vec3 _normal;
out vec3 _fragPos;
out vec3 _localPos;
out vec3 _localNormal;
out vec3 _cameraPosition;
out vec2 _texCoords;
out vec3 _lightColor;
uniform mat4 ModelMatrix = mat4(1);
uniform mat4 ViewMatrix = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);
uniform int time;
uniform float ambientStrength;
uniform vec3 ambientLightColor;
uniform vec3 cameraPosition;

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
    vec4 pos = ModelMatrix * vec4(vPosition, 1.0);
    gl_Position =  ProjectionMatrix * ViewMatrix * pos;
    _ambientColor = ambientLightColor;
    _ambient = ambientStrength;
    _normal = mat3(transpose(inverse(ModelMatrix))) * vNormal;
    _fragPos = pos.xyz;
    _localPos = vPosition;
    _localNormal= vNormal;
    _cameraPosition = cameraPosition;
    _texCoords = vTextureCoords;

    //lights
    vec3 viewDir = normalize(_cameraPosition - _fragPos);
    vec3 ambient =  (ambientLuminance * material.ambientReflection);

    vec3 lightColor = vec3(0,0,0);//calculateDirectionalLightSurfaceColor(directionLight,_normal,viewDir);
    lightColor += calculateAdditionalLights(_fragPos, _normal, viewDir);

    _lightColor = lightColor+ambient;


}
