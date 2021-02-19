#version 330 core
out vec4  fColor;

#import res/mapping.glsl

in vec3 _ambientColor;
in float _ambient;
in vec3 _normal;
in vec3 _fragPos;
in vec3 _localPos;
in vec3 _localNormal;
in vec3 _cameraPosition;
in vec2 _texCoords;
in vec3 _lightColor;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D rough;
uniform sampler2D height;
uniform sampler2D ambient;

uniform bool useTextures = true;
uniform bool hasTexCoords = false;

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

    fColor = vec4(_lightColor,1.0f)*textureCol;
}
