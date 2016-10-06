#include "Player.h"

Player::Player(string name, char locationPath[100], OgreBulletDynamics::DynamicsWorld * world, int MaxTimeWithoutGettingHit)
:  _frontSpeed(btVector3(15, 0.0, 0.0)), _sideSpeed(btVector3(0.0, 0.0, 15)), _jumpSpeed(Ogre::Vector3(0,4,0)), _sensibility(60.0), _angleY(0.0), _health(100), _timeWithoutGettingHit(MaxTimeWithoutGettingHit), _maxTimeWithoutGettingHit(MaxTimeWithoutGettingHit)
{
    _name = name;
    _iteratorObjectEquipped = 0;
    auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");
    _playerCamera = sceneMgr->getCamera("PlayerCamera");
    _playerCamera->lookAt(5,5,0);

    // LOADING the player
    Ogre::DotSceneLoader* mDotSceneLoader = new Ogre::DotSceneLoader();
    mDotSceneLoader->parseDotScene("patchouli.scene", "General", sceneMgr,
        			sceneMgr->getRootSceneNode());

    // Camera not needed and always deleted to avoid errors
    sceneMgr->destroyCamera("Camera");

    _node = sceneMgr->getSceneNode("Armature");

    // Loading the Player and his/her position
    std::shared_ptr<Scene> scene = make_shared<Scene>();

    // Parse data...
    Importer::getSingletonPtr()->parseScene(locationPath, scene.get());
    std::vector<GraphVertex*> vertexes = scene->getGraph()->getVertexes();

    for (std::vector<GraphVertex*>::const_iterator it = vertexes.begin(); it != vertexes.end(); ++it) {
    	cout << (*it)->getData().getType() << endl;
    	if ((*it)->getData().getType() == "player") {
    		cout << "Player found" << endl;
    		setPosition((*it)->getData().getPosition().x,
                        (*it)->getData().getPosition().y+2,
                        (*it)->getData().getPosition().z);

    		cout << _node->getPosition().x << endl;
    		cout << _node->getPosition().y << endl;
    		cout << _node->getPosition().z << endl;

         }
     }

    //Adding physics to the player
    _xform.setIdentity();
    _xform.setOrigin(btVector3(_node->getPosition().x,_node->getPosition().y,_node->getPosition().z));

    OgreBulletCollisions::AnimatedMeshToShapeConverter *trimeshPlayerConverter = new
    OgreBulletCollisions::AnimatedMeshToShapeConverter(sceneMgr->getEntity("patchBody")); //Entity of the player

    OgreBulletCollisions::CollisionShape *playerTrimesh = trimeshPlayerConverter->createConvex();
    _rigidBody= new OgreBulletDynamics::RigidBody("player", world);

    Ogre::Vector3 VecPlayer = _node->getPosition();
    _rigidBody->setShape(_node, playerTrimesh, 0.0, 0.9, 0.3, VecPlayer, Quaternion::IDENTITY);

    delete trimeshPlayerConverter;

    _rigidBody->enableActiveState();
    //_rigidBody->setKinematicObject(true);

    //This way the object won't have problems like lying on the ground and so on
    _rigidBody->getBulletRigidBody()->setAngularFactor(0.0);
    _rigidBody->getBulletRigidBody()->setSleepingThresholds(0.0, 0.0);

    _animationState = sceneMgr->getEntity("patchBody")->getAnimationState("Idle");
    _animationState->setLoop("true");
    _animationState->setEnabled("true");

    cout << "Player " << _name << " created." << endl;
}

Player::~Player()
{
	_bodies.clear();  _shapes.clear();
	_inventory.clear();
    cout << "Player " << _name << " deleted." << endl;
}

string
Player::getName(){
    return _name;
}

Ogre::SceneNode*
const Player::getNode(){
    return _node;
}

void
Player::setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z){
    _node->setPosition(x, y, z);
}

