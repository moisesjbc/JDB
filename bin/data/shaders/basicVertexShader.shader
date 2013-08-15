#version 430 core

in vec2 vPosition;
in vec2 texCoordinates;

//uniform float depth;
uniform mat4 mvpMatrix;

out vec2 outputTexCoordinates;

void main()
{
	gl_Position = mvpMatrix * vec4( vPosition.x, vPosition.y, 0.0f, 1.0f );
	outputTexCoordinates = texCoordinates;
}

