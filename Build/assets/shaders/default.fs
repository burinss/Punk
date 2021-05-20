#version 400 core
//out vec4 FragColor;
layout(location = 0) out vec4 FragColor;

struct Light
{
	vec3 lightColor;
	vec3 position;
	float lightRadius;
	float compression;
};

in    vec3 FragPos;
in    vec2 TexCoords;
in    vec3 TangentViewPos;
in    vec3 TangentFragPos;
in    vec3 Normal;
in    vec4 FragPosLightSpace;

uniform     Light LightsArray[9];

uniform int numLights;
uniform vec3 viewPos;

uniform sampler2D shadowSampler;

uniform sampler2D diffuseTexture;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

vec2 poissonDisk[64] = vec2[]( 
	vec2( -0.04117257f, -0.1597612f ),
	vec2( 0.06731031f, -0.4353096f ),
	vec2( -0.206701f, -0.4089882f ),
	vec2( 0.1857469f, -0.2327659f ),
	vec2( -0.2757695f, -0.159873f ),
	vec2( -0.2301117f, 0.1232693f ),
	vec2( 0.05028719f, 0.1034883f ),
	vec2( 0.236303f, 0.03379251f ),
	vec2( 0.1467563f, 0.364028f ),
	vec2( 0.516759f, 0.2052845f ),
	vec2( 0.2962668f, 0.2430771f ),
	vec2( 0.3650614f, -0.1689287f ),
	vec2( 0.5764466f, -0.07092822f ),
	vec2( -0.5563748f, -0.4662297f ),
	vec2( -0.3765517f, -0.5552908f ),
	vec2( -0.4642121f, -0.157941f ),
	vec2( -0.2322291f, -0.7013807f ),
	vec2( -0.05415121f, -0.6379291f ),
	vec2( -0.7140947f, -0.6341782f ),
	vec2( -0.4819134f, -0.7250231f ),
	vec2( -0.7627537f, -0.3445934f ),
	vec2( -0.7032605f, -0.13733f ),
	vec2( 0.8593938f, 0.3171682f ),
	vec2( 0.5223953f, 0.5575764f ),
	vec2( 0.7710021f, 0.1543127f ),
	vec2( 0.6919019f, 0.4536686f ),
	vec2( 0.3192437f, 0.4512939f ),
	vec2( 0.1861187f, 0.595188f ),
	vec2( 0.6516209f, -0.3997115f ),
	vec2( 0.8065675f, -0.1330092f ),
	vec2( 0.3163648f, 0.7357415f ),
	vec2( 0.5485036f, 0.8288581f ),
	vec2( -0.2023022f, -0.9551743f ),
	vec2( 0.165668f, -0.6428169f ),
	vec2( 0.2866438f, -0.5012833f ),
	vec2( -0.5582264f, 0.2904861f ),
	vec2( -0.2522391f, 0.401359f ),
	vec2( -0.428396f, 0.1072979f ),
	vec2( -0.06261792f, 0.3012581f ),
	vec2( 0.08908027f, -0.8632499f ),
	vec2( 0.9636437f, 0.05915006f ),
	vec2( 0.8639213f, -0.309005f ),
	vec2( -0.03422072f, 0.6843638f ),
	vec2( -0.3734946f, -0.8823979f ),
	vec2( -0.3939881f, 0.6955767f ),
	vec2( -0.4499089f, 0.4563405f ),
	vec2( 0.07500362f, 0.9114207f ),
	vec2( -0.9658601f, -0.1423837f ),
	vec2( -0.7199838f, 0.4981934f ),
	vec2( -0.8982374f, 0.2422346f ),
	vec2( -0.8048639f, 0.01885651f ),
	vec2( -0.8975322f, 0.4377489f ),
	vec2( -0.7135055f, 0.1895568f ),
	vec2( 0.4507209f, -0.3764598f ),
	vec2( -0.395958f, -0.3309633f ),
	vec2( -0.6084799f, 0.02532744f ),
	vec2( -0.2037191f, 0.5817568f ),
	vec2( 0.4493394f, -0.6441184f ),
	vec2( 0.3147424f, -0.7852007f ),
	vec2( -0.5738106f, 0.6372389f ),
	vec2( 0.5161195f, -0.8321754f ),
	vec2( 0.6553722f, -0.6201068f ),
	vec2( -0.2554315f, 0.8326268f ),
	vec2( -0.5080366f, 0.8539945f )
);

