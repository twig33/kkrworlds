#include <input.h>
input myinput;

input* Input(){
	return &myinput;	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)
		return;
    int mode = (action == GLFW_PRESS);
	switch (key){
		case GLFW_KEY_W:
			Input()->w = mode;
			return;
		case GLFW_KEY_A:
			Input()->a = mode;
			return;
		case GLFW_KEY_S:
			Input()->s = mode;
			return;
		case GLFW_KEY_D:
			Input()->d = mode;
			return;
		case GLFW_KEY_UP:
			Input()->up = mode;
			return;
		case GLFW_KEY_DOWN:
			Input()->down = mode;
			return;
		case GLFW_KEY_LEFT:
			Input()->left = mode;
			return;
		case GLFW_KEY_RIGHT:
			Input()->right = mode;
			return;
	}
	
}
int input::YAxis(int mode)
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
int input::XAxis(int mode)
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