void
Player::move(bool front, bool back, bool left, bool right, bool firstPerson){
	 auto sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("SceneManager");
	 _walkDirection = btVector3(0.0, 0.0, 0.0);

	 if (left){
	       _walkDirection -= _sideSpeed;
	 }
	 if (right){
	       _walkDirection += _sideSpeed;
	 }
	 if (front){
	       _walkDirection += _frontSpeed;
	 }
	 if (back){
	       _walkDirection -= _frontSpeed;
	 }

	 if (!front && !back && !left&& !right){
		   //_rigidBody->setActivationState(false);
	       _rigidBody->getBulletRigidBody()->setLinearVelocity(btVector3(0.0, 0.0, 0.0));
	       _animationState = sceneMgr->getEntity("patchBody")->getAnimationState("Idle");
	 }else{
		 _rigidBody->setActivationState(true);
		 if(firstPerson==false){
			 _rigidBody->setLinearVelocity(-_walkDirection.x(), -_walkDirection.y(), -_walkDirection.z());
		 }else{
			 _rigidBody->getBulletRigidBody()->setLinearVelocity(QmV3(_walkDirection, _xform.getRotation()));
		 }
		 _animationState = sceneMgr->getEntity("patchBody")->getAnimationState("Walk");
		 _animationState->setLoop("true");
		 _animationState->setEnabled("true");
	 }

	 _playerCamera->setPosition(_node->getPosition()+Ogre::Vector3(0,2,0));
}

void
Player::rotate(){
	Ogre::Quaternion quat = _playerCamera->getOrientation();
	_rigidBody->getBulletRigidBody()->getMotionState()->getWorldTransform(_xform);
	auto aux = quat.w;
	_xform.setRotation(btQuaternion(0, quat.y, 0, aux)*btQuaternion(0,1,0,1));
	_rigidBody->getBulletRigidBody()->getMotionState()->setWorldTransform(_xform);
	_rigidBody->getBulletRigidBody()->setCenterOfMassTransform(_xform);

}

void
Player::jump(Vector3 cameraD){
	GameManager::getSingletonPtr()->playJumpSound();
	_rigidBody->setActivationState(true);
	_rigidBody->applyImpulse(_jumpSpeed+cameraD*Vector3(2,0,2),_node->getPosition());
}

void Player::addTime(float time){
	_animationState->addTime(time);
}

void Player::addToInventory(GameObject* object){
	_inventory.push_back(object);
	cout << object->getName() << " added to the player" << endl;
}

bool Player::useObject(string name){
	bool found = false;
	for (int j = 0; j < _inventory.size() && found == false; j++) {
		if(_inventory[j]->getName() == name){
			found = true;
			_inventory.erase(_inventory.begin() + j);
		}
	}

	return found;
}

bool Player::takeDamage(int damage){
	if(_timeWithoutGettingHit >= _maxTimeWithoutGettingHit){
		_health = _health - damage;
		_timeWithoutGettingHit = 0;
		return true;
	}

	return false;
}

void Player::addTimeWithoutGettingHit(float time){
	if(_timeWithoutGettingHit<=_maxTimeWithoutGettingHit){
		_timeWithoutGettingHit = _timeWithoutGettingHit + time;
	}
}

void Player::rotateCamera(double x){
	//_playerCamera->lookAt(vecDistance);
	_playerCamera->yaw(Ogre::Degree(x));
}

Ogre::Vector3 Player::getCameraDirection(){
	return _playerCamera->getDirection();
}

double Player::getDamageEquippedWeapon(){
	if(_inventory.empty()){
		return 1;
	}else{
		return _inventory[_iteratorObjectEquipped]->getDamage();
	}
}

int Player::getHealth(){
	return _health;
}

btVector3 Player::QmV3(const btVector3 & v, const btQuaternion & q)
{
   btVector3 qv(q.x(), q.y(), q.z());
   btVector3 uv = qv.cross(v);
   btVector3 uuv = qv.cross(uv);
   uv *= (2.0 * q.w());
   uuv *= 2.0;
   return v + uv + uuv;
}
