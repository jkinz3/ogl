#ifndef INPUT_H
#define INPUT_H

void computeMatricesFromInputs();
glm::mat4 GetViewMatrix();
glm::mat4 GetProjectionMatrix();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


#endif

