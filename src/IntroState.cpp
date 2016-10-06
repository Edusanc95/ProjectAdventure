#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->createCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));
  _sceneMgr->setSkyBox(true, "titleScreen");
  _exitGame = false;
}

void
IntroState::exit()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
  _sceneMgr->destroyAllCameras();
}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;

  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    changeState(PlayState::getSingletonPtr());
  }
}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void
IntroState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
IntroState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void IntroState::introStateExit() {
	_exitGame = true;
}

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{
  assert(msSingleton);
  return *msSingleton;
}
