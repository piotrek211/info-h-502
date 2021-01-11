#version 330 core
out vec4 FragColor;
in vec2 v_TexCoords;
in vec3 v_Position;  //model position
in vec3 v_NormalCoord;
in mat3 m_TBN;

// material parameters
uniform sampler2D texture_albedo1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_metallic1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_ao1;

// lights
uniform vec3 u_LightPositions[6];
uniform vec3 u_LightColors[6];

uniform vec3 u_ViewPos;


//uniform samplerCube depthMap;
uniform samplerCube u_DepthMap[6];
uniform int u_ShadowsEnable;

uniform float u_FarPlane;


const float PI = 3.141592;
// Constant normal incidence Fresnel factor for all dielectrics.
const vec3 Fdielectric = vec3(0.04);

const float Epsilon = 0.0001;


float shadows[6];


float ShadowCalculation(vec3 fragmentPos, int map){
    // vector between the fragment position and the light position
    vec3 fragmentToLight = fragmentPos - u_LightPositions[map];
    // by using this fragmentToLight vector, we get the closest depth in that direction by using the depth mapping.   
    float closestDepth = texture(u_DepthMap[map], fragmentToLight).r;
    //the depth is in linear range between 0 and 1 so we need to retransform it back to its original depth value.
    closestDepth *= u_FarPlane;
    // the current depth is simply the distance between the light and the fragment to draw.
    float currentDepth = length(fragmentToLight);
    // test for shadows
    float bias = 2.50; // bias to avoid shadow acne
    //if the element to draw is closer than the closest element in depth map (currentDepth < closestDepth) then there is no shadow.
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        
    return shadow;
}

vec3 getNormalFromMap() {
    vec3 tangentNormal = texture(texture_normal1, v_TexCoords).xyz * 2.0 - 1.0;   //transform to range [-1, 1]

    return normalize(m_TBN * tangentNormal);
}

//Normal Distribution Function (Trowbridge-Reitz GGX)
float NDFGGX(vec3 N, vec3 H, float roughness) {
    float alpha = roughness*roughness;
    float alphaSquared = alpha*alpha;
    float cosNH = max(dot(N, H), 0.0);
    float cosNHSquared = cosNH*cosNH;

    float nom   = alphaSquared;
    float denom = (cosNHSquared * (alphaSquared - 1.0) + 1.0);

    return alphaSquared / (PI * denom * denom);
}

float GeometryFunctionSchlickGGX(float cosNV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float denom = cosNV * (1.0 - k) + k;

    return cosNV / denom;
}

float GeometrySubSmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float cosNV = max(dot(N, V), 0.0);
    float cosNL = max(dot(N, L), 0.0);
    float ggx2 = GeometryFunctionSchlickGGX(cosNV, roughness);
    float ggx1 = GeometryFunctionSchlickGGX(cosNL, roughness);

    return ggx1 * ggx2;
}
vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Cook-Torrance specular microfacet BRDF.
vec3 CookTorranceBRDF(float D, float G, vec3 F, vec3 N, vec3 V, vec3 L) {
    return ((D * G * F) / max(Epsilon, 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)));
}


void main() {		
    vec3 albedo     = pow(texture(texture_albedo1, v_TexCoords).rgb, vec3(2.2)); //from sRGB to linear space
    //mettalic, roughness and ao are either black or white so we only take the R channel because R=G=B
    float metallic  = texture(texture_metallic1, v_TexCoords).r;
    float roughness = texture(texture_roughness1, v_TexCoords).r;
    float ao        = texture(texture_ao1, v_TexCoords).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(u_ViewPos - v_Position);

    // Fresnel reflectance at normal incidence (for metals use albedo color).
    //mix(x, y , a) performs a linear interpolation between x and y using a to weight between them. Returns: x*(1-a) + y*a
    vec3 F0 = mix(Fdielectric, albedo, metallic);

    // Rendering equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 6; i++) {
        // calculate per-light radiance
        vec3 L = normalize(u_LightPositions[i] - v_Position);
        vec3 H = normalize(V + L); //Half-vectro between V and L
        float distance = length(u_LightPositions[i] - v_Position);
        float attenuation = 1.0 / (1.0 + 0.2*distance + 0.1*distance*distance);
        vec3 Li = u_LightColors[i] * attenuation;  //Radiance

        // Cook-Torrance BRDF
        // Calculate normal distribution function for specular BRDF.
        float D = NDFGGX(N, H, roughness);   
        //Calculate geometry function for specular BRDF.
        float G   = GeometrySubSmith(N, V, L, roughness); 
        //Calculate Fresnel term for direct lighting.
        vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 specularBRDF = CookTorranceBRDF(D, G, F, N, V, L);

        // kS is equal to Fresnel
        vec3 kS = F;
        //Metals either reflect or absorb the energy, keeping the diffuse contribution at 0.
        //If not metal, then there is a diffuse scattering.
        vec3 kD = mix(vec3(1.0) - kS, vec3(0.0), metallic);
  	  
        float cosLi = max(dot(N, L), 0.0);    
        vec3 diffuseBRDF = kD * albedo / PI;
        // add to outgoing radiance Lo
        Lo += (diffuseBRDF + specularBRDF) * Li * cosLi;  // Rendering equation
    }   
    

    // ambient lighting with environment lighting
    vec3 ambient = vec3(0.03) * albedo * ao;
    float shadow = 0.0f;
    if (u_ShadowsEnable == 1){
         for (int j = 0; j < 6; j++) {
            shadows[j] = ShadowCalculation(v_Position, j);
            shadow += shadows[j];
        }  
        shadow = (shadow >= 1.0) ? 1.0 : 0.0;
    }

    //if there is a shadow, then the fragment gets only its ambient color.

    vec3 color = ambient + (1.0 - shadow)*Lo;

    // HDR tonemapping: Reinhard's Tone-Mapping Operator
    color = color / (color + vec3(1.0));  

    // gamma correction
    color = pow(color, vec3(1.0/2.2)); 
   
    FragColor = vec4(color, 1.0);

}