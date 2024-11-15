#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

const int screenWidth = 1800;
const int screenHeight = 800;

const char* vertexShaderSource = R"(
#verson 330 core
layout(location = 0) in vec3 aPos;
void main() {
	gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main(){
	FragColor = vec4(0.8,0.3,0.4,1.0);
}
)";

GLfloat verticies[] =
{
		-0.5f, 0.0f, 0.0f,
		0.0f, 0.8f, 0.0f,
		0.5f, 0.0f, 0.0f
};

GLuint shaderProgram;
static void SetupShaders()
{
	// Create a vertex shader object and specify it as a vertex shader type
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach the vertex shader source code to the vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compile the vertex shader to convert the source code into machine-readable instructions
	glCompileShader(vertexShader);
	// Create a fragment shader object and specify it as a fragment shader type
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach the fragment shader source code to the fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// Compile the fragment shader to convert the source code into machine-readable instructions
	glCompileShader(fragmentShader);
	// Create a shader program to link the compiled shaders together
	shaderProgram = glCreateProgram();

	// Attach the compiled vertex shader to the shader program
	glAttachShader(shaderProgram, vertexShader);

	// Attach the compiled fragment shader to the shader program
	glAttachShader(shaderProgram, fragmentShader);
	// Link the shaders in the program, combining them into an executable for the GPU
	glLinkProgram(shaderProgram);

	// Delete the vertex shader object, as it is no longer needed after linking
	glDeleteShader(vertexShader);

	// Delete the fragment shader object, as it is no longer needed after linking
	glDeleteShader(fragmentShader);
}

GLuint VAO, VBO;
static void SetupBuffers()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_DYNAMIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Engine", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "ERROR: Window couln't open" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL())
	{
		std::cout << "ERROR: Couln't load GLAD" << std::endl;
		return -1;
	}

	SetupShaders();
	SetupBuffers();

	glViewport(0, 0, screenWidth, screenHeight);

	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}