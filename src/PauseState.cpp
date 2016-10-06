#include "PauseState.h"

template<> PauseState* Ogre::Singleton<PauseState>::msSingleton = 0;

void PauseState::enter() {
	_root = Ogre::Root::getSingletonPtr();

	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->getSceneManager("SceneManager");
	_camera = _sceneMgr->getCamera("PlayCamera");
	_viewport = _root->getAutoCreatedWindow()->getViewport(0);
	// Nuevo background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 1.0, 0.0));

	//Adjusting GUI to this state
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitIntroButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitPauseButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitWinButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"PlayButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(
			true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
					"restartButton")->setVisible(true);
	_exitGame = false;
}

void PauseState::exit() {
}

void PauseState::pause() {
}

void PauseState::resume() {
	//Adjusting GUI to this state
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitIntroButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitPauseButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitWinButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"PlayButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(
			true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
				"restartButton")->setVisible(true);
}

bool PauseState::frameStarted(const Ogre::FrameEvent& evt) {
	return true;
}

bool PauseState::frameEnded(const Ogre::FrameEvent& evt) {
	if (_exitGame)
		return false;

	return true;
}

void PauseState::keyPressed(const OIS::KeyEvent &e) {
	// Tecla p --> Estado anterior.
	if (e.key == OIS::KC_P) {
		popState();
	}
}

void PauseState::keyReleased(const OIS::KeyEvent &e) {
}

void PauseState::mouseMoved(const OIS::MouseEvent &e) {
}

void PauseState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
}

void PauseState::mouseReleased(const OIS::MouseEvent &e,
		OIS::MouseButtonID id) {
}

void PauseState::pauseStateExit() {
	_exitGame = true;
}

PauseState*
PauseState::getSingletonPtr() {
	return msSingleton;
}

PauseState&
PauseState::getSingleton() {
	assert(msSingleton);
	return *msSingleton;
}
