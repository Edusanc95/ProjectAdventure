#include "Stage.h"
#include "PlayState.h"

Stage::Stage(char* level, char locationPath[100],
		OgreBulletDynamics::DynamicsWorld * world) {

	_level = level;
	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");

	sceneMgr->setSkyBox(true, "MySky");
	_world = world;

	// LOADING the level
	Ogre::DotSceneLoader* mDotSceneLoader = new Ogre::DotSceneLoader();
	char scenePath[100];
	Ogre::SceneNode* m_pSceneNode;

	strcpy(scenePath, "");
	strcat(scenePath, _level);
	strcat(scenePath, ".scene");

	mDotSceneLoader->parseDotScene(scenePath, "General", sceneMgr,
			m_pSceneNode = sceneMgr->getRootSceneNode());

	// Camera not needed and always deleted to avoid errors
	sceneMgr->destroyCamera("disposableCamera");

	// Making physics for the level
	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter =
			new OgreBulletCollisions::StaticMeshToShapeConverter(
					sceneMgr->getEntity(_level)); //Entity of the level

	OgreBulletCollisions::CollisionShape *trackTrimesh =
			trimeshConverter->createTrimesh();

	OgreBulletDynamics::RigidBody *rigidTrack =
			new OgreBulletDynamics::RigidBody(_level, world);

	// OgreBulletCollisions::CollisionShape *Shape;
	//  Shape = new OgreBulletCollisions::StaticPlaneCollisionShape
	//    (Ogre::Vector3(0,1,0), 0);   // Vector normal y distancia
	//  OgreBulletDynamics::RigidBody *rigidBodyPlane = new
	//    OgreBulletDynamics::RigidBody("rigidBodyPlane", _world);

	Ogre::Vector3 VecTrack = Ogre::Vector3(0, 0, 0); //_sceneMgr->getSceneNode("test_stage")->getPosition();
	rigidTrack->setShape(sceneMgr->getSceneNode(_level), trackTrimesh, 0.8,
			0.95, 0, VecTrack, Quaternion::IDENTITY);

	delete trimeshConverter;

	// Loading the objects of the level
	std::shared_ptr<Scene> scene = make_shared<Scene>();

	// Parse data...
	Importer::getSingletonPtr()->parseScene(locationPath, scene.get());
	std::vector<GraphVertex*> vertexes = scene->getGraph()->getVertexes();

	// Adding things to the stage
	_nChests = 0;
	_nDoors = 0;
	_nGameEnemies = 0;
	_nBalls = 0;
	_nTotalBalls = 0;
	_MaxBalls = 3;
	_nExits = 0;
	_nTraps = 0;
	_nSigns = 0;
	for (std::vector<GraphVertex*>::const_iterator it = vertexes.begin();
			it != vertexes.end(); ++it) {
		cout << (*it)->getData().getType() << endl;
		if ((*it)->getData().getType() == "chestKey") {
			_nChests++;
			cout << "Chest with key found" << endl;
			Chest aux("_Chest" + to_string(_nChests),
					Ogre::Vector3((*it)->getData().getPosition().x,
							(*it)->getData().getPosition().z,
							-(*it)->getData().getPosition().y), world, 1);
			_chests.push_back(aux);
		}

		if ((*it)->getData().getType() == "signHouse") {
			_nSigns++;
			cout << "Sign of the House found" << endl;
			Sign aux("_Sign" + to_string(_nSigns),
					Ogre::Vector3((*it)->getData().getPosition().x,
							(*it)->getData().getPosition().z,
							-(*it)->getData().getPosition().y), world,
					"Patchouli's home.");
			_Signs.push_back(aux);
		}
		if ((*it)->getData().getType() == "chestAS") {
			_nChests++;
			cout << "Chest with arcade stick found" << endl;
			Chest aux("_Chest" + to_string(_nChests),
					Ogre::Vector3((*it)->getData().getPosition().x,
							(*it)->getData().getPosition().z,
							-(*it)->getData().getPosition().y), world, 2);
			_chests.push_back(aux);
		}

		if ((*it)->getData().getType() == "doorKey") {
			_nDoors++;
			cout << "Door that opens with a key found" << endl;
			Door aux("_Door" + to_string(_nDoors),
					Ogre::Vector3((*it)->getData().getPosition().x,
							(*it)->getData().getPosition().z,
							-(*it)->getData().getPosition().y), world, "Key");
			_doors.push_back(aux);
		}

		if ((*it)->getData().getType() == "exit") {
			_nExits++;
			cout << "Exit" << endl;
			Exit aux("_Exit" + to_string(_nExits),
					Ogre::Vector3((*it)->getData().getPosition().x,
							(*it)->getData().getPosition().z,
							-(*it)->getData().getPosition().y), world);
			_Exits.push_back(aux);
		}

		if ((*it)->getData().getType() == "trap") {
			_nTraps++;
			cout << "Trap" << endl;
			Trap aux("_Trap" + to_string(_nTraps),
					Ogre::Vector3((*it)->getData().getPosition().x,
							(*it)->getData().getPosition().z,
							-(*it)->getData().getPosition().y), world, 25);
			_Traps.push_back(aux);
		}

		if ((*it)->getData().getType() == "blueSlime") {
			_nGameEnemies++;
			cout << "Blue slime (enemy) found" << endl;
			MeleeEnemy* aux = new MeleeEnemy(
					"_Enemy" + to_string(_nGameEnemies),
					Ogre::Vector3((*it)->getData().getPosition().x,
							(*it)->getData().getPosition().z,
							-(*it)->getData().getPosition().y), world, 15, 10,
					"sphere", 15);
			_GameEnemies.push_back(aux);
		}
	}

	// Concatenate this into an string for a better optimization later
	cout << "Stage: " << _level << " created." << endl;
}

