#include "CameraBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

CameraBehaviour::CameraBehaviour(float pHorizontalSpeed, float pVerticalSpeed, GameObject* followPoint, glm::vec3 pOffset) : AbstractBehaviour(), _horizontalSpeed(pHorizontalSpeed), _verticalSpeed(pVerticalSpeed), _followPoint(followPoint), _offset(pOffset)
{
	_rMatrix = glm::mat4(1.f);
}

CameraBehaviour::~CameraBehaviour()
{
}

void CameraBehaviour::update(float pStep)
{
	//follow();
	orbit(pStep);
}

void CameraBehaviour::follow()
{
	
}

void CameraBehaviour::orbit(float pStep)
{
	glm::mat4 followPoint = (_followPoint->getTransform());
	glm::vec3 offset = (_offset);

	float verticalSpeed = 0.0f; //default if no keys
	float horizontalSpeed = 0.0f;

	glm::mat4 cameraMatrix = _owner->getTransform();
	glm::vec3 cameraPosition = glm::vec3(cameraMatrix[3]);
	glm::mat4 tempfollowPoint = cameraMatrix;

	//make rotation matrix
	glm::mat4 frameRotationMatrix(1.f);
	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			verticalSpeed = -_verticalSpeed;
			frameRotationMatrix = glm::rotate(0.1f * verticalSpeed * pStep, glm::vec3(1, 0, 0));
			_rMatrix *= frameRotationMatrix;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			verticalSpeed = _verticalSpeed;
			frameRotationMatrix = glm::rotate(0.1f * verticalSpeed * pStep, glm::vec3(1, 0, 0));
			_rMatrix *= frameRotationMatrix;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			horizontalSpeed = _horizontalSpeed;
			glm::mat4 tempMat(_rMatrix);
			glm::mat4 tempInverseMat = glm::inverse(_rMatrix);
			tempMat *= tempInverseMat;
			frameRotationMatrix = glm::rotate(0.1f * horizontalSpeed * pStep, (glm::vec3(0, 1, 0)));
			tempMat *= frameRotationMatrix;
			tempMat *= _rMatrix;
			_rMatrix = tempMat;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			horizontalSpeed = -_horizontalSpeed;
			glm::mat4 tempMat(_rMatrix);
			glm::mat4 tempInverseMat = glm::inverse(_rMatrix);
			tempMat *= tempInverseMat;
			frameRotationMatrix = glm::rotate(0.1f * horizontalSpeed * pStep, glm::vec3(0, 1, 0));
			tempMat *= frameRotationMatrix;
			tempMat *= _rMatrix;
			_rMatrix = tempMat;
		}
		// reset offset for following 
	}

	
	glm::mat4 tempMat(_rMatrix);
	tempMat = glm::translate(tempMat, offset);
	cameraMatrix = followPoint * tempMat;

	//translate the object in its own local space
	//_owner->translate(glm::vec3(0.0f, 0.0f, moveSpeed * pStep));

	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, then we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);

	
	/*
	
	cameraMatrix[3] = glm::vec4(0, 0, 0, 4);
	cameraMatrix = glm::rotate(-0.01f, glm::vec3(0, 1, 0)) * cameraMatrix;
	cameraMatrix[3] = glm::vec4(cameraPosition, 1);
	
	*/

	_owner->setTransform(cameraMatrix);  //times vertex
	//std::cout << (cameraMatrix[3]);
	//std::cout << (followPoint[3]);
	//rotate the object in its own local space
	//_owner->rotate(glm::radians(turnSpeed * pStep), glm::vec3(0.0f, 1.0f, 0.0f));

	//NOTE:
	//The reason the above happens in the local space of the object and not in the world space
	//is that we use the _owner->translate / rotate shortcuts which in turn call glm::rotate / glm::translate
	//The methods multiply the current transform with the translation/rotation matrix from left to right
	//meaning everything happens in local space.
}
