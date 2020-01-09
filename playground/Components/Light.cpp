#include "Light.h"


BaseLight::BaseLight(glm::vec3 SpawnLocation, float SpawnLightIntensity, glm::vec3 SpawnLightColor)
{
	WorldLocation = SpawnLocation;
	LightIntensity = SpawnLightIntensity;
	LightColor = SpawnLightColor;
}

glm::vec3 BaseLight::GetWorldLocation()
{
	return WorldLocation;
}

glm::vec3 BaseLight::GetLightColor()
{
	return LightColor;
}

float BaseLight::GetLightIntensity()
{
	return LightIntensity;
}

void BaseLight::SetLightIntensity(float InLightIntensity)
{
	LightIntensity = InLightIntensity;
}

void BaseLight::SetLightPosition(glm::vec3 InLightPosition)
{
	WorldLocation = InLightPosition;
}
