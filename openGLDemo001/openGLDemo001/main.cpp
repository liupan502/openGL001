#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <atlstr.h>
using namespace std;

char* CreateShader(const char* shaderName);

int main(int count, char* arpp[])
{
	glutInit(&count, arpp);
	
	glutInitWindowSize(300,300);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutCreateWindow("test");
	glewInit();
	const GLubyte* renderName = glGetString(GL_VERSION);
	if(renderName != NULL)
		cout << renderName << endl;
	char* shaderSource = CreateShader("vertexShader001.txt");
	if(shaderSource)
	{	
		cout << shaderSource << endl;
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		if(0 == vertexShader)
		{
			cout << "´´½¨vertexShaderÊ§°Ü" << endl;
		}
		else
		{
			const char* shaderArray[] = {shaderSource};
			glShaderSource(vertexShader,1,shaderArray,NULL);
			glCompileShader(vertexShader);
			GLint result;
			glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&result);
			if(GL_FALSE == result)
			{
				cout << "vertexShader ±àÒë³ö´í" << endl;
				GLint logLen;
				glGetShaderiv( vertexShader, GL_INFO_LOG_LENGTH, &logLen );
				if( logLen > 0 )
				{
					char * log = (char *)malloc(logLen);
					GLsizei written;
					glGetShaderInfoLog(vertexShader, logLen, &written, log);
					cout << log << endl;
					free(log);
				}
			}
		}
	}
	getchar();
}

char* CreateShader(const char* shaderName)
{
	char* buf = NULL;	
	ifstream input(shaderName);
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
