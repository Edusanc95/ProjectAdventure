/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: David Vallejo Fernández    David.Vallejo@uclm.es
 *
 * Código modificado a partir de Managing Game States with OGRE
 * http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
 * Inspirado en Managing Game States in C++
 * http://gamedevgeek.com/tutorials/managing-game-states-in-c/
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *********************************************************************/

#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <memory>

#include <Importer.h>
#include <Scene.h>

#include "GameState.h"
#include "WinState.h"
#include "Player.h"
#include "Stage.h"
#include "DotSceneLoader.h"

#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"

#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

using namespace std;
using namespace Ogre;

class PlayState : public Ogre::Singleton<PlayState>, public GameState {

 public:
  PlayState () {}

  void loadLevel(char* level);
  void addingPhysics();
  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();

  void isBetter();
  int getPoints();

  // Methods used for not overcharging the memory
  void deletePhysicsObject();

  // Used for management of the GUI
  void playStateExit();

 protected:
  //Used to store the time
  double long _delta;
  double long _timeWithoutTouchingFloor;
  bool _firstPerson;
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  bool _exitGame;

  std::shared_ptr<Player> _player;
  std::shared_ptr<Stage> _stage;
  OgreBulletDynamics::DynamicsWorld * _world;
  OgreBulletCollisions::DebugDrawer * _debugDrawer;

  //Used for a smooth movement
  struct movementVariables
           {
         	  bool _moveUp;
         	  bool _moveDown;
         	  bool _moveRight;
         	  bool _moveLeft;
         	  bool _jump;
           } _movementVariables;
};

#endif