Stage::~Stage() {
	_chests.clear();
	_doors.clear();
	_GameEnemies.clear();
	_BallBullets.clear();
	cout << "Stage " << _level << " deleted." << endl;
}

bool Stage::DetectCollisionPlayer(std::shared_ptr<Player> playerP) {
	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");
	btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold =
				bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);

		for (int j = 0; j < contactManifold->getNumContacts(); j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if (pt.getDistance() < 4) {
				btCollisionObject* obA =
						(btCollisionObject*) (contactManifold->getBody0());
				btCollisionObject* obB =
						(btCollisionObject*) (contactManifold->getBody1());

				Ogre::SceneNode* player = playerP->getNode();
				Ogre::SceneNode* stage = sceneMgr->getSceneNode(_level);

				OgreBulletCollisions::Object *obDrain = _world->findObject(
						player);
				OgreBulletCollisions::Object *obStage = _world->findObject(
						stage);

				OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
				OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

				if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
					Ogre::SceneNode* node = NULL;
					if ((obOB_A == obStage) && (obOB_A)) {
						node = obDrain->getRootNode();

					} else if ((obOB_B == obStage) && (obOB_B)) {
						node = obDrain->getRootNode();
					}
					if (node) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool Stage::openChest(std::shared_ptr<Player> playerP) {

	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");
	btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

	Ogre::SceneNode* player = playerP->getNode();
	OgreBulletCollisions::Object *obPlayer = _world->findObject(player);

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold =
				bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA =
				(btCollisionObject*) (contactManifold->getBody0());
		btCollisionObject* obB =
				(btCollisionObject*) (contactManifold->getBody1());

		for (int j = 0; j < _chests.size() && _nChests > 0; j++) {
			Ogre::SceneNode* chest = sceneMgr->getSceneNode(
					_chests[j].getName());

			OgreBulletCollisions::Object *obDrain = _world->findObject(chest);

			OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
			OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

			if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
				Ogre::SceneNode* node = NULL;
				if ((obOB_A == obPlayer) && (obOB_A)) {
					node = obDrain->getRootNode();
					//delete obDrain;
				} else if ((obOB_B == obPlayer) && (obOB_B)) {
					node = obDrain->getRootNode();
					//delete obDrain;
				}
				if (node) {
					cout << "Chest open" << endl;
					CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
							"eventLabel")->setText("Object obtained: "+_chests[j].getGameObject()->getName());
					playerP->addToInventory(_chests[j].getGameObject());
					_nChests--;
					//Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager")->destroySceneNode(node);
					//sceneMgr->getRootSceneNode()->removeAndDestroyChild(node->getName());
					GameManager::getSingletonPtr()->playGetPointSound();
					_chests.erase(_chests.begin() + j);
					//GameManager::getSingletonPtr()->playGetPointSound();
					return true;
				}
			}
		}
	}

	return false;
}

