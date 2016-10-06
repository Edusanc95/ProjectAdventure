#ifndef Player_H
#define Player_H

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

#include <Importer.h>
#include <Scene.h>
#include <GameObject.h>

using namespace std;
using namespace Ogre;

class Player{
public:
    Player(string name, char locationPath[100], OgreBulletDynamics::DynamicsWorld * world, int MaxTimeWithoutGettingHit);
    ~Player();
    string getName();
    Ogre::SceneNode* const getNode();
    void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);

    void move(bool front, bool back, bool left, bool right, bool firstPerson);
    void jump(Ogre::Vector3 cameraD);
    void rotate();
    void rotateCamera(double x);
    Ogre::Vector3 getCameraDirection();

    btVector3 QmV3(const btVector3 & v, const btQuaternion & q);

    void addToInventory(GameObject* object);
    bool useObject(string objectName); // Used to use keys and health potions
    						     // It deletes an object of the same name as "objectName" and returns true if we found it.
    							 // Otherwise it returns false.

    //Function used to work with animations
    void addTime(float time);

    //Function used to take damage
    bool takeDamage(int damage);

    int getHealth();

    //Functions used to calculate if the player got damaged recently
    void addTimeWithoutGettingHit(float time);

    // Used to get the damage of the equipped weapon
    double getDamageEquippedWeapon();

private:
    // Player variables
    string _name;
    int _health;
    float _timeWithoutGettingHit;
    int _maxTimeWithoutGettingHit;
    Ogre::SceneNode* _node;
    int _iteratorObjectEquipped;
    Ogre::Camera* _playerCamera;

    // Physics variables
    Ogre::AnimationState* _animationState;

    btVector3 _frontSpeed, _sideSpeed;
    Ogre::Vector3 _jumpSpeed;
    std::deque <OgreBulletDynamics::RigidBody *>         _bodies;
    std::deque <OgreBulletCollisions::CollisionShape *>  _shapes;
    btScalar _angleY;
    btScalar _sensibility; // from 1 to 200 as %.

    // Really often used variables
    btTransform _xform;
    btVector3 _walkDirection;

    OgreBulletDynamics::RigidBody *_rigidBody;

    // Inventory
    std::vector<GameObject*> _inventory;
};
#endif
