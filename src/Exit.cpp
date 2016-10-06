/*
 * Exit.cpp
 *
 *  Created on: Jun 5, 2016
 *      Author: edu
 */

#include "Exit.h"

Exit::Exit(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world){
	_name = name;

	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");

	// LOADING the Exit

	Entity *entity2 = sceneMgr->createEntity("exit.mesh");
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

	cout << "Exit: "<< _name << " created" << endl;
}

Exit::~Exit(){
	cout << "Exit: "<< _name << " deleted" << endl;
}

string Exit::getName(){
	return _name;
}


