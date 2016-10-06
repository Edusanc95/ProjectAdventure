/*
 * Trap.cpp
 *
 *  Created on: Jun 5, 2016
 *      Author: edu
 */

#include "Trap.h"

Trap::Trap(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world, double damage){
	_name = name;
	_damage = damage;
	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");

	// LOADING the Trap

	Entity *entity2 = sceneMgr->createEntity("Trap.mesh");
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

	cout << "Trap: "<< _name << " created" << endl;
}

Trap::~Trap(){
	cout << "Trap: "<< _name << " deleted" << endl;
}

string Trap::getName(){
	return _name;
}

double Trap::getDamage(){
	return _damage;
}

