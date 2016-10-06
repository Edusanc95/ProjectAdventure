/*
 * MeleeEnemy.h
 *
 *  Created on: May 29, 2016
 *      Author: edu
 */

#ifndef INCLUDE_MELEEENEMY_H_
#define INCLUDE_MELEEENEMY_H_

#include "GameEnemy.h"
class MeleeEnemy : public GameEnemy {

public:
	MeleeEnemy(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world, int health, int damage, string typeOfMesh,
			   float distanceForActivation);
	~MeleeEnemy();
	void stateMachine(Ogre::Node* playerNode);
	bool takeDamage(int damage);
	void addTime(double long time);
};


#endif /* INCLUDE_MELEEENEMY_H_ */
