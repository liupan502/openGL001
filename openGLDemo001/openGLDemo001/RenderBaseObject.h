#ifndef RENDER_BASE_OBJECT
#define RENDER_BASE_OBJECT
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/swizzle.hpp>

using namespace glm;
//using glm::mat4;
//using glm::vec3;
//using glm::vec4;
//using glm::mat3;

class RenderBaseObject{
public:
	virtual void Render() = 0;
	virtual void Roate(float angle,float x_axis,float y_axis,float z_axis) = 0;
	virtual void Translate(vec3 offset) = 0;
	virtual void Scale(vec3 scaleParam) = 0;
	mat4 transformMatrix;
};
#endif