#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL2.h>
using namespace std;
const char* vertex_shader =
"#version 430 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 texture;\n"
"out vec2 empty_texture;\n"
"void main()\n"
"{\n"
"empty_texture = texture;\n"
"gl_Position = vec4(position, 1);\n"
"}\0";
const char* fragment_shader =
"#version 430 core\n"
"out vec4 filled_texture;\n"
"in vec2 empty_texture;\n"
"uniform sampler2D texture1;\n" 
"void main()\n"
"{\n"
"filled_texture = texture(texture1, empty_texture);\n" 
"}\n\0";
int main() {


	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "hello", NULL, NULL);
	if (window == NULL) {
		cout << "error";

	};

	glfwMakeContextCurrent(window);
	glewInit();
	glViewport(0, 0, 800, 600);

	int vertex = glCreateShader(GL_VERTEX_SHADER);
	int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char* pointer_to_strings[1];
	pointer_to_strings[0] = vertex_shader;
	glShaderSource(vertex, 1, pointer_to_strings, 0);
	pointer_to_strings[0] = fragment_shader;
	glShaderSource(fragment, 1, pointer_to_strings, 0);
	glCompileShader(vertex);
	glCompileShader(fragment);
	int program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	float points[]
	{

		// Positions        // Texture Coords
		0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 0
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1
		-0.5f, -0.5f, 0.0f,0.0f, 0.0f, // 2
		-0.5f,  0.5f, 0.0f,0.0f, 1.0f  // 3

	};
	unsigned int merging[]
	{
		0, 1, 2,
		2, 3, 0
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(merging), merging, GL_STATIC_DRAW);
	
                 //Every time you need to bind all the information of the vertex before drawing the model, when the amount of data is large,
	         //it is very troublesome to repeat such an action.
                 //VAO can store all of these configurations in an object. 
		//Each time you draw a model, you only need to bind the VAO object.
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); 
	

	//each vertex has 5 values
	//since it starts at the starting point 0, (arrays always start at 0) we don't need to put to where it actually begins

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

         //a vertex has 5 values
	// and it starts at the float of array[3]



	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrchannel;
	unsigned char* data = SOIL_load_image("resources/murkey.jpg", &width, &height, &nrchannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 1.7f, 0.5f, 2.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glUseProgram(program);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	glfwTerminate();
	return 0;
}
