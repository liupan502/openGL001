#include "RenderObject.h"
void RenderObject::Render()
{
	;
}

void RenderObject::Roate(float angle,float x_axis,float y_axis,float z_axis)
{
	mat4 roateMatrix = glm::rotate(angle,x_axis,y_axis,z_axis);
	this->transformMatrix = roateMatrix*this->transformMatrix;
}

void RenderObject::Scale(vec3 scaleParam)
{
	mat4 scaleMatrix = glm::scale(scaleParam);
	this->transformMatrix = scaleMatrix*this->transformMatrix;
}

void RenderObject::Translate(vec3 offset)
{
	mat4 translationMatrix = glm::translate(offset);
	this->transformMatrix = translationMatrix*this->transformMatrix;
}