#type vertex
#version 460 core
layout(location = 0) in vec3 a_Position;
void main()
{
	gl_Position = vec4(a_Position,1.0f);
}

#type fragment
#version 460 core
layout(location = 0) out vec4 FragColor;
uniform vec4 u_Color;
void main()
{
	FragColor = u_Color;
}