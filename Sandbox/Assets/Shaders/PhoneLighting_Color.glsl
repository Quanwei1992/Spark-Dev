#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_WorldPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position,1.0f);
    v_Normal = a_Normal;
    v_WorldPos = vec3(u_Model * vec4(a_Position,1.0f));
}

#type fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

in vec3 v_Normal;
in vec3 v_WorldPos;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;

    // Diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_WorldPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * u_LightColor;

    // Final
    vec3 result = (ambient + diffuse) * u_ObjectColor;
	FragColor = vec4(result,1.0);
}