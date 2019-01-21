#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void Tick();

const unsigned int Scale = 20;
const int w = 28;
const int n = 28;
const float SCALE = 2.0f / n;
const float W = SCALE;
const float N = SCALE;

const unsigned int SCR_WIDTH = Scale*w;
const unsigned int SCR_HEIGHT = Scale*n;

int dir, num = 4;

struct {
	int x;
	int y;
}s[100];

class Fructs {
public:
	int x, y;

	void New() {
		x = rand() % w;
		y = rand() % n;
	}

	void DrawApple() {
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 3, 3);
	}
}m[10];

int main()
{
	for (int i = 0; i < 10; i++) {
		m[i].New();
		std::cout << m[i].x << "\t" << m[i].y << std::endl;
	}

	s[0].x = 10;
	s[0].y = 10;

	std::cout << "w = " << w << std::endl << "n = " << n << std::endl;
	std::cout << "W = " << W << std::endl << "N = " << N << std::endl;
	std::cout << "SCALE = " << SCALE << std::endl;

	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	///////////////////////////////////////////////////////////////////////

	Shader BackSquareShader("3.3.shader.vs", "3.3.shader.fs");

	float BackSquare[] = {
		// positions         // colors
		-1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.5f,   // bottom right
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.5f,   // bottom left
		-1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 0.5f,    // top 

		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.5f,   // bottom right
		 1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 0.5f,   // bottom left
		-1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 0.5f,   // top 
	};

	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");
	
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	Shader fieldShaderVerticalLines("3.3.shader.vs", "3.3.shader.fs");

	float FieldLinesVertical[] = {
		// positions         // colors
		 1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
	};

	Shader fieldShaderHorizontalLines("3.3.shader.vs", "3.3.shader.fs");

	float FieldLinesHorizontal[] = {
		// positions         // colors
		 -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		 -1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
	};

	Shader snakeShader("3.3.shader.vs", "3.3.shader.fs");

	float SnakeSquare[] = {
		// positions         // colors
		 -1.0f,        -1.0f,       0.0f,  0.0f, 1.0f, 0.0f, //bot right
		 -1.0f,        -1.0f+SCALE, 0.0f,  0.0f, 1.0f, 0.0f, //top right
		 -1.0f+SCALE,  -1.0f+SCALE, 0.0f,  0.0f, 1.0f, 0.0f, //top left

		 -1.0f+SCALE,  -1.0f,       0.0f,  0.0f, 1.0f, 0.0f, //bot left
		 -1.0f,        -1.0f,       0.0f,  0.0f, 1.0f, 0.0f, //bot right
		 -1.0f+SCALE,  -1.0f+SCALE, 0.0f,  0.0f, 1.0f, 0.0f, //top left
	};

	Shader appleShader("3.3.shader.vs", "3.3.shader.fs");

	float AppleSquare[] = {
		// positions                          // colors
		-1.0f,          -1.0f,         0.0f,   1.0f, 0.0f, 0.0f, //bot right
		-1.0f,          -1.0f + SCALE, 0.0f,   1.0f, 0.0f, 0.0f, //top right
		-1.0f + SCALE,  -1.0f + SCALE, 0.0f,   1.0f, 0.0f, 0.0f, //top left
		
		-1.0f + SCALE,  -1.0f,         0.0f,   1.0f, 0.0f, 0.0f, //bot left
		-1.0f,          -1.0f,         0.0f,   1.0f, 0.0f, 0.0f, //bot right
		-1.0f + SCALE,  -1.0f + SCALE, 0.0f,   1.0f, 0.0f, 0.0f, //top left
	};

	///////////////////////////////////////////////////////////////////////
	
	unsigned int VBO[6], VAO[6];
	glGenVertexArrays(6, VAO);
	glGenBuffers(6, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Draw Field Vertical lines
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FieldLinesVertical), FieldLinesVertical, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Draw Field Horizontal lines
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FieldLinesHorizontal), FieldLinesHorizontal, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Draw Snake
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SnakeSquare), SnakeSquare, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Draw Apple
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AppleSquare), AppleSquare, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Draw gradient background square
	glBindVertexArray(VAO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BackSquare), BackSquare, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	///////////////////////////////////////////////////////////////////////

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		BackSquareShader.use();
		glBindVertexArray(VAO[5]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 3, 3);

		ourShader.use();
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// output play field
		fieldShaderVerticalLines.use();
		glBindVertexArray(VAO[1]);
		for (int i = 0; i < n; i++) {
			fieldShaderVerticalLines.setFloat("PosY", float(i)*N);
			glDrawArrays(GL_LINE_STRIP, 0, 2);
		}

		fieldShaderHorizontalLines.use();
		glBindVertexArray(VAO[2]);
		for (int i = 0; i < w; i++) {
			fieldShaderHorizontalLines.setFloat("PosX", float(i)*W);
			glDrawArrays(GL_LINE_STRIP, 0, 2);
		}

		// output apple
		appleShader.use();
		glBindVertexArray(VAO[4]);
		for (int i = 0; i < 10; i++) {
			appleShader.setFloat("PosX", float(m[i].x)*SCALE);
			appleShader.setFloat("PosY", float(m[i].y)*SCALE);
			m[i].DrawApple();
		}

		// output snake
		snakeShader.use();
		glBindVertexArray(VAO[3]);
		for (int i = 0; i < num; i++) {
			snakeShader.setFloat("PosX", float(s[i].x)*SCALE);
			snakeShader.setFloat("PosY", float(s[i].y)*SCALE);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_TRIANGLES, 3, 3);
		}

		Tick();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);
	
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// acomplish direction
	bool direction = false;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		if (dir != 3 && direction == false) {
			dir = 0;
			direction = true;
		}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		if (dir != 1 && direction == false) {
			dir = 2;
			direction = true;
		}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		if (dir != 0 && direction == false) {
			dir = 3;
			direction = true;
		}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		if (dir != 2 && direction == false) {
			dir = 1;
			direction = true;
		}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Tick() {
	for (int i = num; i > 0; --i) {
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	//	std::cout << s[i].x << "\t" << s[i].y << std::endl;
	}
	bool direction = false;
	if (dir == 0) s[0].y += 1;
	if (dir == 1) s[0].x -= 1;
	if (dir == 2) s[0].x += 1;
	if (dir == 3) s[0].y -= 1;

	for(int i=0; i<10; i++)
		if ((s[0].x == m[i].x) && (s[0].y == m[i].y)) {
			num++;
			m[i].New();
		}

	if (s[0].x == n)  s[0].x = 0;
	if (s[0].y == w)  s[0].y = 0;
	if (s[0].x == -1) s[0].x = w-1;
	if (s[0].y == -1) s[0].y = n-1;

	for (int i = 1; i < num; i++)
		if (s[0].x == s[i].x && s[0].y == s[i].y)
			num = i;

	Sleep(50);
}