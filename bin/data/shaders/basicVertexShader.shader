#version 430 core

in vec2 vPosition;

//uniform float depth;
uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4( vPosition.x, vPosition.y, 0.0f, 1.0f );
}

