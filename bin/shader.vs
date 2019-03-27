#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform vec3 ourOffset;
    void main()
    {
       gl_Position = vec4(aPos.x + ourOffset.x, aPos.y + ourOffset.y, aPos.z + ourOffset.z, 1.0);
	   TexCoord = aTexCoord;
    }