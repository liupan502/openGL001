#ifndef SCENE
#define SCENE
#include "Camera.h"
#include "RenderObject.h"
class Scene : public RenderObject
{
protected:
	Camera currentCamera;
};

#endif