#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <shader.h>
GLuint id = 0;
GLfloat vertices[] = {
// First triangle
0.5f, 0.5f, 0.0f, // Top Right
0.5f, -0.5f, 0.0f, // Bottom Right
-0.5f, 0.5f, 0.0f, // Top Left

};
GLfloat vertices2[] = { // Note that we start from 0!
// Second triangle
0.5f, -0.5f, 0.0f, // Bottom Right
-0.5f, -0.5f, 0.0f, // Bottom Left
-0.5f, 0.5f, 0.0f // Top Left
};


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"uniform vec3 ourOffset;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x + ourOffset.x, aPos.y + ourOffset.y, aPos.z + ourOffset.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
	"uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";
static const double fpslimit = 1.0f / 60.0f;
int main(void)
{
	//glfw init and window
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "kkrworlds", nullptr,
	nullptr);
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);
	//glew init 
	glewExperimental = GL_TRUE;
	glewInit();

	//shader init
	Shader defaultShader("shader.vs","shader.frag");
	GLint vertexVecLocation = glGetUniformLocation(defaultShader.ID, "ourOffset");
	GLint fragmentColorLocation = glGetUniformLocation(defaultShader.ID, "ourColor");
	
	unsigned int VBO, VAO, VBO2, VAO2;
    glGenVertexArrays(1, &VAO); glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO); glGenBuffers(1, &VBO2);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	//same attribute because we can
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	long frames = 0;
	float offsetx = 0.01;
	float curroffsetx = 0.01;
	float offsety = 0.01;
	float curroffsety = 0.01;
	while(!glfwWindowShouldClose(window))
	{
		double timeBefore = glfwGetTime();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		defaultShader.use();
		curroffsetx += offsetx;
		if (vertices[0] + curroffsetx >= 1.0f || vertices[6] + curroffsetx <= -1.0f){
			offsetx *= -1;
		}
		glUniform3f(vertexVecLocation, curroffsetx, 0.0f, 0.0f);
		glUniform4f(fragmentColorLocation, 1.0f, 0.5f, 0.2f, 1.0f);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
	//	glDrawArrays(GL_TRIANGLES_ADJACENCY, 1, 2);
		curroffsety += offsety;
		if (vertices2[7] + curroffsety >= 1.0f || vertices2[1] + curroffsety <= -1.0f){
			offsety *= -1;
		}
		glUniform3f(vertexVecLocation, 0.0f, curroffsety, 0.0f);
		glUniform4f(fragmentColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES,0,3);
		glfwSwapBuffers(window);
		double timeAfter = glfwGetTime();
		double difference = timeAfter - timeBefore;
		if (difference < fpslimit){
			while (glfwGetTime() < timeAfter + fpslimit - difference){};
		}
		frames++;
	}
	glfwTerminate();
}