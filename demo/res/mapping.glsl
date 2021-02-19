float sphereRad = 3;
const float PI = 3.1415926538;

float f(float d,float a,float b,float c) {
    return a*d*d+b*d+c;
}

vec2 getSphericalSTCoordinates(float radius, vec3 normal, vec3 position) {
    float a = 1;
    float b = 2*dot(position, normal);
    float c = 1-radius*radius;

    float delta = b*b-4*a*c;
    float q = -0.5*(b+sign(b)*sqrt(delta));
    float d1 = q/a;
    float d2 = c/q;

    float ans = f(d1, a, b, c);
    float ans2 = f(d2, a, b, c);

    float d = max(ans, ans2);

    vec3 P = position + d*normal;
    P = (P/length(P))*radius;


    vec2 st = vec2(
    acos(P.x/radius)/PI,
    atan(P.z/P.y)/PI+0.5
    );

    return st;
}