#version 430 core

in vec2 vPosition;

uniform float depth;
uniform mat4 viewProjectionMatrix;

void main()
{
	gl_Position = viewProjectionMatrix * vec4( vPosition.x, vPosition.y, depth, 1.0f );
}

