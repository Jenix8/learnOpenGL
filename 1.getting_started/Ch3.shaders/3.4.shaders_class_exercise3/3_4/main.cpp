#include <glad/glad.h> // 이게 먼저
#include <GLFW/glfw3.h> 

#include "shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main() {
	// Initializing and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL); // window 객체 생성
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // window의 컨텍스트를 현재 스레드의 주 컨텍스트로 지정하겠다고 GLFW에 알림
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
		std::cout << "Failed to initialize GLAD" << '\n';
		return -1;
	}

	// build and compile our shader program
	Shader ourShader("shader.vs", "shader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,   
		 0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.7f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// render the triangle
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}