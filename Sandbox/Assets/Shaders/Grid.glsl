#type vertex
#version 460 core
layout(location = 0) in vec3 a_Position;

uniform mat4 u_InversedViewProjection;

out vec3 v_NearPoint;
out vec3 v_FarPoint;

vec3 unproj(vec3 point)
{
     vec4 unprojectedPoint =  u_InversedViewProjection * vec4(point, 1.0);
     return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{
    v_NearPoint = unproj(vec3(a_Position.x,a_Position.y,-1));
    v_FarPoint = unproj(vec3(a_Position.x,a_Position.y,1));
	gl_Position = vec4(a_Position,1.0);
}

#type fragment
#version 460 core
layout(location = 0) out vec4 FragColor;
in vec3 v_NearPoint;
in vec3 v_FarPoint;
uniform mat4 u_ViewProjection;

uniform float u_LineWidth;
uniform float u_CameraDistance;

vec4 grid(vec3 fragPos3D,float scale)
{
    vec2 coord = fragPos3D.xz / scale;
    vec2 derivative = fwidth(coord) * u_LineWidth;
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    return color;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = u_ViewProjection * vec4(pos.xyz, 1.0);
    return ((clip_space_pos.z / clip_space_pos.w) + 1.0) * 0.5;
}

void main()
{
    float t = -v_NearPoint.y / (v_FarPoint.y - v_NearPoint.y);
    vec3 fragPos3D = v_NearPoint + t * (v_FarPoint - v_NearPoint);
    gl_FragDepth = computeDepth(fragPos3D);

    float level =  (1 / log(10)) * log(u_CameraDistance+10.0);
    float i;
    float f = modf(level,i);
    float scale = pow(10,i - 1);
    vec4 currentGrid = grid(fragPos3D,scale);
    vec4 nextGrid = grid(fragPos3D,scale * 10);
    currentGrid.a *= (1-f);
    nextGrid.a *=f;
    FragColor = max(currentGrid,nextGrid) * float(t > 0);
}