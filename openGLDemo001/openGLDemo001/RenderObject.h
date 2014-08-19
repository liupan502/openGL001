#ifndef RENDER_OBJECT
#define RENDER_OBJECT
#include "RenderBaseObject.h"
class RenderObject:public RenderBaseObject
{
public:
	virtual void Render();
	virtual void Roate(float angle,float x_axis,float y_axis,float z_axis) ;
	virtual void Translate(vec3 offset) ;
	virtual void Scale(vec3 scaleParam) ;
};
#endif