/*
 * stage.h
 *
 *  Created on: Mar 26, 2016
 *      Author: edu
 */

#ifndef INCLUDE_STAGE_H_
#define INCLUDE_STAGE_H_

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
#include <Chest.h>
#include <Door.h>
#include <Player.h>
#include <GameEnemy.h>
#include <MeleeEnemy.h>
#include <BallBullet.h>
#include <Exit.h>
#include <Trap.h>
#include <Sign.h>
using namespace std;
using namespace Ogre;

class Stage{
public:
    Stage(char* level, char locationPath[100], OgreBulletDynamics::DynamicsWorld * world);
    ~Stage();

    bool DetectCollisionPlayer(std::shared_ptr<Player> playerP);
    bool DetectExit(std::shared_ptr<Player> playerP);
    bool openChest(std::shared_ptr<Player> playerP);
    bool openDoor(std::shared_ptr<Player> playerP);
    bool readSign(std::shared_ptr<Player> playerP);
    bool DetectCollisionEnemy(std::shared_ptr<Player> playerP);
    bool DetectCollisionTrap(std::shared_ptr<Player> playerP);
    void attack(std::shared_ptr<Player> playerP, bool firstPerson);
    //The logic behind this method is that we need to notify the enemies whenever the state on them needs to be changed
    //(Mainly the changes are because of the distance between the player and the enemy)
    void iterateStateGameEnemies(Ogre::Node* playerNodeP);

    void iterateCollisionEnemies(); // To see if a bullet did damage to an enemy

    void iterateBulletsTimeLeft(double delta);

    // Animation control
    void addTime(float time);

private:
    std::vector<Chest> _chests;
    std::vector<Door> _doors;
    std::vector<GameEnemy*> _GameEnemies;
    std::vector<BallBullet> _BallBullets;
    std::vector<Exit> _Exits;
    std::vector<Trap> _Traps;
    std::vector<Sign> _Signs;
    int _nChests;
    int _nDoors;
    int _nGameEnemies;
    int _nBalls;
    int _nTotalBalls;
    int _MaxBalls;
    int _nExits;
    int _nTraps;
    int _nSigns;


    char* _level;
    OgreBulletDynamics::DynamicsWorld * _world;
};



#endif /* INCLUDE_STAGE_H_ */
