#include "Camera.h"

void Camera::Render()
{
	;
}

Camera::Camera()
{
	LookAt(vec3(0.0f,0.0f,-5.0f),vec3(0.0f,0.0f,0.f),vec3(0.0f,1.0f,0.0f));
	Perspective(1.0f,60.0f,1.0f,100.0f);
}

void Camera::LookAt(vec3 cameraPisition,vec3 cameraTarget,vec3 up)
{
	transformMatrix = lookAt(cameraPisition,cameraTarget,up);	
}

void Camera::Perspective(float ratio,float FOVAngle,float near,float far)
{
	projectMatrix = glm::perspective(FOVAngle,ratio,near,far);
}

