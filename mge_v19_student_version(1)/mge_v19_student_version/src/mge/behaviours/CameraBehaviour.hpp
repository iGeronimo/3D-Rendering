#ifndef CAMERABEHAVIOUR_HPP
#define CAMERABEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"

/**
 * KeysBehaviour allows you to move an object using the keyboard in its own local space.
 * Left right turns, forward back moves.
 */
class CameraBehaviour : public AbstractBehaviour
{
public:
	//move speed is in units per second, turnspeed in degrees per second
	CameraBehaviour(float pHorizontalspeed = 10, float pVerticalspeed = 10, GameObject* followPoint = nullptr, glm::vec3 pOffset = glm::vec3(0,0,5));
	virtual ~CameraBehaviour();
	virtual void update(float pStep);
	virtual void orbit(float pStep);
	virtual void follow();

private:
	float _horizontalSpeed;
	float _verticalSpeed;
	GameObject* _followPoint;
	glm::vec3 _offset;
};

#endif // KEYSBEHAVIOUR_HPP

