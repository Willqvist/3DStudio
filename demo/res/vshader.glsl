#version 330 core

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
uniform mat4 ModelMatrix = mat4(1);
uniform mat4 ViewMatrix = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);
uniform int time;
uniform float ambientStrength;
uniform vec3 ambientLightColor;
uniform vec3 cameraPosition;

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
}
