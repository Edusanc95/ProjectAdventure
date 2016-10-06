#ifndef GameActor_H
#define GameActor_H

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

class GameActor{

protected:
	Ogre::SceneNode* _node;
	string _name;
};

#endif