bool Stage::openDoor(std::shared_ptr<Player> playerP) {

	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");
	btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

	Ogre::SceneNode* player = playerP->getNode();
	OgreBulletCollisions::Object *obPlayer = _world->findObject(player);

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold =
				bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA =
				(btCollisionObject*) (contactManifold->getBody0());
		btCollisionObject* obB =
				(btCollisionObject*) (contactManifold->getBody1());

		for (int j = 0; j < _doors.size() && _nDoors > 0; j++) {
			Ogre::SceneNode* door = sceneMgr->getSceneNode(_doors[j].getName());

			OgreBulletCollisions::Object *obDrain = _world->findObject(door);

			OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
			OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

			if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
				Ogre::SceneNode* node = NULL;
				if ((obOB_A == obPlayer) && (obOB_A)) {
					node = obDrain->getRootNode();
					//delete obDrain;
				} else if ((obOB_B == obPlayer) && (obOB_B)) {
					node = obDrain->getRootNode();
					//delete obDrain;
				}
				if (node) {
					if (playerP->useObject(_doors[j].getOpensWith())) {
						cout << "Door open" << endl;
						CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
								"eventLabel")->setText("Door open");
						delete obDrain;
						_nDoors--;
						//Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager")->destroySceneNode(node);
						sceneMgr->getRootSceneNode()->removeAndDestroyChild(
								node->getName());
						_doors.erase(_doors.begin() + j);
						//GameManager::getSingletonPtr()->playGetPointSound();
						return true;
					} else {
						CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
								"eventLabel")->setText(
								"You need a " + _doors[j].getOpensWith());
						cout << "You need a " << _doors[j].getOpensWith()
								<< " to open the door" << endl;
					}
				}
			}
		}
	}

	return false;
}

