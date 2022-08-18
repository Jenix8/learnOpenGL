#include <glad/glad.h> // 捞霸 刚历
#include <GLFW/glfw3.h> 
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// initializing setup
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// camera setup
Camera camera(glm::vec3(1.2f, 1.3f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting 
glm::vec3 lightPos(0.0f, 0.0f, 2.0f);

int main() {
	// Initializing and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL); // window 按眉 积己
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);					  
	glfwSetScrollCallback(window, scroll_callback);						  
																		  
	// tell GLFW to capture our mouse									  
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		  

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
		std::cout << "Failed to initialize GLAD" << '\n';
		return -1;
	}

	///////// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader program

	Shader lightingShader("color.vs", "color.fs");
	Shader lightCubeShader("light_cube.vs", "light_cube.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);					

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		lightingShader.use();
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);

		// create transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		// render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// lamp obj.
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);

		// render the lamp
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float cameraSpeed = static_cast<float>(2.5f * deltaTime);	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UPWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWNWARD, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}