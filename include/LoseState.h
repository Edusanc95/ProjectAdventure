/*
 * LoseState.h
 *
 *  Created on: Feb 8, 2016
 *      Author: edu
 */

#ifndef INCLUDE_LOSESTATE_H_
#define INCLUDE_LOSESTATE_H_


#ifndef LoseState_H
#define LoseState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <string.h>
#include "GameState.h"

using namespace std;
class LoseState : public Ogre::Singleton<LoseState>, public GameState
{
 public:
  LoseState() {}

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
  static LoseState& getSingleton ();
  static LoseState* getSingletonPtr ();

  // Used for management of the GUI
  void LoseStateExit();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  bool _exitGame;
};

#endif



#endif /* INCLUDE_LOSESTATE_H_ */
