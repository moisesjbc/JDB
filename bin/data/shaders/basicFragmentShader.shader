/*** 
	Code copied from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 430 core

uniform sampler2D sampler;

in vec2 texCoordinates;

out vec4 finalColor;

void main()
{
	finalColor = texture( sampler, texCoordinates );
}

