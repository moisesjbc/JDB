/*** 
	Code copied from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 420 core

uniform sampler2DArray tex;
uniform int slice;

in vec2 uv;

out vec4 finalColor;

void main()
{
	finalColor = texture( tex, vec3( uv.s, 1.0f - uv.t, slice ) );
}
