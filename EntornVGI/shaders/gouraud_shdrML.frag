//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (EE-UAB)
//******** Entorn basic amb interfície MFC i Status Bar
//******** Enric Marti (Març 2017)
// gouraud_shdrML.frag: Fragment Program en GLSL per implementar il.luminacio de Gouraud


uniform sampler2D albedoMap;
uniform sampler2D normalMapTex;
uniform sampler2D metallicMap;
uniform sampler2D glowMap;
uniform samplerCube irradianceMap;
uniform samplerCube specularIBL0;
uniform samplerCube specularIBL1;
uniform samplerCube specularIBL2;
uniform samplerCube specularIBL3;
uniform samplerCube specularIBL4;
uniform samplerCube specularIBL5;
uniform sampler2D   brdfLUT;  
uniform bool texture;
uniform float camPosX;
uniform float camPosY;
uniform float camPosZ;
varying vec3 WorldPos;
varying vec3 normal;
vec3 lightPositions[] = vec3[4](vec3(0.0,2.0,0.0),vec3(5.0,3.0,5.0),vec3(0.0,3.0,4.0),vec3(2.0,5.0,2.0));
vec3 lightColors[] = vec3[4](4*vec3(1.0,1.0,1.0),4*vec3(1.0,0.0,1.0),4*vec3(1.0,1.0,0.0),4*vec3(0.0,1.0,1.0));
vec3 directionalLightColor = vec3(0.9,0.3,0.2);
vec3 directionalLightDirection = vec3(1.0,0.0,0.0);



const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

void main ()
{
	vec3 albedo     = texture2D(albedoMap,gl_TexCoord[0].st).rgb;
    vec3 normalmap  = texture2D(normalMapTex,gl_TexCoord[0].st).rgb*2.0-1.0;
	vec3 glow		= texture2D(glowMap,gl_TexCoord[0].st).rgb;
    float metallic  = texture2D(metallicMap, gl_TexCoord[0].st).r;
    float roughness = texture2D(metallicMap, gl_TexCoord[0].st).g;
    float ao        = texture2D(metallicMap, gl_TexCoord[0].st).b;


	vec3 camPos = vec3(camPosX,camPosY,camPosZ);
    vec3 V = normalize(camPos - WorldPos);

	vec3 Q1 = dFdx(WorldPos);
	vec3 Q2 = dFdy(WorldPos);
	vec2 st1 = dFdx(gl_TexCoord[0].st);
	vec2 st2 = dFdy(gl_TexCoord[0].st);
	
	vec3 T = normalize(Q1*st2.y - Q2*st1.y);
	vec3 B = normalize(-Q1*st2.x + Q2*st1.x);
	
	// the transpose of texture-to-eye space matrix
	mat3 TBN = mat3(T, B, normal);
	normalmap = normalize(TBN*normalmap);

    if (texture) {
	vec3 N = normalmap;
	vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i)  //Point Lights
    {
		vec3 L = normalize(lightPositions[i] - WorldPos);
		vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;
		float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
		vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
		// kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);  
		
        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    
	}
	for(int i = 0; i < 1; ++i)  //DirectionalLight
    {
		vec3 L = normalize(directionalLightDirection);
		vec3 H = normalize(V + L);
        vec3 radiance = directionalLightColor;
		float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
		vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
		// kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);  
		
        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    
	}

	vec3 R = reflect(-V, N);
	vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness); 
	vec3 kD = 1.0 - kS;
	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;

	const float INVERSE_MAX_REFLECTION_LOD = 0.166;
	vec3 prefilteredColor;
	//float value = mix(0.0,roughness,dot(N,V));
	if(roughness <= INVERSE_MAX_REFLECTION_LOD)
	{
		prefilteredColor =  texture(specularIBL0, R).rgb;   
	}
	else if(roughness <= 2.0*INVERSE_MAX_REFLECTION_LOD)
	{
		prefilteredColor =  texture(specularIBL1, R).rgb;
	}
	else if(roughness <= 3.0*INVERSE_MAX_REFLECTION_LOD)
	{
		prefilteredColor =  texture(specularIBL2, R).rgb; 
	}
	else if(roughness <= 4.0*INVERSE_MAX_REFLECTION_LOD)
	{
		prefilteredColor =  texture(specularIBL3, R).rgb;
	}
	else if(roughness <= 5.0*INVERSE_MAX_REFLECTION_LOD)
	{
		prefilteredColor =  texture(specularIBL4, R).rgb;
	}
	else
	{
		prefilteredColor =  texture(specularIBL5, R).rgb; 
	}
	
	
	
	vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 specular = prefilteredColor * (kS * envBRDF.x + envBRDF.y);
	vec3 ambient = (kD * diffuse + specular) * ao; 

    vec3 color = ambient + Lo + glow;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
	//gl_FragColor = vec4(normal.r, normal.g, normal.b, 1.0);
	//gl_FragColor = vec4(prefilteredColor.r, prefilteredColor.g, prefilteredColor.b, 1.0);

    }
    else 
	{
	gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }

}
