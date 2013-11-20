#version 420 core

uniform mat4 mvpMatrix;
uniform unsigned int t;
uniform vec4 dColor;

layout(location = 0) in vec2 pos0;
layout(location = 1) in vec2 vel0;
layout(location = 2) in vec4 color0;

out vec4 color;

void main()
{
	gl_Position = mvpMatrix * ( vec4( pos0.x, pos0.y, 0.0f, 1.0f ) + vec4( vel0.x * t, vel0.y * t, 0.0f, 1.0f ) );
	color = color0 + dColor * t;
}

