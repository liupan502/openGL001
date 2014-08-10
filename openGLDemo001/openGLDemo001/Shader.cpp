#include "Shader.h"
#include <iostream>
#include <GL/glew.h>
#include <fstream>

Shader::Shader() 
{
	shaderIndex = 0;
}

char* Shader::LoadShaderSource(const char* shaderFilePath)
{
	char* buf = NULL;	
	ifstream input(shaderFilePath);
	if(input)
	{		
		input.seekg(0,ios_base::end);
		int size = input.tellg();
		input.seekg(0,ios_base::beg);
		buf = new char[size];
		memset(buf,0,size);
		input.read( buf,size);		
	}	
	return buf;
}

bool Shader::Init(GLenum shaderType,const char* shaderFilePath,string& errorInfo)
{	
	// load shader source
	char* shaderSource = NULL;
	if(!(shaderSource = LoadShaderSource(shaderFilePath)))
	{
		errorInfo = "load shader source failed";
		return false;
	}
	
	// create shader
	if(!(shaderIndex = glCreateShader(shaderType)))
	{
		errorInfo = "create shader failed";
		return false;
	}
	
	// compile shader
		const char* shaderArray[] = {shaderSource};
		glShaderSource(shaderIndex,1,shaderArray,NULL);
		glCompileShader(shaderIndex);
		GLint result;
		glGetShaderiv(shaderIndex,GL_COMPILE_STATUS,&result);
		if(GL_FALSE == result)
		{
			// get compile error information
			GLint logLen;
			char * log = NULL;
			glGetShaderiv( shaderIndex, GL_INFO_LOG_LENGTH, &logLen );
			if( logLen > 0 )
			{
				log = (char *)malloc(logLen);
				GLsizei written;
				glGetShaderInfoLog(shaderIndex, logLen, &written, log);				
			}

			errorInfo = "compile failed \r\n" + (log == NULL)?log:"";

			if(log)
				free(log);
			return false;
		}

	if(shaderSource)
				delete shaderSource;
	return true;
}