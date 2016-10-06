#include "PlayState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "LoseState.h"
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void PlayState::loadLevel(char* level) {
	// Deleting the previous level
	_sceneMgr->clearScene();

	// Camera positioning
	//_camera->setPosition(2,5,0);
	//_camera->lookAt(5,5,0);

	// Auxiliar variables
	char locationPath[100];

	addingPhysics();

	strcpy(locationPath, "./data/levels/");
	strcat(locationPath, level);
	strcat(locationPath, ".xml");

	_stage = std::make_shared < Stage > (level, locationPath, _world);

	_player = std::make_shared < Player > ("Player1", locationPath, _world, 1);

	_movementVariables._moveUp = false;
	_movementVariables._moveDown = false;
	_movementVariables._moveRight = false;
	_movementVariables._moveLeft = false;
	_movementVariables._jump = false;

	cout << _player->getNode()->getPosition().x << endl;
	cout << _player->getNode()->getPosition().y << endl;
	cout << _player->getNode()->getPosition().z << endl;

	if (_firstPerson == true) {
		_player->getNode()->setVisible(false);
	} else {
		_player->getNode()->setVisible(true);
	}

	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
								"eventLabel")->setText("");
	_timeWithoutTouchingFloor = 0;
	//_sceneMgr->destroyAllLights();
	//_camera->lookAt(Ogre::Vector3(_player->getNode()->getPosition()));
	//_camera->pitch(Ogre::Degree(90));
}

void PlayState::addingPhysics() {
	// Creacion del modulo de debug visual de Bullet ------------------
	_debugDrawer = new OgreBulletCollisions::DebugDrawer();
	_debugDrawer->setDrawWireframe(true);
	SceneNode *node = _sceneMgr->getRootSceneNode()->createChildSceneNode(
			"debugNode", Vector3::ZERO);
	node->attachObject(static_cast<SimpleRenderable *>(_debugDrawer));

	// Creacion del mundo (definicion de los limites y la gravedad) ---
	AxisAlignedBox worldBounds = AxisAlignedBox(Vector3(-100, -100, -100),
			Vector3(100, 100, 100));
	Vector3 gravity = Vector3(0, -9.8, 0);

	_world = new OgreBulletDynamics::DynamicsWorld(_sceneMgr, worldBounds,
			gravity);
	_world->setDebugDrawer(_debugDrawer);
}

void PlayState::enter() {
	_root = Ogre::Root::getSingletonPtr();

	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->getSceneManager("SceneManager");

	/*
	 _sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	 _sceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
	 _sceneMgr->setAmbientLight(ColourValue(0.9, 0.9, 0.9));
	 _sceneMgr->setShadowTextureCount(2);
	 _sceneMgr->setShadowTextureSize(512);
	 */

	_camera = _sceneMgr->createCamera("PlayCamera");
	_sceneMgr->createCamera("PlayerCamera");
	_sceneMgr->getCamera("PlayerCamera")->setNearClipDistance(1);
	_camera->setNearClipDistance(1);
	_camera->pitch(Ogre::Degree(-45));
	_camera->yaw(Ogre::Degree(90));

	//_camera->setPosition(0,10,90);
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	// Nuevo background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));

	GameManager::getSingletonPtr()->changeTrack("playstate.wav");

	// Adjusting GUI to this state
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitIntroButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitPauseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"PlayButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitWinButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(
			false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"restartButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitLoseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
				"lifeLabel")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
					"eventLabel")->setVisible(true);
	_exitGame = false;
	_firstPerson = false;

	loadLevel("first_stage");

}

void PlayState::exit() {
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
	_sceneMgr->destroyAllCameras();
	_player.reset();
	_stage.reset();
	deletePhysicsObject();

	GameManager::getSingletonPtr()->changeTrack("maintheme.wav");
	// Adjusting GUI to this state for the IntroState
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitIntroButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitPauseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"PlayButton")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitWinButton")->setVisible(false);
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
			"restartButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitLoseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
					"lifeLabel")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
						"eventLabel")->setVisible(false);
}

