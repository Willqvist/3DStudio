struct Material {
    vec3 ambientReflection;
    vec3 diffuseReflection;
    vec3 specularReflection;
    float shininess;
};

uniform Material material = {vec3(1,1,1),vec3(1,1,1),vec3(1,1,1),450};