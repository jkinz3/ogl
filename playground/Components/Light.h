#include "glm/glm.hpp"

class BaseLight
{
public:

	BaseLight(glm::vec3 SpawnLocation, float SpawnLightIntensity, glm::vec3 SpawnLightColor);

	glm::vec3 GetWorldLocation();
	glm::vec3 GetLightColor();
	float GetLightIntensity();
	void SetLightIntensity(float InLightIntensity);
	void SetLightPosition(glm::vec3 InLightPosition);

private:

	glm::vec3 WorldLocation;
	float LightIntensity;
	glm::vec3 LightColor;


	
};