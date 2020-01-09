#ifndef INPUT_H
#define INPUT_H

void computeMatricesFromInputs();
glm::mat4 GetViewMatrix();
glm::mat4 GetProjectionMatrix();
glm::vec3 GetCameraPosition();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float AdjustLightBrightness();


#endif