float Random(vec4 co) {
  float dp = dot(co, vec4(12.9898, 78.233, 45.164, 94.673));
  return fract(sin(dp) * 43758.5453);
}

float GetBlockerDistance(vec3 shadowCoords, float lightSize) {
    int blockers = 0;
    float averageBlockerDistance = 0.0;
    float searchWidth = lightSize * (shadowCoords.z - 0.1) / viewPos.z;
    for (int i = 0; i < 16; i++) {
      float z = texture(shadowSampler, shadowCoords.xy + poissonDisk[i] * searchWidth).r;
      if (z < (shadowCoords.z - 0.005)) {
        ++blockers;
        averageBlockerDistance += z;
      }
    }

    if (blockers > 0) return averageBlockerDistance / blockers;
    else return -1;
}

float PCSSShadow(vec4 lightSpacePosition, float lightSize) {
    vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
    if (projCoords.z > 1.0)
      return 0.0;

    projCoords = projCoords * 0.5 + 0.5;

    float blockerDistance = GetBlockerDistance(projCoords, lightSize);
    if (blockerDistance == -1)
      return 0.0;

    float penumbraWidth = (projCoords.z - blockerDistance) / blockerDistance;

    float radius = penumbraWidth * lightSize * 0.1 / projCoords.z;

    // PCF
	float shadow = 0.0;
	float theta = Random(vec4(projCoords.xy, FragPos.xy));
	mat2 rotation = mat2(vec2(cos(theta), sin(theta)), vec2(-sin(theta), cos(theta)));
	float bias = max(0.05 * (1.0 - dot(Normal, FragPos - lightSpacePosition.xyz)), 0.002);  
    for (int i = 0; i < 32; i++) {
	  vec2 offset = (rotation * poissonDisk[i]) * radius;
      float z = texture(shadowSampler, projCoords.xy + offset).r;
      shadow += z < (projCoords.z - bias) ? 1.0 : 0.0;
    }
    return shadow / 32 * 0.85;
}


void main()
{    
    vec3 scatteredLight = vec3(0.0);
    vec3 reflectedLight = vec3(0.0);

    for (int i = 0; i < numLights; i++) {
    
    	vec3 lightDirection = normalize(LightsArray[i].position - FragPos);
    
    	vec3 halfVector = normalize(lightDirection + (viewPos - FragPos));

    	float attenunation = 1.0;
    
    	float lightDistance = length(lightDirection);
    	lightDirection = lightDirection / lightDistance;

    	//attenunation = 1.0 / (lightDistance * lightDistance);
    	attenunation = pow(smoothstep(LightsArray[i].lightRadius, 0, length(LightsArray[i].position - FragPos)), LightsArray[i].compression);
    	float diffuse = max(0.0, dot(Normal, lightDirection));

    	float specular = max(0.0, dot(Normal, halfVector));

    	specular = (diffuse == 0.0) ? 0.0 : pow(specular, 256);

    	scatteredLight += LightsArray[i].lightColor * diffuse * attenunation;
    	//reflectedLight += LightsArray[i].lightColor * specular * attenunation;
    }

    vec4 albedoTexData = texture(diffuseTexture, TexCoords);
    vec4 albd = vec4(pow(albedoTexData.rgb, vec3(2.2)), albedoTexData.a);

    float shadow = PCSSShadow(FragPosLightSpace, 1/LightsArray[0].lightRadius);
    
    //float shadow = CalculateShadow(FragPosLightSpace);
    //float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 color = min((albd.rgb) + ((1.0 - shadow) *  (scatteredLight + reflectedLight)), vec3(1.0));
    
    FragColor = vec4(color, albd.a);
    //FragColor = vec4(vec3(shadow), albd.a);
	// obtain normal from normal map in range [0,1]
    //vec3 normal = texture(texture_normal1, TexCoords).rgb;
    // transform normal vector to range [-1,1]
    //vec3 normal = Normal;
    //normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    //normal = normalize(normal);
	// get diffuse color
    //vec3 result = vec3(0,0,0);
    //for(int i=0;i<numLights;i++){ 
    // result+=CalcPointLight(lightsPos[i],normal,TangentLightsPos[i]);
    //}

    //FragColor = vec4(ambient + diffuse + specular, 1.0);
    //FragColor = vec4(1.0,1.0,1.0,1.0);
    //FragColor=texture(diffuseTexture, TexCoords);
    //FragColor = vec4(result*(lightColor), texture(diffuseTexture, TexCoords).a);
}
