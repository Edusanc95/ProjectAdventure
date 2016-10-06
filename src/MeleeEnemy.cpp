/*
 * MeleeEnemy.cpp
 *
 *  Created on: May 29, 2016
 *      Author: edu
 */

#include "MeleeEnemy.h"

MeleeEnemy::MeleeEnemy(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world, int health, int damage, string typeOfMesh,
		   float distanceForActivation){
	_name = name;
	_health = health;
	_damage = damage;

	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");

	// LOADING the chest

	Entity *entity2 = sceneMgr->createEntity("SphereSlimeB.mesh");
	SceneNode *node2 = sceneMgr->createSceneNode(_name);
	node2->attachObject(entity2);
	node2->setPosition(location);
	sceneMgr->getRootSceneNode()->addChild(node2);

	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter2 = new
	OgreBulletCollisions::StaticMeshToShapeConverter(entity2);

	OgreBulletCollisions::CollisionShape *Trimesh2;
	if(typeOfMesh == "trimesh"){
		Trimesh2 = trimeshConverter2->createTrimesh();
	}else if(typeOfMesh == "sphere"){
		Trimesh2 = trimeshConverter2->createSphere();
	}
	OgreBulletDynamics::RigidBody *rigidObject2 = new
	OgreBulletDynamics::RigidBody(_name, world);
	rigidObject2->setShape(node2, Trimesh2, 0.5, 0.5, 0.5, location,
					 Quaternion::IDENTITY);

	rigidObject2->getBulletRigidBody()->setAngularFactor(0.0);
	rigidObject2->enableActiveState();

	_state = 1;
	_distanceForActivation = distanceForActivation;
	_node = node2;
	_rigidBody = rigidObject2;

	_animationState = entity2->getAnimationState("Idle");
	_animationState->setEnabled("true");
	_animationState->setLoop("true");

	cout << "MeleeEnemy: "<< _name << " created" << endl;
}

MeleeEnemy::~MeleeEnemy(){
	cout << "MeleeEnemy: "<< _name << " deleted" << endl;
}

void MeleeEnemy::stateMachine(Ogre::Node* playerNode){

	//playerNode->getPosition().length();
	Ogre::Vector3 distanceVector = playerNode->getPosition() - _node->getPosition(); //this gives a vector from node1 to node2
	Ogre::Real distance = distanceVector.length(); //calculates its length
	switch(_state){
	case 1:

		if(distance < _distanceForActivation){
			cout << "State for "<< _name << " is now active." << endl;
			_state = 2;
		}else{
			// Do nothing
		}
		break;

	case 2:
		if(distance > _distanceForActivation){
			cout << "State for "<< _name << " is now idle." << endl;
			_state = 1;
		}else{
			_rigidBody->setActivationState(true);
			_rigidBody->setLinearVelocity(distanceVector);
			//_rigidBody->applyImpulse(distanceVector * Ogre::Vector3(0.05,0.05,0.05),_node->getPosition());
		}
		break;
	}
}

bool MeleeEnemy::takeDamage(int damage){
	_health = _health - damage;
	if(_health<0){
		return true;
	}else{
		return false;
	}
}

void MeleeEnemy::addTime(double long time) {
	_animationState->addTime(time);
}
