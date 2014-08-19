#ifndef CAMERA
#define CAMERA
#include "RenderObject.h"

class Camera:public RenderObject{
	
public:
	virtual void Render();
	Camera();
	void LookAt(vec3 cameraPosition,vec3 cameraTarget,vec3 up);
	void Perspective(float ratio,float FOVAngle,float near,float far);

protected:
	//vec4 cameraPisition;
	//vec4 cameraTarget;
	//vec3 up;
	mat4 projectMatrix;


};
#endif