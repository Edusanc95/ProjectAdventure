/*
 * Door.cpp
 *
 *  Created on: May 27, 2016
 *      Author: edu
 */

#include "Door.h"

Door::Door(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world, string opensWith){
	_name = name;
	_open = false;
	_iterationLength = 0.5;

	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");

	// LOADING the chest

	Entity *entity2 = sceneMgr->createEntity("Door.mesh");
	SceneNode *node2 = sceneMgr->createSceneNode(_name);
	node2->attachObject(entity2);
	node2->setPosition(location);
	sceneMgr->getRootSceneNode()->addChild(node2);

	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter2 = new
	OgreBulletCollisions::StaticMeshToShapeConverter(entity2);

	OgreBulletCollisions::TriangleMeshCollisionShape *Trimesh2 =
	trimeshConverter2->createTrimesh();

	OgreBulletDynamics::RigidBody *rigidObject2 = new
	OgreBulletDynamics::RigidBody(_name, world);
	rigidObject2->setShape(node2, Trimesh2, 0.5, 0.5, 0, location,
					 Quaternion::IDENTITY);

	_node = node2;
	_opensWith = opensWith;

	cout << "Door: "<< _name << " created" << endl;
}

Door::~Door(){
	cout << "Door: "<< _name << " deleted" << endl;
}

string Door::getName(){
	return _name;
}

string Door::getOpensWith(){
	return _opensWith;
}




