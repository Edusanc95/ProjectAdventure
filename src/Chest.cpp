/*
 * Chest.cpp

 *
 *  Created on: May 26, 2016
 *      Author: edu
 */
#include "Chest.h"

Chest::Chest(string name, Ogre::Vector3 location,
		OgreBulletDynamics::DynamicsWorld * world, int typeObject) {
	_name = name;
	_open = false;
	_iterationLength = 0.5;

	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");

	// LOADING the chest

	Entity *entity2 = sceneMgr->createEntity("Chest.mesh");
	SceneNode *node2 = sceneMgr->createSceneNode(_name);
	node2->attachObject(entity2);
	node2->setPosition(location);
	sceneMgr->getRootSceneNode()->addChild(node2);

	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter2 =
			new OgreBulletCollisions::StaticMeshToShapeConverter(entity2);

	OgreBulletCollisions::TriangleMeshCollisionShape *Trimesh2 =
			trimeshConverter2->createTrimesh();

	OgreBulletDynamics::RigidBody *rigidObject2 =
			new OgreBulletDynamics::RigidBody(_name, world);
	rigidObject2->setShape(node2, Trimesh2, 0.5, 0.5, 0, location,
			Quaternion::IDENTITY);

	_node = node2;
	switch (typeObject) {
	case 1:
		_gameObject = new GameObject("Key", 10);
		break;
	case 2:
		_gameObject = new GameObject("ArcadeStick", 50);
	default:
		break;
	}

	Ogre::SceneManager::MovableObjectIterator itrr =
			sceneMgr->getMovableObjectIterator("Entity");
	while (itrr.hasMoreElements()) {
		std::cout << itrr.getNext()->getName() << std::endl;
	}

	_animationState = entity2->getAnimationState("my_animation");
	_animationState->setLoop("false");
	_animationState->setEnabled("false");
	cout << "Chest: " << _name << " created" << endl;
}

Chest::~Chest() {
	cout << "Chest: " << _name << " deleted" << endl;
}

GameObject* Chest::getGameObject() {
	return _gameObject;
}

string Chest::getName() {
	return _name;
}

void Chest::addTime(float time) {
	_animationState->addTime(time);
}

void Chest::setAnimationState(bool aux) {
	_animationState->setEnabled(aux);
}
