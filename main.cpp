#define GLEW_STATIC
#define GLFW_DLL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
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

class model
{
	public:
		glm::vec3 localRot = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 globalRot = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 getMatrix()
		{
			glm::mat4 mat = glm::mat4(1.0f);
			mat = glm::rotate(mat, glm::radians(globalRot[0]), glm::vec3(1.0, 0.0, 0.0));
			mat = glm::rotate(mat, glm::radians(globalRot[1]), glm::vec3(0.0, 1.0, 0.0));
			mat = glm::rotate(mat, glm::radians(globalRot[2]), glm::vec3(0.0, 0.0, 1.0));
			mat = glm::translate(mat, position);
		 	mat = glm::rotate(mat, glm::radians(localRot[0]), glm::vec3(1.0, 0.0, 0.0));
			mat = glm::rotate(mat, glm::radians(localRot[1]), glm::vec3(0.0, 1.0, 0.0));
			mat = glm::rotate(mat, glm::radians(localRot[2]), glm::vec3(0.0, 0.0, 1.0));
			mat = glm::scale(mat, scale);
			if (mat == glm::mat4(1.0f))
				std::cout << "aaaaaaA";
			return mat;
		}
};


enum AxisModes {axisWASD, axisARROWS};

class input
{
	public:
		int w = 0;
		int a = 0;
		int s = 0;
		int d = 0;
		int up = 0;
		int down = 0;
		int left = 0;
		int right = 0;
		//fix this delenie na 2 funci dobavit eshe mode arg nu hz kak eto ne po urodski sdelat a to budet swithc v switche
		int YAxis(int mode)
		{
			int upbutton;
			int downbutton;
			switch(mode){
				case axisWASD:
					upbutton = w;
					downbutton = s;
					break;
				case axisARROWS:
					upbutton = up;
					downbutton = down;
					break;
			}
			if (upbutton == downbutton)
				return 0;
			else
				return (upbutton ? 1 : -1);
		}
		int XAxis(int mode)
		{
			int rightbutton;
			int leftbutton;
			switch(mode){
				case axisWASD:
					rightbutton = d;
					leftbutton = a;
					break;
				case axisARROWS:
					rightbutton = right;
					leftbutton = left;
					break;
			}
			if (rightbutton == leftbutton)
				return 0;
			else
				return (leftbutton ? 1 : -1);
		}
};

input Input;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)
		return;
    int mode = (action == GLFW_PRESS);
	switch (key){
		case GLFW_KEY_W:
			Input.w = mode;
			return;
		case GLFW_KEY_A:
			Input.a = mode;
			return;
		case GLFW_KEY_S:
			Input.s = mode;
			return;
		case GLFW_KEY_D:
			Input.d = mode;
			return;
		case GLFW_KEY_UP:
			Input.up = mode;
			return;
		case GLFW_KEY_DOWN:
			Input.down = mode;
			return;
		case GLFW_KEY_LEFT:
			Input.left = mode;
			return;
		case GLFW_KEY_RIGHT:
			Input.right = mode;
			return;
	}
	
}

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
	GLint transformLoc = glGetUniformLocation(defaultShader.ID, "transform");
	GLint projLoc = glGetUniformLocation(defaultShader.ID, "proj");
	GLint viewLoc = glGetUniformLocation(defaultShader.ID, "view");
	//init TEXTURE
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_DEPTH_TEST);  
	// load and generate the texture
	int width,height,nrChannels;
	unsigned char *data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	unsigned int VBO[2], VAO[2], EBO;
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO); 
	glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
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
	
	glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
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
	 
	unsigned int triangles = 200;
	model transforms[triangles];
	transforms[0].localRot = glm::vec3(0.0f, 0.0f, 45.0f);
	transforms[0].scale = glm::vec3(0.5f,0.5f,0.5f);
	transforms[0].position = glm::vec3(0.0f,0.0f, -3.5f);
	for (int i = 1; i < triangles; ++i){
		transforms[i] = transforms[i - 1];
		transforms[i].globalRot = glm::vec3(0.0f, 0.0f, transforms[i-1].globalRot.z + 10.0f);
		transforms[i].position = glm::vec3(0.0f, transforms[i-1].position.y + 0.012f, -3.5f);
	}
	float currevenz = -3.5f;
	float curroddz = -3.5f;
	float evenzadd = 0.02f;
	float oddzadd = -0.02f;
	
	//matrice...for...projection...for...shader
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)800/(float)600, 0.1f, 100.0f);
	//camera
	glm::mat4 camera = glm::mat4(1.0f);
	glm::vec3 cameraPos = glm::vec3(0.0,0.0,3.0);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
	camera = glm::lookAt(cameraPos,
						 glm::vec3(0.0,0.0,0.0),
						 glm::vec3(0.0,1.0,0.0));
	float pitch = 0.0;
	float yaw = -90;
	//camera.position = glm::vec3(0.0f,0.0f,0.0f);
	//input callbak
	glfwSetKeyCallback(window, key_callback);
	while(!glfwWindowShouldClose(window))
	{
		double timeBefore = glfwGetTime();
		glfwPollEvents();
		//camera.position.y += Input.XAxis() * 0.05;
		//camera.position.x += Input.YAxis() * 0.05;
		cameraFront.y = 0;
		cameraPos += (GLfloat)(Input.YAxis(axisWASD) * 0.2) * cameraFront;
		cameraPos += (GLfloat)(Input.XAxis(axisWASD) * 0.2 * -1) * glm::cross(glm::vec3(cameraFront.x,0.0, cameraFront.z), cameraUp);
		pitch += Input.YAxis(axisARROWS) * 0.7;
		if (pitch > 89.0)
			pitch = 89.0;
		if (pitch < -89.0)
			pitch = -89.0;
		yaw += Input.XAxis(axisARROWS) * 0.7 * -1;
		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
		camera = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera)); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (currevenz <= -3.75f || currevenz >= -3.25f){
			evenzadd *= -1;
		}
		if (curroddz <= -3.75f || curroddz >= -3.25f){
			oddzadd *= -1;
		}
		currevenz += evenzadd;
		curroddz += oddzadd;
		for (int i = 0; i < triangles; ++i){
			glm::mat4 model;
			if (i%2 == 0){
				glUniform4f(fragmentColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
				transforms[i].globalRot += glm::vec3(0.0f, 0.0f, 0.5f);
				transforms[i].localRot += glm::vec3(0.0f, 0.0f, -1.0f);
				transforms[i].position += glm::vec3(0.0f, 0.0f, evenzadd);
			} else {
				glUniform4f(fragmentColorLocation, 1.0f, 0.5f, 0.2f, 1.0f);
				transforms[i].globalRot += glm::vec3(0.0f, 0.0f, -0.5f);
				transforms[i].localRot += glm::vec3(0.0f, 0.0f, 1.0f);
				transforms[i].position += glm::vec3(0.0f, 0.0f, oddzadd);
			}
			model = transforms[i].getMatrix();
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
			//bind texture
			glBindTexture(GL_TEXTURE_2D, texture);
			//use shader
			defaultShader.use();
			//fucking draw
			glBindVertexArray(VAO[0]);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		}
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