void PlayState::pause() {
}

void PlayState::resume() {
	// Se restaura el background colour.
	// Adjusting GUI to this state
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitIntroButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitPauseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"PlayButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitWinButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(
			false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"restartButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"quitLoseButton")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
					"lifeLabel")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
							"eventLabel")->setVisible(true);
	// Se restaura el background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt) {

	_delta += evt.timeSinceLastFrame;

	if (_player->getHealth() <= 0) {
		GameManager::getSingletonPtr()->pauseTrack();
		GameManager::getSingletonPtr()->playDeathSound();
		pushState(LoseState::getSingletonPtr());
	}

	if (_firstPerson == false) {
		_camera->setPosition(
				_player->getNode()->getPosition() + Ogre::Vector3(22, 25, 0));
	} else {
		_camera->setPosition(
				_player->getNode()->getPosition() + Ogre::Vector3(0, 2, 0));
	}
	_player->rotate();
	//if(evt.timeSinceLastFrame<16.66667){
	//	  sleep(0.01666667 - (evt.timeSinceLastFrame*0.001));
	//}

	_player->addTime(evt.timeSinceLastFrame);
	_player->addTimeWithoutGettingHit(evt.timeSinceLastFrame);

	if (_stage->DetectCollisionPlayer(_player)) {
		if (_movementVariables._jump) {
			_player->jump(Ogre::Vector3(0, 0, 0));
		} else {
			if ((_movementVariables._moveUp || _movementVariables._moveDown
					|| _movementVariables._moveRight
					|| _movementVariables._moveLeft)) {
				_player->move(_movementVariables._moveUp,
						_movementVariables._moveDown,
						_movementVariables._moveLeft,
						_movementVariables._moveRight, _firstPerson);
			}
		}
	}

	int nMaxSteps = evt.timeSinceLastFrame / (1 / 60) + 1; // time divided by 1/60th second
	_world->stepSimulation(evt.timeSinceLastFrame, nMaxSteps, 1 / 60);
	_stage->addTime(evt.timeSinceLastFrame);

	_stage->DetectCollisionEnemy(_player);
	_stage->DetectExit(_player);
	_stage->DetectCollisionTrap(_player);

	_stage->iterateStateGameEnemies(_player->getNode());
	_stage->iterateBulletsTimeLeft(evt.timeSinceLastFrame);
	_stage->iterateCollisionEnemies();

	if (_stage->DetectCollisionPlayer(_player)) {
		_timeWithoutTouchingFloor = 0;
	} else {
		_timeWithoutTouchingFloor += evt.timeSinceLastFrame;
	}

	//To see if the player has fallen or if he/she's bugged. An alternative method is using a plane to calculate the collision when the player falls,
	//but this way we also cover certain possible bugs
	if (_timeWithoutTouchingFloor > 5) {
		_player->takeDamage(9999);
	}

	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
							"lifeLabel")->setText("Life: "+std::to_string(_player->getHealth()));
	//cout << 1.0 / evt.timeSinceLastFrame << endl;
	return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt) {
	if (_exitGame)
		return false;

	_world->stepSimulation(evt.timeSinceLastFrame, 5);

	return true;
}

