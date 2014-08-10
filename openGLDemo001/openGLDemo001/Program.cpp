#include "Program.h"

Program::Program( )
{
	programIndex = 0;
}

bool Program::Init()
{
	programIndex = glCreateProgram();
	return programIndex != 0;
}

void Program::AttachShader(Shader shader)
{
	glAttachShader(programIndex,shader.ShaderIndex());
}