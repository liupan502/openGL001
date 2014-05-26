#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

int main(int count, char* arpp[])
{
	glutInit(&count, arpp);
	
	glutInitWindowSize(300,300);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutCreateWindow("test");
	const GLubyte* renderName = glGetString(GL_EXTENSIONS);
	if(renderName != NULL)
		cout << renderName << endl;
	getchar();
}
