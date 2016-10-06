/*
 * Trap.h
 *
 *  Created on: Jun 5, 2016
 *      Author: edu
 */

#ifndef INCLUDE_TRAP_H_
#define INCLUDE_TRAP_H_

#include "GameActor.h"

class Trap : public GameActor {
public:
    Trap(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world, double damage);
    ~Trap();

    string getName();
    double getDamage();
private:
    double _damage;
};


#endif /* INCLUDE_TRAP_H_ */
