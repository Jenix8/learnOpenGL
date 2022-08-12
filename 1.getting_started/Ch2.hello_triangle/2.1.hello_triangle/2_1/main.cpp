#include <glad/glad.h> // 이게 먼저
#include <GLFW/glfw3.h> 

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
} // 창 크기가 바뀌어도 그에 맞게 조절하는 함수
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
} // 키보드 입력을 받는 함수

// setting
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";		
const char* fragmentShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "void main()\n"
								   "{\n"
								   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "}\n\0";

int main() {
	// Initializing and configure
	glfwInit(); // GLFW 초기화
	// 버전 설정 (OpenGL 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core-profile. openGL 기능의 작은 집합에 접근할 수 있음

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
	} // GLAD 초기화


	//// build and compile our shader program
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// 1st parameter: Shader를 컴파일하기 위해 shader 객체를 받음
	// 2nd parameter: 소스 코드가 몇 개의 문자열로 되어있는지에 대한 값
	// 3rd parameter: vertex Shader의 실제 소스 코드, 4th param: NULL
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // 컴파일이 성공했는지 확인
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	// 0. 정점 배열을 OpenGL에서 사용하기 위해 버퍼에 복사
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 1. 그런 다음 vertex 속성 포인터를 설정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// OpenGL에게 vertex 데이터를 어떻게 해석해야 하는지 알려주는 함수
	// 1st parameter: 설정할 vertex 속성을 지정. layout (location = 0)으로 position vertex 속성의 위치를 지정했었음
	// 2nd parameter: vertex 속성의 크기. vec3이므로 3
	// 3rd parameter: 데이터의 타입
	// 4th parameter: 데이터를 정규화 할 것인지 지정. GL_TRUE로 하면 0과 1 사이에 있지 않은 값들이 그 사이로 매핑됨
	// 5th parameter: stride, 연이은 vertex 속성 세트들 사이의 공백을 알림 (3개의 float이 있으므로 위와 같음)
	// last one: (void*) 타입이므로 형변환 필요, 버퍼에서 데이터가 시작하는 위치의 offset
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
	
}