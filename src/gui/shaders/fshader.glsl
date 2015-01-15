#version 130

uniform vec4 color;
uniform bool use_lights;

varying vec4 normal_frag;

float light(vec4 norm)
{
    vec3 lightDir = vec3(1,1,-1);
    float ambient = 0.2;

    float dotp = dot(normalize(norm.xyz), normalize(lightDir));
    return max(mix(ambient,1,dotp), ambient);
}

void main()
{
    if (use_lights) {
        gl_FragColor = color * light(normal_frag);
    } else {
        gl_FragColor = color;
    }
}
