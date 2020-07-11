#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
class camera
{
protected:
	glm::vec3 _position;
	glm::vec3 _target;
	glm::vec3 _up;
	glm::mat4 _view;
	glm::mat4 _projection;
public:
	camera() { }
	virtual ~camera() { }
	virtual void update (float deltaTime) = 0;
	glm::vec3 getPosition() const { return _position; }
	void setPosition(const glm::vec3& value) { _position = value; }
	glm::vec3 getTarget() const { return _target; }
	void setTarget(const glm::vec3& value) { _target = value; }
	glm::vec3 getUp() const { return _up; }
	void setUp(const glm::vec3& value) { _up = value; }
	glm::mat4 getView() const { return _view; }
	glm::mat4 getProjection() const { return _projection; }
	void setProjection(float fov, float aspect, float nearPlane, float farPlane)
	{
		_projection = glm::perspective(glm::degrees(fov), aspect, nearPlane, farPlane);
	}
};

class target_camera : public camera
{
public:
	target_camera() : camera() { }
	~target_camera() { }
	void update(float deltaTime);
};

class first_person_camera : public camera
{
private:
	glm::quat _rotation;
	glm::vec3 _translation;
public:
	first_person_camera() : camera() { }
	~first_person_camera() { }
	glm::quat getRotation() const { return _rotation; }
	void setRotation(const glm::quat& value) { _rotation = value; } 
	void update(float deltaTime);
	void rotate(float deltaYaw, float deltaPitch);
	void move(const glm::vec3& translation);
};

class arc_ball_camera : public camera
{
private:
	float _rotX;
	float _rotY;
	float _minRotY;
	float _maxRotY;
	float _distance;
	float _minDistance;
	float _maxDistance;
public:
	arc_ball_camera() { }
	~arc_ball_camera() { }
	float getRotationX() const { return _rotX; }
	void setRotationX(float value) { _rotX = value; }
	float getRotationY() const { return _rotY; }
	void setRotationY(float value) { _rotY = value; }
	float getMinRotationY() const { return _minRotY; }
	void setMinRotationY(float value) { _minRotY = value; }
	float getMaxRotationY() const { return _maxRotY; }
	void setMaxRotationY(float value) { _maxRotY = value; }
	float getDistance() const { return _distance; }
	void setDistance(float value) { _distance = value; }
	float getMinDistance() const { return _minDistance; }
	void setMinDistance(float value) { _minDistance = value; }
	float getMaxDistance() const { return _maxDistance; }
	void setMaxDistance(float value) { _maxDistance = value; }
	void update(float deltaTime);
	void move(float movement);
	void rotate(float deltaX, float deltaY);
	void translate(const glm::vec3& translation);
};

class chase_camera : public camera
{
private:
	glm::vec3 _followPosition;
	glm::vec3 _followRotation;
	glm::vec3 _positionOffset;
	glm::vec3 _targetOffset;
	glm::vec3 _relativeRotation;
	float _springiness;
public:
	chase_camera() { }
	~chase_camera() { }
	glm::vec3 getFollowPosition() const { return _followPosition; }
	void setFollowPosition(const glm::vec3& value) { _followPosition = value; }
	glm::vec3 getFollowRotation() const { return _followRotation; }
	void setFollowRotation(const glm::vec3& value) { _followRotation = value; } 
	glm::vec3 getPositionOffset() const { return _positionOffset; }
	void setPositionOffset(const glm::vec3& value) { _positionOffset = value; }
	glm::vec3 getTargetOffset() const { return _targetOffset; }
	void setTargetOffset(const glm::vec3& value) { _targetOffset = value; }
	glm::vec3 getRelativeRotation() const { return _relativeRotation; }
	void setRelativeRotation(const glm::vec3& value) { _relativeRotation = value; }
	float getSpringiness() const { return _springiness; }
	void setSpringiness(float value) { _springiness = glm::clamp<float>(value, 0.0f, 1.0f); }
	void update(float deltaTime);
	void move(const glm::vec3& newFollowPosition, const glm::vec3& newFollowRotation);
	void rotate(const glm::vec3& deltaRotation);
};