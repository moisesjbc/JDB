#version 420 core

uniform mat4 mvpMatrix;

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec2 inputUV;

out vec2 outputUV;

void main()
{
	outputUV = inputUV;
	gl_Position = mvpMatrix * vec4( vPosition.x, vPosition.y, 0.0f, 1.0f );
}