void PlayState::keyPressed(const OIS::KeyEvent &e) {
	// Tecla p --> PauseState.
	if (e.key == OIS::KC_P) {
		pushState(PauseState::getSingletonPtr());
	}

	// Tecla c --> IntroState
	if (e.key == OIS::KC_C) {
		changeState(IntroState::getSingletonPtr());
	}

	// Tecla N --> Load test level.
	if (e.key == OIS::KC_N) {
		deletePhysicsObject();
		loadLevel("test_stage");
	}

	// Tecla Q --> Change type of camera.
	if (e.key == OIS::KC_Q) {
		if (_firstPerson == false) {
			_camera = _sceneMgr->getCamera("PlayerCamera");
			_player->getNode()->setVisible(false);
			_viewport->setCamera(_camera);
			_firstPerson = true;
		} else {
			_camera = _sceneMgr->getCamera("PlayCamera");
			_player->getNode()->setVisible(true);
			_viewport->setCamera(_camera);
			_firstPerson = false;
		}
	}

	// Tecla M --> Load first Level.
	if (e.key == OIS::KC_M) {
		deletePhysicsObject();
		loadLevel("first_stage");
	}

	// Movement
	if (e.key == OIS::KC_W) {
		_movementVariables._moveUp = true;
	}

	if (e.key == OIS::KC_S) {
		_movementVariables._moveDown = true;
	}

	if (e.key == OIS::KC_D) {
		_movementVariables._moveRight = true;
	}

	if (e.key == OIS::KC_A) {
		_movementVariables._moveLeft = true;
	}

	if (e.key == OIS::KC_SPACE && _stage->DetectCollisionPlayer(_player)) {
		//_player->jump(_camera->getDirection());
		_movementVariables._jump = true;
	}

	// Tecla E --> "Use" button.
	if (e.key == OIS::KC_E) {
		_stage->openChest(_player);
		_stage->openDoor(_player);
		_stage->readSign(_player);
	}

	// Keys for physics debug
	if (e.key == OIS::KC_G)
		_world->setShowDebugShapes(true);
	if (e.key == OIS::KC_H)
		_world->setShowDebugShapes(false);
}

void PlayState::keyReleased(const OIS::KeyEvent &e) {
	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}

	//Test for movement
	if (e.key == OIS::KC_W) {
		_movementVariables._moveUp = false;
		_player->move(false, false, false, false, _firstPerson);
	}

	if (e.key == OIS::KC_S) {
		_movementVariables._moveDown = false;
		_player->move(false, false, false, false, _firstPerson);
	}

	if (e.key == OIS::KC_D) {
		_movementVariables._moveRight = false;
		_player->move(false, false, false, false, _firstPerson);
	}

	if (e.key == OIS::KC_A) {
		_movementVariables._moveLeft = false;
		_player->move(false, false, false, false, _firstPerson);
	}

	if (e.key == OIS::KC_SPACE) {
		_movementVariables._jump = false;
	}
}

void PlayState::mouseMoved(const OIS::MouseEvent &e) {
	if (_firstPerson == false) {
		_player->rotateCamera(e.state.X.rel * -0.1f);
	} else {
		_camera->yaw(Ogre::Degree(e.state.X.rel * -0.1f));
		_camera->pitch(Ogre::Degree(e.state.Y.rel * -0.1f));
	}
}

void PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	if (id == OIS::MB_Left) {
		_stage->attack(_player, _firstPerson);
	}
}

void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
}

void PlayState::playStateExit() {
	_exitGame = true;
}

PlayState*
PlayState::getSingletonPtr() {
	return msSingleton;
}

PlayState&
PlayState::getSingleton() {
	assert(msSingleton);
	return *msSingleton;
}

void PlayState::deletePhysicsObject() {

	// Eliminar mundo dinamico y debugDrawer -------------------------
	delete _world->getDebugDrawer();
	_world->setDebugDrawer(0);
	delete _world;
}

void PlayState::isBetter() {
	cout << "You win!" << endl;
	//We start to check if we get a high score
	string line;
	ifstream myfile("media/txt/scoreboard.txt");
	bool notBetter = true;

	pushState(WinState::getSingletonPtr());

	if (myfile.is_open()) {
		getline(myfile, line);
		while (getline(myfile, line)) {
			if (line[0] - '0' <= getPoints()) {
				GameManager::getSingletonPtr()->createIntroduceName_window();
				notBetter = false;
			}
		}
		myfile.close();
	} else {
		cout << "Unable to open file";
	}

}

int PlayState::getPoints() {
	return _delta;
}

