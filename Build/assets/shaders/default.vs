#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out    vec3 FragPos;
out    vec2 TexCoords;
out    vec3 TangentLightsPos[9];
out    vec3 TangentViewPos;
out    vec3 TangentFragPos;
out    vec3 Normal;
out    vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 P_lightSpace;
uniform int numLights;
uniform vec3 viewPos;
uniform vec3 lightsPos[9];

void main()
{
	
    FragPos = vec3(model * vec4(aPos, 1.0));   
    TexCoords = aTexCoords;
    
    mat4 m = model;
    vec3 T = normalize(mat3(m) * aTangent);
    vec3 B = normalize(mat3(m) * aBitangent);
    vec3 N = normalize(mat3(m) * aNormal);
    //T = normalize(T - dot(T, N) * N);
    //vec3 B = cross(N, T);
    Normal=N;
    mat3 TBN = mat3(T, B, N); 
    for(int i=0;i<numLights;i++){   
    	TangentLightsPos[i] = TBN * lightsPos[i];
    }
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
    FragPosLightSpace = P_lightSpace * vec4(FragPos, 1.0);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}