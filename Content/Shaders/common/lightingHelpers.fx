#include "../common/objectProperties.fx"
#include "../common/transformBuf.fx"
#include "../common/cameraBuf.fx"
#include "../common/dirLightBuf.fx"
#include "../common/pointLightBuf.fx"
#include "../common/spotlightBuf.fx"
#include "../common/shaderGlobals.fx"
#include "../common/textureInput.fx"

float calcAttenuation(float3 attenConstants, float lightDist)
{
  return min(1.0/(attenConstants.x + 
                  lightDist * attenConstants.y + 
                  lightDist * lightDist * attenConstants.z), 1.0);
}

float calcFog(float near, float far, float camDist)
{
  return (far - camDist)/(far - near);
}

float calcDiffuse(float3 normal, 
                  float3 light)
{
  return max(dot(normal, light), 0.0);  
}

float phongSpec(float3 normal, 
                float3 light,
                float3 view, 
                float shininess)
{
  float nDotL = max(dot(normal, light), 0.0);
  float3 reflectionVector = ((2 * nDotL) * normal) - light;
  float rDotV = max(dot(reflectionVector, view), 0.0);
  return max(pow(rDotV, shininess), 0.0);
}

float blinnSpec(float3 normal, 
                float3 light, 
                float3 view, 
                float shininess)
{
  float3 halfVec = (light + view) * 0.5;
  float nDotH = dot(normal, halfVec);
  return max(pow((clamp(nDotH, 0.0, 1.0)), shininess), 0.0);
}

float calcSpotLight(float phi, 
                    float theta, 
                    float3 lightDir, 
                    float3 light, 
                    float falloff)
{
  lightDir = normalize(lightDir);
  float cosAlpha = dot(-light, lightDir);
  float numerator = clamp(cosAlpha - cos(phi), 0.0, 1.0);
  float denominator = cos(theta) - cos(phi);
  denominator = abs(denominator);
  return pow(numerator/denominator, falloff);
}

float4 sumDirLightsPhong( float4 texColor, float3 normal, float3 view)
{
  float4 finalColor = float4(0,0,0,0);
  float4 ambient;
  float4 diffuse;
  float4 specular;
  float3 light;
  for(int i = 0; i < numDirLights; ++i)
  {
    light = normalize(-dirLights[i].lightDir.xyz);

    ambient = float4(dirLights[i].ambientIntensity,1.0) * ambientColor * 
              ambientConstant;

    diffuse = float4(dirLights[i].diffuseIntensity,1.0) * diffuseColor * 
              diffuseConstant *
              calcDiffuse(normal, light);
    diffuse *= texColor;

    specular =  float4(dirLights[i].specularIntensity,1.0) * specularColor * 
                specularConstant *
                phongSpec(normal, 
                          light, 
                          view, 
                          shininess);
    finalColor += ambient + diffuse + specular;
  }
  return finalColor;
}

float4 sumDirLightsBlinn( float4 texColor, float3 normal, float3 view)
{
  float4 finalColor = float4(0,0,0,0);
  float4 ambient;
  float4 diffuse;
  float4 specular;
  float3 light;
  for(int i = 0; i < numDirLights; ++i)
  {
    light = normalize(-dirLights[i].lightDir.xyz);
    

    ambient = float4(dirLights[i].ambientIntensity,1.0) * ambientColor * 
              ambientConstant;

    diffuse = float4(dirLights[i].diffuseIntensity,1.0) * diffuseColor * 
              diffuseConstant *
              calcDiffuse(normal, light);
    diffuse *= texColor;
    specular =  float4(dirLights[i].specularIntensity,1.0) * specularColor * 
                specularConstant * 
                blinnSpec(normal, 
                          light, 
                          view, 
                          shininess);
    finalColor += ambient + diffuse + specular;
  }
  return finalColor;
}

float4 sumSpotLightsPhong( float4 texColor, float3 normal, float3 view, float3 pos)
{
  float4 finalColor = float4(0,0,0,0);
  float4 ambient;
  float4 diffuse;
  float4 specular;
  float3 light;
  float3 lightFull;
  float  lightDist;
  for(int i = 0; i < numSpotLights; ++i)
  {
    lightFull = spotLights[i].lightPos.xyz - pos;
    lightDist = length(lightFull);
    light = lightFull / lightDist;  
    
    ambient = float4(spotLights[i].ambientIntensity,1.0) * ambientColor * 
              ambientConstant;

    diffuse = float4(spotLights[i].diffuseIntensity,1.0) * diffuseColor * 
              diffuseConstant * 
              calcDiffuse(normal, light);
    diffuse *= texColor;


    specular =  float4(spotLights[i].specularIntensity,1.0) * specularColor * 
                specularConstant * 
                phongSpec(normal, light, view, shininess);
    float attenScalar = calcAttenuation(spotLights[i].attnConstants.xyz, 
                                        length(lightFull));
    float spotScalar = calcSpotLight( spotLights[i].phi, 
                                      spotLights[i].theta, 
                                      spotLights[i].lightDir.xyz, 
                                      light,
                                      spotLights[i].spotFalloff);
    finalColor += (ambient  * attenScalar) + 
                  (diffuse  * attenScalar * spotScalar) + 
                  (specular * attenScalar * spotScalar);
  }
  return finalColor;
}

