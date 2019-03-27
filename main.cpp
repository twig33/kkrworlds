#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string>
#include <iostream>
#include <shader.h>
GLuint id = 0;
GLfloat vertices[] = {
// First triangle		
//coords				//texture coords
0.5f, 0.5f, 0.0f, 		1.0f, 1.0f,// Top Right
0.5f, -0.5f, 0.0f, 		1.0f, 0.0f,// Bottom Right
-0.5f, 0.5f, 0.0f, 		0.0f, 1.0f//top left

};
GLfloat vertices2[] = { // Note that we start from 0!
// Second triangle
0.5f, -0.5f, 0.0f, 		1.0f, 0.0f,// Bottom Right
-0.5f, -0.5f, 0.0f, 	0.0f, 0.0f,// Bottom Left
-0.5f, 0.5f, 0.0f, 		0.0f, 1.0f// Top Left
};
GLuint indices[] = {
	0, 1, 2
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
	
	//init TEXTURE
		std::cout << "what";
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width,height,nrChannels;
	unsigned char *data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		std::cout << "what2";
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	unsigned int VBO, VAO, VBO2, VAO2, EBO;
    glGenVertexArrays(1, &VAO); glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO); glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//vertices coord attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texture coord attrib
	//glBindTexture(GL_TEXTURE_2D, texture);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//same attribute because we can
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	long frames = 0;
	float offsetx = 0.01;
	float curroffsetx = 0.01;
	float offsety = 0.01;
	float curroffsety = 0.01;
	while(!glfwWindowShouldClose(window))
	{
		double timeBefore = glfwGetTime();
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//calc autism
		curroffsetx += offsetx;
		if (vertices[0] + curroffsetx >= 1.0f || vertices[6] + curroffsetx <= -1.0f){
			offsetx *= -1;
		}
		//setting uniforms
		glUniform3f(vertexVecLocation, curroffsetx, 0.0f, 0.0f);
		glUniform4f(fragmentColorLocation, 1.0f, 0.5f, 0.2f, 1.0f);
		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture);
		//use shader
		defaultShader.use();
		//fucking draw
        glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//calc autism
		curroffsety += offsety;
		if (vertices2[5*2+1] + curroffsety >= 1.0f || vertices2[1] + curroffsety <= -1.0f){
			offsety *= -1;
		}
		//setting uniforms
		glUniform3f(vertexVecLocation, 0.0f, curroffsety, 0.0f);
		glUniform4f(fragmentColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture);
		//use shader
		defaultShader.use();
		//fucking draw
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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