bool Stage::readSign(std::shared_ptr<Player> playerP) {
	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");
	btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

	Ogre::SceneNode* player = playerP->getNode();
	OgreBulletCollisions::Object *obPlayer = _world->findObject(player);

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold =
				bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA =
				(btCollisionObject*) (contactManifold->getBody0());
		btCollisionObject* obB =
				(btCollisionObject*) (contactManifold->getBody1());

		for (int j = 0; j < _Signs.size() && _nSigns > 0; j++) {
			Ogre::SceneNode* sign = sceneMgr->getSceneNode(_Signs[j].getName());

			OgreBulletCollisions::Object *obDrain = _world->findObject(sign);

			OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
			OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

			if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
				Ogre::SceneNode* node = NULL;
				if ((obOB_A == obPlayer) && (obOB_A)) {
					node = obDrain->getRootNode();
					//delete obDrain;
				} else if ((obOB_B == obPlayer) && (obOB_B)) {
					node = obDrain->getRootNode();
					//delete obDrain;
				}
				if (node) {
					CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
							"eventLabel")->setText(_Signs[j].getMessage());
					cout << _Signs[j].getMessage() << endl;
				}
			}
		}
	}

	return false;
}
bool Stage::DetectCollisionEnemy(std::shared_ptr<Player> playerP) {
	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");
	btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

	Ogre::SceneNode* player = playerP->getNode();
	OgreBulletCollisions::Object *obPlayer = _world->findObject(player);

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold =
				bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);

		for (int j = 0; j < contactManifold->getNumContacts(); j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if (pt.getDistance() < 0.1f) {
				btCollisionObject* obA =
						(btCollisionObject*) (contactManifold->getBody0());
				btCollisionObject* obB =
						(btCollisionObject*) (contactManifold->getBody1());

				for (int j = 0; j < _GameEnemies.size() && _nGameEnemies > 0;
						j++) {
					Ogre::SceneNode* enemy = sceneMgr->getSceneNode(
							_GameEnemies[j]->getName());

					OgreBulletCollisions::Object *obDrain = _world->findObject(
							enemy);

					OgreBulletCollisions::Object *obOB_A = _world->findObject(
							obA);
					OgreBulletCollisions::Object *obOB_B = _world->findObject(
							obB);

					if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
						Ogre::SceneNode* node = NULL;
						if ((obOB_A == obPlayer) && (obOB_A)) {
							node = obDrain->getRootNode();
						} else if ((obOB_B == obPlayer) && (obOB_B)) {
							node = obDrain->getRootNode();

						}
						if (node) {
							if (playerP->takeDamage(
									_GameEnemies[j]->getDamage())) {
								CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
										"eventLabel")->setText(
										"-"
												+ to_string(
														_GameEnemies[j]->getDamage())
												+ " health");
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;

}

void Stage::iterateStateGameEnemies(Ogre::Node* playerNode) {
	for (int j = 0; j < _GameEnemies.size() && _nGameEnemies > 0; j++) {
		_GameEnemies[j]->stateMachine(playerNode);
	}
}

void Stage::attack(std::shared_ptr<Player> playerP, bool firstPerson) {
	if (_nBalls < _MaxBalls) {
		cout << "Shooting" << endl;
		_nBalls++;
		_nTotalBalls++;
		BallBullet aux("_BallBullet" + to_string(_nTotalBalls), "ball.mesh",
				playerP->getNode()->getPosition(),
				playerP->getCameraDirection(), _world, 0.75,
				playerP->getDamageEquippedWeapon(), firstPerson);
		_BallBullets.push_back(aux);
		GameManager::getSingletonPtr()->playShootSound();
	} else {
		cout << "Too many balls" << endl;
	}
}

void Stage::iterateBulletsTimeLeft(double delta) {
	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");

	for (int j = 0; j < _BallBullets.size() && _nBalls > 0; j++) {
		if (_BallBullets[j].timeLeft(delta)) {
			Ogre::SceneNode* ball = sceneMgr->getSceneNode(
					_BallBullets[j].getName());
			OgreBulletCollisions::Object *obBall = _world->findObject(ball);
			delete obBall;
			_BallBullets.erase(_BallBullets.begin() + j);
			sceneMgr->getRootSceneNode()->removeAndDestroyChild(
					ball->getName());
			_nBalls--;
		}
	}
}

void Stage::iterateCollisionEnemies() {
	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");
	btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold =
				bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
		//for (int ww = 0; ww < contactManifold->getNumContacts(); ww++) {
		//	btManifoldPoint& pt = contactManifold->getContactPoint(ww);

		//if (pt.getDistance() < 0.1f) {

		btCollisionObject* obA =
				(btCollisionObject*) (contactManifold->getBody0());
		btCollisionObject* obB =
				(btCollisionObject*) (contactManifold->getBody1());

		for (int j = 0;
				j < _GameEnemies.size() && _nBalls > 0 && _nGameEnemies > 0;
				j++) {
			for (int x = 0;
					x < _BallBullets.size() && _nBalls > 0 && _nGameEnemies > 0;
					x++) {

				Ogre::SceneNode* enemy = sceneMgr->getSceneNode(
						_GameEnemies[j]->getName());
				Ogre::SceneNode* ball = sceneMgr->getSceneNode(
						_BallBullets[x].getName());

				OgreBulletCollisions::Object *obEnemy = _world->findObject(
						enemy);
				OgreBulletCollisions::Object *obBall = _world->findObject(ball);

				OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
				OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

				if ((obOB_A == obEnemy) || (obOB_B == obEnemy)) {
					Ogre::SceneNode* node = NULL;
					if ((obOB_A == obBall) && (obOB_A)) {
						node = obEnemy->getRootNode();
					} else if ((obOB_B == obBall) && (obOB_B)) {
						node = obEnemy->getRootNode();

					}

					if (node) {
						cout << "Enemigo disparado" << endl;
						if (_GameEnemies[j]->takeDamage(
								_BallBullets[x].getDamage())) {
							_nGameEnemies--;
							delete obEnemy;
							sceneMgr->getRootSceneNode()->removeAndDestroyChild(
									enemy->getName());
							_GameEnemies.erase(_GameEnemies.begin() + j);

						}
						// We only delete the physic part
						delete obBall;

						//_BallBullets.erase(_BallBullets.begin() + j);
						//sceneMgr->getRootSceneNode()->removeAndDestroyChild(
						//		ball->getName());
						//_nBalls--;
						//if(playerP->takeDamage(_GameEnemies[j]->getDamage())){
						//	cout << "Collision with an enemy of "<< _GameEnemies[j]->getDamage() << endl;
						//}
					}
				}
				//}
				//}
			}
		}
	}
}
bool Stage::DetectExit(std::shared_ptr<Player> playerP) {

	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");
	btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

	Ogre::SceneNode* player = playerP->getNode();
	OgreBulletCollisions::Object *obPlayer = _world->findObject(player);

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold =
				bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);

		for (int j = 0; j < contactManifold->getNumContacts(); j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if (pt.getDistance() < 0.1f) {
				btCollisionObject* obA =
						(btCollisionObject*) (contactManifold->getBody0());
				btCollisionObject* obB =
						(btCollisionObject*) (contactManifold->getBody1());

				for (int j = 0; j < _Exits.size() && _nExits > 0; j++) {
					Ogre::SceneNode* exit = sceneMgr->getSceneNode(
							_Exits[j].getName());

					OgreBulletCollisions::Object *obDrain = _world->findObject(
							exit);

					OgreBulletCollisions::Object *obOB_A = _world->findObject(
							obA);
					OgreBulletCollisions::Object *obOB_B = _world->findObject(
							obB);

					if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
						Ogre::SceneNode* node = NULL;
						if ((obOB_A == obPlayer) && (obOB_A)) {
							node = obDrain->getRootNode();
							delete obDrain;
						} else if ((obOB_B == obPlayer) && (obOB_B)) {
							node = obDrain->getRootNode();
							delete obDrain;
						}
						if (node) {
							cout << "Exit triggered" << endl;
							GameManager::getSingletonPtr()->pauseTrack();
							GameManager::getSingletonPtr()->playWinSound();
							_nExits--;
							//Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager")->destroySceneNode(node);
							sceneMgr->getRootSceneNode()->removeAndDestroyChild(
									node->getName());
							_Exits.erase(_Exits.begin() + j);
							//GameManager::getSingletonPtr()->playGetPointSound();
							PlayState::getSingletonPtr()->isBetter();
							return true;
						}
					}
				}
			}
		}
	}
	return false;

}

