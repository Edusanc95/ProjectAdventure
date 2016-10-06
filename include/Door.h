/*
 * Door.h
 *
 *  Created on: May 27, 2016
 *      Author: edu
 */

#ifndef INCLUDE_DOOR_H_
#define INCLUDE_DOOR_H_

#include "GameActor.h"
#include "GameObject.h"

class Door : public GameActor {
public:
    Door(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world, string opensWith);
    ~Door();

    string getName();
    string getOpensWith();
private:
    bool _open; // To know if the door is open or not
    float _iterationLength; // The length of which the chest can be opened
    string _opensWith; // Variable used to know what we do need to do to open the door
};


#endif /* INCLUDE_DOOR_H_ */
