#include "camera.h"

void target_camera::update(float deltaTime)
{
	glm::vec3 forward = _target - _position;
	glm::vec3 side = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward);
	_up = glm::cross(forward, side);
	_up = glm::normalize(_up);
	_view = glm::lookAt(_position, _target, _up);
}

void first_person_camera::update(float deltaTime)
{
	glm::mat4 mat = glm::mat4_cast(_rotation);
	glm::vec4 vec(_translation, 1.0f);
	vec = mat * vec;
	_translation = glm::vec3(vec.x, vec.y, vec.z);
	_position += _translation;
	_translation = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec4 forward(0.0f, 0.0f, 1.0f, 1.0f);
	forward = mat * forward;
	_target = _position + glm::vec3(forward.x, forward.y, forward.z);

	glm::vec4 tempUp(0.0f, 1.0f, 0.0f, 1.0f);
	tempUp = mat * tempUp;
	_up = glm::vec3(tempUp.x, tempUp.y, tempUp.z);

	_view = glm::lookAt(_position, _target, _up);
}

void first_person_camera::rotate(float deltaYaw, float deltaPitch)
{
	_rotation = glm::rotate(_rotation, glm::degrees(deltaYaw), glm::vec3(0.0f, 1.0f, 0.0f));
	_rotation = glm::rotate(_rotation, glm::degrees(deltaPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	_rotation = glm::normalize(_rotation);
}

void first_person_camera::move(const glm::vec3& translation)
{
	_translation += translation;
}

void arc_ball_camera::update(float deltaTime)
{
	glm::mat4 mat = glm::eulerAngleYX(_rotY, _rotX);
	glm::vec4 translation(0.0f, 0.0f, _distance, 1.0f);
	translation = mat * translation;
	_position = _target + glm::vec3(translation.x, translation.y, translation.z);
	glm::vec4 tempUp(0.0f, 1.0f, 0.0f, 1.0f);
	tempUp = mat * tempUp;
	_up = glm::vec3(tempUp.x, tempUp.y, tempUp.z);
	_view = glm::lookAt(_position, _target, _up);
}

void arc_ball_camera::move(float movement)
{
	_distance = glm::clamp<float>(_distance + movement, _minDistance, _maxDistance);
}

void arc_ball_camera::rotate(float deltaX, float deltaY)
{
	_rotX += deltaX;
	_rotY = glm::clamp<float>(_rotY + deltaY, _minRotY, _maxRotY);
}

void arc_ball_camera::translate(const glm::vec3& translation)
{
	_position += translation;
}

void chase_camera::update(float deltaTime)
{
	glm::vec3 combinedRotation = _followRotation + _relativeRotation;
	glm::mat4 mat = glm::eulerAngleYXZ(combinedRotation.y, combinedRotation.x, combinedRotation.z);

	glm::vec4 vec = mat * glm::vec4(_positionOffset, 1.0f);
	glm::vec3 desiredPosition = _followPosition + glm::vec3(vec);
	_position = glm::mix(_position, desiredPosition, _springiness);

	_targetOffset = glm::vec3(mat * glm::vec4(_targetOffset, 1.0f));
	_target = _followPosition + _targetOffset;

	_up = glm::vec3(mat * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	_view = glm::lookAt(_position, _target, _up);
}

void chase_camera::move(const glm::vec3& newFollowPosition, const glm::vec3& newFollowRotation)
{
	_followPosition = newFollowPosition;
	_followRotation = newFollowRotation;
}

void chase_camera::rotate(const glm::vec3& deltaRotation)
{
	_relativeRotation += deltaRotation;
}