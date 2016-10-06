#include "WinState.h"
#include "PlayState.h"
#include <iostream>

using namespace std;
template<> WinState* Ogre::Singleton<WinState>::msSingleton = 0;

void WinState::enter() {
	_root = Ogre::Root::getSingletonPtr();

	// Sounds and tracks management
	//GameManager::getSingletonPtr()->changeTrack("victoryfanfare.mp3");

	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->getSceneManager("SceneManager");
	_camera = _sceneMgr->createCamera("WinCamera");
	_viewport = _root->getAutoCreatedWindow()->getViewport(0);

	//Adjusting GUI to this state
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitIntroButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitPauseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitWinButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"PlayButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(
			true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"restartButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
						"lifeLabel")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
							"eventLabel")->setVisible(false);
	cout << "Win state" << endl;
	_exitGame = false;
}

void WinState::exit() {
	 _sceneMgr->clearScene();
	 _root->getAutoCreatedWindow()->removeAllViewports();
	 _sceneMgr->destroyAllCameras();
}

void WinState::pause() {
}

void WinState::resume() {
	// Adjusting GUI to this state
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitIntroButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitPauseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitWinButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"PlayButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(
			false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"restartButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
					"lifeLabel")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
							"eventLabel")->setVisible(false);
}

bool WinState::frameStarted(const Ogre::FrameEvent& evt) {
	return true;
}

bool WinState::frameEnded(const Ogre::FrameEvent& evt) {
	if (_exitGame)
		return false;

	return true;
}

void WinState::keyPressed(const OIS::KeyEvent &e) {

	if (e.key == OIS::KC_P) {

	}

	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(
			static_cast<CEGUI::Key::Scan>(e.key));
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);
}

void WinState::keyReleased(const OIS::KeyEvent &e) {
	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}
}

void WinState::mouseMoved(const OIS::MouseEvent &e) {
}

void WinState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
}

void WinState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
}

void WinState::winStateExit() {
	_exitGame = true;
}

WinState*
WinState::getSingletonPtr() {
	return msSingleton;
}

WinState&
WinState::getSingleton() {
	assert(msSingleton);
	return *msSingleton;
}

