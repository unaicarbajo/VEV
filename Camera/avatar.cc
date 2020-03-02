#include "tools.h"
#include "avatar.h"
#include "scene.h"

Avatar::Avatar(const std::string &name, Camera * cam, float radius) :
	m_name(name), m_cam(cam), m_walk(false) {
	Vector3 P = cam->getPosition();
	m_bsph = new BSphere(P, radius);
}

Avatar::~Avatar() {
	delete m_bsph;
}

bool Avatar::walkOrFly(bool walkOrFly) {
	bool walk = m_walk;
	m_walk = walkOrFly;
	return walk;
}

//
// AdvanceAvatar: advance 'step' units
//
// @@ TODO: Change function to check for collisions. If the destination of
// avatar collides with scene, do nothing.
//
// Return: true if the avatar moved, false if not.

bool Avatar::advance(float step) {

	Node *rootNode = Scene::instance()->rootNode(); // root node of scene

	if (m_walk)
		this->m_cam->walk(step);
	else
		this->m_cam->fly(step);
	
	m_bsph->setPosition(m_cam->getPosition());
	if (rootNode->checkCollision(m_bsph)){
		if (m_walk)
			this->m_cam->walk((-1.0f)*step);
		else
			this->m_cam->fly((-1.0f)*step);
		m_bsph->setPosition(m_cam->getPosition());
		return false;
	}
	return true;
}

void Avatar::leftRight(float angle) {
	if (m_walk)
		m_cam->viewYWorld(angle);
	else
		m_cam->yaw(angle);
}

void Avatar::upDown(float angle) {
	m_cam->pitch(angle);
}

void Avatar::print() const { }
