
#include <GL/glew.h>
#include <GL/glut.h>
#include "glm.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include <atlstr.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/swizzle.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>          
#include <assimp/postprocess.h>  

#include "Shader.h"
#include "Program.h"

using namespace std;
using glm::mat4;
using glm::vec3;

GLuint vaoHandle;
GLuint faceNum = 0;
Program program;
mat4 mvp;
char* CreateShader(const char* shaderName);
const char* vertexShaderPath = "vertexShader001.txt";
const char* fragmentShaderPath = "fragment001.txt";

string  modelFilePath = "..\\sphere\\sphere.3DS";

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	GLuint location = glGetUniformLocation(program.GetIndex(),"MVP");
	glUniformMatrix4fv(location,1,GL_FALSE,&mvp[0][0]);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES,0,faceNum*10);
	glutSwapBuffers();	
}

void Resize(int w,int h)
{
	glViewport(0,0,w,h);
}

int main(int count, char* arpp[])
{
	
		// load model file
	Assimp::Importer importer;
	const aiScene* scene  = importer.ReadFile(modelFilePath,aiProcess_CalcTangentSpace       |  
																	aiProcess_Triangulate            |        
																	aiProcess_JoinIdenticalVertices  |        
																	aiProcess_SortByPType);

	mat4 perspectiveMat = glm::perspective(60.0f,1.0f,0.1f,500.0f);
	//mat4 perspectiveMat = glm::frustum(60.0f,1.0f,0.1f,500.0f);
	//ma4 tmpMat = glm::loo
	mat4 modelRoateMat = glm::rotate(270.0f,0.0f,1.0f,0.0f);
	mat4 camTranslationMat = glm::translate(glm::vec3(0.0f,0.0f,-100));
	mat4 roate = glm::rotate(30.0f,0.0f,0.0f,1.0f);
	mvp = perspectiveMat*camTranslationMat*modelRoateMat;
    //mvp = roate;
	glutInit(&count, arpp);
	
	glutInitWindowSize(300,300);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
	
	glutCreateWindow("test");
	glClearColor(0.0f,1.0f,0.0f,1.0f);
	glewInit();	

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	 unsigned int meshIndex = scene->mRootNode->mChildren[0]->mMeshes[0];
	 aiMesh* pMesh = scene->mMeshes[meshIndex];
	 faceNum = pMesh->mNumFaces;
	 float* vertexBuffer = new float[faceNum*9];
	 int floatValueNum = 0;
	 for(int faceIndex = 0;faceIndex<faceNum;faceIndex++)
	 {
		 aiFace face = pMesh->mFaces[faceIndex];
		 int vertexNum = face.mNumIndices;
		 /*vertexBuffer[floatValueNum++] = -0.8f; 
		 vertexBuffer[floatValueNum++] = -0.8f; 
		 vertexBuffer[floatValueNum++] = 0; 	
		  vertexBuffer[floatValueNum++] = 0.8f; 
		 vertexBuffer[floatValueNum++] = -0.8f; 
		 vertexBuffer[floatValueNum++] = 0; 	
		  vertexBuffer[floatValueNum++] = 0.0f; 
		 vertexBuffer[floatValueNum++] = 0.8f; 
		 vertexBuffer[floatValueNum++] = 0; 	*/
		 for(int vertexMidIndex = vertexNum-1;vertexMidIndex>=0;vertexMidIndex--)
		 {
			 int vertexIndex = face.mIndices[vertexMidIndex];
			 aiVector3D vertex = pMesh->mVertices[vertexIndex];
			 glm::vec4 vertex4 = glm::vec4((GLfloat)vertex.x,(GLfloat)vertex.y,(GLfloat)vertex.z,1.0f);
			 glm::vec4 newVertex = camTranslationMat*vertex4;
			 vertexBuffer[floatValueNum++] = vertex.x; 
			 vertexBuffer[floatValueNum++] = vertex.y; 
			 vertexBuffer[floatValueNum++] = vertex.z;
			 if(vertex.Length() > 50.5)
			 {
				cout << vertex.x << "  " << vertex.y << "  " << vertex.z << endl;
			 }
			 //vertexBuffer[floatValueNum++] = 0.0f;//vertex.0; 		
		 }
	 }

	 
	
	Shader vertexShader,fragmentShader;
	string errorInfo;
	if(! vertexShader.Init(GL_VERTEX_SHADER,vertexShaderPath,errorInfo))
	{
		cout << errorInfo.c_str() << endl;
		return -1;
	}

	if(! fragmentShader.Init(GL_FRAGMENT_SHADER,fragmentShaderPath,errorInfo))
	{
		cout << errorInfo.c_str() << endl;
		return -1;
	}
	
	

	if(!program.Init())
	{
		cout << "create program failed" << endl;
		return -1;
	}

	program.AttachShader(vertexShader);
	program.AttachShader(fragmentShader);
	// send data to shaders
	glBindAttribLocation(program.GetIndex(),0,"VertexPosition");
	//glBindAttribLocation(programHandle,1,"VertexColor");

	GLuint vboHandles[2];
	glGenBuffers(2,vboHandles);
	//GLuint positionBufferHandle = vboHandles[0];
	//GLuint colorBufferHandle = vboHandles[1];
	GLuint pointBufferIndex = 0;
	 glGenBuffers(1,&pointBufferIndex);
	 glBindBuffer(GL_ARRAY_BUFFER,pointBufferIndex);
	 glBufferData(GL_ARRAY_BUFFER,sizeof(float)*floatValueNum,vertexBuffer,GL_STATIC_DRAW);	

	glGenVertexArrays(1,&vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER,pointBufferIndex);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);

	//glBindBuffer(GL_ARRAY_BUFFER,colorBufferHandle);
	//glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);	
	glLinkProgram(program.GetIndex());
	glUseProgram(program.GetIndex());
	/*GLint status;
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
	}*/
	
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
