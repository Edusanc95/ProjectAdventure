/*
 * GameEnemy.h
 *
 *  Created on: May 29, 2016
 *      Author: edu
 */

#ifndef INCLUDE_GAMEENEMY_H_
#define INCLUDE_GAMEENEMY_H_


#include <Ogre.h>
#include <iostream>
#include <memory>

#include "DotSceneLoader.h"
#include "GameManager.h"

#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"

#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Shapes/OgreBulletCollisionsCapsuleShape.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <BulletCollision/CollisionDispatch/btGhostObject.h>


using namespace std;
using namespace Ogre;

class GameEnemy{

public:
	int getDamage(){
		return _damage;
	}

	string getName(){
		return _name;
	}

	virtual void stateMachine(Ogre::Node* playerNode) = 0;
	virtual bool takeDamage(int damage) = 0;
	virtual void addTime(double long time) = 0;
protected:
	Ogre::SceneNode* _node;
	OgreBulletDynamics::RigidBody *_rigidBody;

	string _name;
	int _health;
	int _damage;

	Ogre::AnimationState* _animationState;

	//Variables used for the state machine
	float _distanceForActivation;
	int _state; //1 idle, 2 activated
};
#endif /* INCLUDE_GAMEENEMY_H_ */
