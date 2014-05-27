#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <atlstr.h>
using namespace std;

GLuint vaoHandle;

char* CreateShader(const char* shaderName);

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES,0,3);
	glutSwapBuffers();	
}

void Resize(int w,int h)
{
	glViewport(0,0,w,h);
}

int main(int count, char* arpp[])
{
	glutInit(&count, arpp);
	
	glutInitWindowSize(300,300);
	//glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
	//glutInitContextVersion (4, 0);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("test");
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glewInit();
	const GLubyte* renderName = glGetString(GL_VERSION);
	if(renderName != NULL)
		cout << renderName << endl;

	GLuint vertexShader = 0;
	// compile vertex shader
	char* shaderSource = CreateShader("vertexShader001.txt");
	if(shaderSource)
	{	
		cout << shaderSource << endl;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

	// compile fragment shader

	GLuint fragmentShader = 0;
	const char* fragmentShaderSource = CreateShader("fragment001.txt");
	if(fragmentShaderSource)
	{	
		cout << fragmentShaderSource << endl;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		if(0 == fragmentShader)
		{
			cout << "´´½¨fragmentShaderÊ§°Ü" << endl;
		}
		else
		{
			const char* shaderArray[] = {fragmentShaderSource};
			glShaderSource(fragmentShader,1,shaderArray,NULL);
			glCompileShader(fragmentShader);
			GLint result;
			glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&result);
			if(GL_FALSE == result)
			{
				cout << "fragmentShader ±àÒë³ö´í" << endl;
				GLint logLen;
				glGetShaderiv( fragmentShader, GL_INFO_LOG_LENGTH, &logLen );
				if( logLen > 0 )
				{
					char * log = (char *)malloc(logLen);
					GLsizei written;
					glGetShaderInfoLog(fragmentShader, logLen, &written, log);
					cout << log << endl;
					free(log);
				}
			}
		}
	}

	// create program
	GLuint programHandle = glCreateProgram();
	if(0 == programHandle)
	{
		cout << "create program failed" << endl;
		exit(1);
	}
	glAttachShader(programHandle,vertexShader);
	glAttachShader(programHandle,fragmentShader);
	glLinkProgram(programHandle);
	GLint status;
	glGetProgramiv(programHandle,GL_LINK_STATUS,&status);
	if(GL_FALSE == status)
	{
		cout << "link program failed" << endl;
		GLint logLen;
		glGetProgramiv(programHandle,GL_INFO_LOG_LENGTH,&logLen);
		if(logLen > 0)
		{
			char* log = (char*) malloc(sizeof(char)*logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle,logLen,&written,log);
			cout << log << endl;
			free(log);
		}
	}
	else
	{
		glUseProgram(programHandle);
	}

	// send data to shaders
	glBindAttribLocation(programHandle,0,"VertexPosition");
	glBindAttribLocation(programHandle,1,"VertexColor");

	
	float positionData[] = {
			-0.8f,-0.8f,0.0f,
			0.8f,-0.8f,0.0f,
			0.0f,0.8f,0.0f

	};

	float colorData[] = {
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f
	};

	GLuint vboHandles[2];
	glGenBuffers(2,vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];
	glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER,9*sizeof(float),positionData,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER,9*sizeof(float),colorData,GL_STATIC_DRAW);

	glGenVertexArrays(1,&vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER,colorBufferHandle);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);

	glutDisplayFunc(Render);
	glutReshapeFunc(Resize);

	glutMainLoop();

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
