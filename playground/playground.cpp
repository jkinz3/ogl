#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
using namespace glm;

#include "common/shader.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "common/texture.hpp"
#include "input.h"
#include "common/objloader.hpp"


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	bool res = loadOBJ("monkey.obj", vertices, uvs, normals);




	glewExperimental = true;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetScrollCallback(window, scroll_callback);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

		GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	GLuint programID = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");




	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);



	GLuint MatrixID = glGetUniformLocation(programID, "MVP");


	GLuint Texture = loadBMP_custom("uvtemplate.bmp");
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw nothing, see you in tutorial 2 !
		
		glUseProgram(programID);

		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = GetProjectionMatrix();
		glm::mat4 ViewMatrix = GetViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glm::vec3 lightPos = glm::vec3(4,4,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,
			3, 
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glDisableVertexAttribArray(0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);

		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(TextureID, 0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

