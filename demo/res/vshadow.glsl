#version 330 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTextureCoords;

uniform mat4 ModelMatrix = mat4(1);
uniform mat4 ViewMatrix = mat4(1);
uniform mat4 ProjectionMatrix = mat4(1);

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vPosition, 1.0);
}
