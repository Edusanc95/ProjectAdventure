/*
 * Chest.h
 *
 *  Created on: May 26, 2016
 *      Author: edu
 */

#ifndef INCLUDE_CHEST_H_
#define INCLUDE_CHEST_H_

#include "GameActor.h"
#include "GameObject.h"

class Chest : public GameActor {
public:
    Chest(string name, Ogre::Vector3 location, OgreBulletDynamics::DynamicsWorld * world, int typeObject);
    ~Chest();

    GameObject* getGameObject(); // This method is used to give the object of the chest to the player
    string getName();

    void addTime(float time);
    void setAnimationState(bool aux);

private:
    bool _open; // To know if the chest is open or not
    float _iterationLength; // The length of which the chest can be opened
    GameObject* _gameObject; // Object that the chest contains

    // Physics variables
    Ogre::AnimationState* _animationState;
};
#endif /* INCLUDE_CHEST_H_ */
