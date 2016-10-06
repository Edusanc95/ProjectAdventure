/*
 * BallBullet.h
 *
 *  Created on: Jun 3, 2016
 *      Author: edu
 */

#ifndef INCLUDE_BALLBULLET_H_
#define INCLUDE_BALLBULLET_H_


#include "GameActor.h"

class BallBullet : public GameActor {
public:
    BallBullet(string name, string mesh, Ogre::Vector3 location, Ogre::Vector3 direction, OgreBulletDynamics::DynamicsWorld * world, double timeToExpire, double damage, bool firstPerson);
    ~BallBullet();

    // Method used to calculate how many time is left for the object
    bool timeLeft(double delta);
    string getName();
    double getDamage();
private:
    double _timeToExpire;
    double _damage;
};



#endif /* INCLUDE_BALLBULLET_H_ */
