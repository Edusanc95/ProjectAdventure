/*
 * Exit.h
 *
 *  Created on: Jun 5, 2016
 *      Author: edu
 */

#ifndef INCLUDE_EXIT_H_
#define INCLUDE_EXIT_H_

#include "GameActor.h"

class Exit : public GameActor {
public:
    Exit(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world);
    ~Exit();

    string getName();
private:

};


#endif /* INCLUDE_EXIT_H_ */
