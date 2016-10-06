#ifndef WinState_H
#define WinState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <string.h>
#include "GameState.h"

using namespace std;
class WinState : public Ogre::Singleton<WinState>, public GameState
{
 public:
  WinState() {}

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
  static WinState& getSingleton ();
  static WinState* getSingletonPtr ();

  // Used for management of the GUI
  void winStateExit();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  bool _exitGame;
};

#endif
