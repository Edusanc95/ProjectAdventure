/*
 * Sign.h
 *
 *  Created on: Jun 5, 2016
 *      Author: edu
 */

#ifndef INCLUDE_SIGN_H_
#define INCLUDE_SIGN_H_

#include "GameActor.h"

class Sign : public GameActor {
public:
    Sign(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world, string message);
    ~Sign();

    string getName();
    string getMessage();
private:
    string _message;
};



#endif /* INCLUDE_SIGN_H_ */
