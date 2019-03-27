version 330 core
layout (location = 0) in vec3 aPos;
uniform vec3 ourOffset;
    void main()
    {
       gl_Position = vec4(aPos.x + ourOffset.x, aPos.y + ourOffset.y, aPos.z + ourOffset.z, 1.0);
    }\0