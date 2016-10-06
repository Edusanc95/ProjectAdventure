/*
 * BallBullet.cpp
 *
 *  Created on: Jun 3, 2016
 *      Author: edu
 */

#include "BallBullet.h"

BallBullet::BallBullet(string name, string mesh, Ogre::Vector3 location, Ogre::Vector3 direction, OgreBulletDynamics::DynamicsWorld * world, double timeToExpire, double damage, bool firstPerson){
	_name = name;
	_damage = damage;
	_timeToExpire = timeToExpire;

	Ogre::Vector3 pos;
	if(firstPerson == false){
		pos = location+(Quaternion(Degree(-90), Vector3::UNIT_Y)*direction)+Vector3(0,1,0);
	}else{
		pos = location+direction+Vector3(0,1,0);
	}

	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");

	// LOADING the BallBullet

	Entity *entity2 = sceneMgr->createEntity(mesh);
	SceneNode *node2 = sceneMgr->createSceneNode(_name);
	node2->attachObject(entity2);
	node2->setPosition(pos);
	sceneMgr->getRootSceneNode()->addChild(node2);

	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter2 = new
	OgreBulletCollisions::StaticMeshToShapeConverter(entity2);

	OgreBulletCollisions::CollisionShape *Trimesh2;
	Trimesh2 = new OgreBulletCollisions::SphereCollisionShape(0.75);

	OgreBulletDynamics::RigidBody *rigidObject2 = new
	OgreBulletDynamics::RigidBody(_name, world);
	rigidObject2->setShape(node2, Trimesh2, 0.5, 0.5, 0.03, pos,
					 Quaternion::IDENTITY);

	_node = node2;

	//rigidObject2->getBulletRigidBody()->setAngularFactor(0.0);
	rigidObject2->enableActiveState();

	if(firstPerson == false){
		rigidObject2->applyImpulse((Quaternion(Degree(-90), Vector3::UNIT_Y)* direction)*0.5, rigidObject2->getCenterOfMassPosition());
	}else{
		rigidObject2->applyImpulse(direction*0.5, rigidObject2->getCenterOfMassPosition());
	}

	cout << "BallBullet: "<< _name << " created" << endl;
}

BallBullet::~BallBullet(){
	cout << "BallBullet: "<< _name << " deleted" << endl;
}
bool BallBullet::timeLeft(double delta){

	_timeToExpire = _timeToExpire - delta;
	if(_timeToExpire > 0){
		return false; // If the bullet has more time to exist, return false
	}else{
		return true; // If the useful life is ended, return true
	}
}

string BallBullet::getName(){
	return _name;
}

double BallBullet::getDamage(){
	return _damage;
}
