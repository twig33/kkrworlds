#include <GLFW/glfw3.h>

enum AxisModes {axisWASD, axisARROWS};
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
		int YAxis(int mode);
		int XAxis(int mode);
};
input* Input();
