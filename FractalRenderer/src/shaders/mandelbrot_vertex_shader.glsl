#version 460 core
layout(location = 0) in vec3 position;
out vec2 fragCoord;
uniform float zoomFactor;
uniform vec2 translation;

void main()
{
    gl_Position = vec4(position, 1.0);
    fragCoord = (position.xy + translation) / zoomFactor;
}
