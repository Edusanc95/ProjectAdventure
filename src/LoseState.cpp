/*
 * LoseState.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: edu
 */

#include "LoseState.h"
#include "PlayState.h"
#include <iostream>

using namespace std;
template<> LoseState* Ogre::Singleton<LoseState>::msSingleton = 0;

void LoseState::enter() {
	_root = Ogre::Root::getSingletonPtr();

	//Sounds and tracks management
	//GameManager::getSingletonPtr()->changeTrack("losestate.mp3");

	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->getSceneManager("SceneManager");
	_camera = _sceneMgr->createCamera("LoseCamera");
	_viewport = _root->getAutoCreatedWindow()->getViewport(0);

	//Adjusting GUI to this state
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitIntroButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitPauseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitWinButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitLoseButton")->setVisible(true);
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
			"lifeLabel")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
							"restartButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
						"lifeLabel")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
						"eventLabel")->setVisible(false);
	cout << "Lose State" << endl;
	_exitGame = false;
}

void LoseState::exit() {

}

void LoseState::pause() {
}

void LoseState::resume() {
	// Adjusting GUI to this state
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitIntroButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitPauseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitWinButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitLoseButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"PlayButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(
			false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"lifeLabel")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
							"restartButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
						"lifeLabel")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
							"eventLabel")->setVisible(false);
}

bool LoseState::frameStarted(const Ogre::FrameEvent& evt) {
	return true;
}

bool LoseState::frameEnded(const Ogre::FrameEvent& evt) {
	if (_exitGame)
		return false;

	return true;
}

void LoseState::keyPressed(const OIS::KeyEvent &e) {

	if (e.key == OIS::KC_P) {

	}

	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(
			static_cast<CEGUI::Key::Scan>(e.key));
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);
}

void LoseState::keyReleased(const OIS::KeyEvent &e) {
	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}
}

void LoseState::mouseMoved(const OIS::MouseEvent &e) {
}

void LoseState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
}

void LoseState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
}

void LoseState::LoseStateExit() {
	_exitGame = true;
}

LoseState*
LoseState::getSingletonPtr() {
	return msSingleton;
}

LoseState&
LoseState::getSingleton() {
	assert(msSingleton);
	return *msSingleton;
}

