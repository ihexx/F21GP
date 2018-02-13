#include "glPerspective.h"



glPerspective::glPerspective()
{

}


glPerspective::~glPerspective()
{
}

glm::mat4 glPerspective::mvp(const glm::mat4 & model, const glm::mat4 & camView)
{

	return Projection * camView * model;
}
