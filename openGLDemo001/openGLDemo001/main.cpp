
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
using namespace glm;
using glm::mat4;
using glm::vec3;
using glm::mat3;

GLuint vaoHandle;
GLuint faceNum = 0;
Program program;
mat4 mvp;
mat4 modelViewMatrix;
mat4 projectionMatrix;
char* CreateShader(const char* shaderName);
const char* vertexShaderPath = "vertexShader001.txt";
const char* fragmentShaderPath = "fragment001.txt";

vec4 lightPosition(-100.0f,0.0f,-200.0f,1.0f);
vec3 Kd(0.2f,0.3f,0.5f);
vec3 Ld(1.0f,0.5f,0.25f);
string  modelFilePath = "..\\sphere\\sphere.3DS";

void CheckActiveUniforms()
{
	GLint nUniforms,maxLen;
	glGetProgramiv(program.GetIndex(),GL_ACTIVE_UNIFORM_MAX_LENGTH,&maxLen);
	glGetProgramiv(program.GetIndex(),GL_ACTIVE_UNIFORMS,&nUniforms);
	GLchar* name = (GLchar*) malloc(maxLen);
	GLint size,location;
	GLsizei written;
	GLenum type;
	cout << "Location | Name \n";
	for(int i = 0;i<nUniforms;++i)
	{
		glGetActiveUniform(program.GetIndex(),i,maxLen,&written,&size,&type,name);
		location = glGetUniformLocation(program.GetIndex(),name);
		printf("%-8d | %s\n",location,name);
	}

}
void CheckUniformLocation(GLuint location)
{
	switch((int)location)
	{
	case GL_INVALID_VALUE :
		cout << "INVALID_VALUE" << endl; break;
	case GL_INVALID_OPERATION:
		cout << "INVALID_OPERATION" << endl;break;
	default: cout << "valid location" << endl; break;
	}
}
void Render()
{
	//CheckActiveUniforms();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	mat3 normalMatrix(transpose(modelViewMatrix._inverse()));	
	GLuint locationNormalMatrix = glGetUniformLocation(program.GetIndex(),"NormalMatrix");
	CheckUniformLocation(locationNormalMatrix);
	glUniformMatrix3fv(locationNormalMatrix,1,GL_FALSE,&normalMatrix[0][0]);

	GLuint locationModelViewMatrix = glGetUniformLocation(program.GetIndex(),"ModelViewMatrix");
	CheckUniformLocation(locationModelViewMatrix);
	glUniformMatrix4fv(locationModelViewMatrix,1,GL_FALSE,&modelViewMatrix[0][0]);

	GLuint locationProjectinMatrix = glGetUniformLocation(program.GetIndex(),"ProjectionMatrix");
	CheckUniformLocation(locationProjectinMatrix);
	glUniformMatrix4fv(locationProjectinMatrix,1,GL_FALSE,&projectionMatrix[0][0]);
	
	GLuint location = glGetUniformLocation(program.GetIndex(),"MVP");
	CheckUniformLocation(location);
	glUniformMatrix4fv(location,1,GL_FALSE,&mvp[0][0]);

	GLuint locationLightPosition = glGetUniformLocation(program.GetIndex(),"LightPosition");
	CheckUniformLocation(locationLightPosition);
	glUniform4fv(locationLightPosition,1,&lightPosition[0]);

	GLuint locationKd = glGetUniformLocation(program.GetIndex(),"Kd");
	CheckUniformLocation(locationKd);
	glUniform3fv(locationKd,1,&Kd[0]);

	GLuint locationLd = glGetUniformLocation(program.GetIndex(),"Ld");
	CheckUniformLocation(locationLd);
	glUniform3fv(locationLd,1,&Ld[0]);

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

    projectionMatrix = glm::perspective(60.0f,1.0f,0.1f,500.0f);
	mat4 modelRoateMat = glm::rotate(0.0f,0.0f,1.0f,0.0f);
	mat4 camTranslationMat = glm::translate(glm::vec3(0.0f,0.0f,-200));
	modelViewMatrix = camTranslationMat*modelRoateMat;	
	mvp = projectionMatrix*modelViewMatrix;   
	glutInit(&count, arpp);
	
	glutInitWindowSize(500,500);
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

	GLuint vboHandles[2];
	glGenBuffers(2,vboHandles);	
	GLuint pointBufferIndex = 0;
	 glGenBuffers(1,&pointBufferIndex);
	 glBindBuffer(GL_ARRAY_BUFFER,pointBufferIndex);
	 glBufferData(GL_ARRAY_BUFFER,sizeof(float)*floatValueNum,vertexBuffer,GL_STATIC_DRAW);	

	glGenVertexArrays(1,&vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	

	glBindBuffer(GL_ARRAY_BUFFER,pointBufferIndex);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);

	glLinkProgram(program.GetIndex());
	glUseProgram(program.GetIndex());
	
	
	glutDisplayFunc(Render);
	glutReshapeFunc(Resize);

	glutMainLoop();
	delete vertexBuffer;
	vertexBuffer = NULL;
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