float4 sumSpotLightsBlinn( float4 texColor, float3 normal, float3 view, float3 pos)
{
  float4 finalColor = float4(0,0,0,0);
  float4 ambient;
  float4 diffuse;
  float4 specular;
  float3 light;
  float3 lightFull;
  float  lightDist;
  for(int i = 0; i < numSpotLights; ++i)
  {
    lightFull = spotLights[i].lightPos.xyz - pos;
    lightDist = length(lightFull);
    light = lightFull / lightDist;  
    
    ambient = float4(spotLights[i].ambientIntensity,1.0) * ambientColor * 
              ambientConstant;

    diffuse = float4(spotLights[i].diffuseIntensity,1.0) * diffuseColor * 
              diffuseConstant *
              calcDiffuse(normal, light);
    
    diffuse *= texColor;

    specular =  float4(spotLights[i].specularIntensity,1.0) * specularColor * 
                specularConstant *
                blinnSpec(normal, 
                          light,
                          view, 
                          shininess);
    float attenScalar = calcAttenuation(spotLights[i].attnConstants.xyz, 
                                  lightDist);
    float spotScalar = calcSpotLight( spotLights[i].phi, 
                                spotLights[i].theta, 
                                spotLights[i].lightDir.xyz, 
                                light,
                                spotLights[i].spotFalloff);
    finalColor += (ambient  * attenScalar) + 
                  (diffuse  * attenScalar * spotScalar) + 
                  (specular * attenScalar * spotScalar);
  }
  return finalColor;
}

float4 sumPointLightsPhong( float4 texColor, float3 normal, float3 view, float3 pos)
{
  float4 finalColor = float4(0,0,0,0);
  float4 ambient;
  float4 diffuse;
  float4 specular;
  float3 light;
  float3 lightFull;
  float  lightDist;
  for(int i = 0; i < numPointLights; ++i)
  {
    lightFull = pointLights[i].lightPos.xyz - pos;
    lightDist = length(lightFull);
    light = lightFull / lightDist;  

    ambient = float4(pointLights[i].ambientIntensity,1.0) * ambientColor * 
              ambientConstant;

    diffuse = float4(pointLights[i].diffuseIntensity,1.0) * diffuseColor * 
              diffuseConstant *
              calcDiffuse(normal, light);
    diffuse *= texColor;


    specular =  float4(pointLights[i].specularIntensity,1.0) * specularColor * 
                specularConstant * 
                phongSpec(normal, 
                          light, 
                          view,
                          shininess);
    float attenScalar = calcAttenuation(pointLights[i].attnConstants.xyz, 
                                  length(lightFull));
    
    finalColor += (ambient  * attenScalar) + 
                  (diffuse  * attenScalar) + 
                  (specular * attenScalar);
  }
  return finalColor;
}

float4 sumPointLightsBlinn( float4 texColor, float3 normal, float3 view, float3 pos)
{
  float4 finalColor = float4(0,0,0,0);
  float4 ambient;
  float4 diffuse;
  float4 specular;
  float3 light;
  float3 lightFull;
  float  lightDist;
  for(int i = 0; i < numPointLights; ++i)
  {
    lightFull = pointLights[i].lightPos.xyz - pos;
    lightDist = length(lightFull);
    light = lightFull / lightDist;  

    ambient = float4(pointLights[i].ambientIntensity,1.0) * ambientColor * 
              ambientConstant;

    diffuse = float4(pointLights[i].diffuseIntensity,1.0) * diffuseColor * 
              diffuseConstant *
              calcDiffuse(normal, light);
    diffuse *= texColor;


    specular =  float4(pointLights[i].specularIntensity,1.0) * specularColor * 
                specularConstant * 
                blinnSpec(normal, 
                          light, 
                          view,
                          shininess);
    float attenScalar = calcAttenuation(pointLights[i].attnConstants.xyz, 
                                        lightDist);
    
    finalColor += (ambient  * attenScalar) + 
                  (diffuse  * attenScalar) + 
                  (specular * attenScalar);
  }
  return finalColor;
}