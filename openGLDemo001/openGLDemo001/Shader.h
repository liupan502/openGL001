#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>
using namespace std;

class Shader{

public:
	Shader();

	// load shader source and compile shader source ,
	// if error happend during loading and compiling ,
	// error information will be stored in errorInfo as string	
	bool Init(GLenum shaderType,const char* shaderFilePath,string& errorInfo);
	GLuint ShaderIndex(){ return shaderIndex;}

private:
	GLuint shaderIndex;
	char* LoadShaderSource(const char* shaderFilePath );

};
#endif