bool Stage::DetectCollisionTrap(std::shared_ptr<Player> playerP) {

	auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");
	btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

	Ogre::SceneNode* player = playerP->getNode();
	OgreBulletCollisions::Object *obPlayer = _world->findObject(player);

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold =
				bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);

		for (int j = 0; j < contactManifold->getNumContacts(); j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if (pt.getDistance() < 0.1f) {
				btCollisionObject* obA =
						(btCollisionObject*) (contactManifold->getBody0());
				btCollisionObject* obB =
						(btCollisionObject*) (contactManifold->getBody1());

				for (int j = 0; j < _Traps.size() && _nTraps > 0; j++) {
					Ogre::SceneNode* trap = sceneMgr->getSceneNode(
							_Traps[j].getName());

					OgreBulletCollisions::Object *obDrain = _world->findObject(
							trap);

					OgreBulletCollisions::Object *obOB_A = _world->findObject(
							obA);
					OgreBulletCollisions::Object *obOB_B = _world->findObject(
							obB);

					if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
						Ogre::SceneNode* node = NULL;
						if ((obOB_A == obPlayer) && (obOB_A)) {
							node = obDrain->getRootNode();
							//delete obDrain;
						} else if ((obOB_B == obPlayer) && (obOB_B)) {
							node = obDrain->getRootNode();
							//delete obDrain;
						}
						if (node) {
							//cout << "Trap triggered" << endl;
							//_nTraps--;
							//Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager")->destroySceneNode(node);
							//sceneMgr->getRootSceneNode()->removeAndDestroyChild(
							//		node->getName());
							//_Traps.erase(_Traps.begin() + j);
							//GameManager::getSingletonPtr()->playGetPointSound();
							if (playerP->takeDamage(_Traps[j].getDamage())) {
								cout << "Collision with a trap of "
										<< _Traps[j].getDamage() << endl;
								CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
										"eventLabel")->setText(
										"-" + to_string(_Traps[j].getDamage())
												+ " health");
								return true;
							}
							return true;
						}
					}
				}
			}
		}
	}
	return false;

}

void Stage::addTime(float time) {
	for (int j = 0; j < _GameEnemies.size() && _nGameEnemies > 0; j++) {
		_GameEnemies[j]->addTime(time);
	}

	//for (int j = 0; j < _chests.size() && _nChests > 0; j++) {
	//	_chests[j].addTime(time);
	//}
}
