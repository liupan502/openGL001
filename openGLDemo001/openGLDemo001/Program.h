#ifndef PROGRAM
#define PROGRAM
#include "Shader.h"
class Program
{
public:
	Program();
	bool Init();
	void Link();
	void AttachShader(Shader shader);
	GLuint GetIndex()
	{
		return programIndex;
	}
	//void DettachShader(Shader shader);
private:
	GLuint programIndex;
}	;	

#endif