/*
 * Sign.cpp
 *
 *  Created on: Jun 5, 2016
 *      Author: edu
 */

#include "Sign.h"

Sign::Sign(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world, string message){
	_name = name;
	_message = message;
	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");

	// LOADING the Sign

	Entity *entity2 = sceneMgr->createEntity("Sign.mesh");
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

	cout << "Sign: "<< _name << " created" << endl;
}

Sign::~Sign(){
	cout << "Sign: "<< _name << " deleted" << endl;
}

string Sign::getName(){
	return _name;
}

string Sign::getMessage(){
	return _message;
}


