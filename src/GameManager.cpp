#include <Ogre.h>

#include "GameManager.h"
#include "GameState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "WinState.h"
#include "LoseState.h"

using namespace std;
template<> GameManager* Ogre::Singleton<GameManager>::msSingleton = 0;

GameManager::GameManager() {
	_root = 0;
}

GameManager::~GameManager() {
	while (!_states.empty()) {
		_states.top()->exit();
		_states.pop();
	}

	if (_root)
		delete _root;
}

void GameManager::start(GameState* state) {
	// Creación del objeto Ogre::Root.
	_root = new Ogre::Root();

	loadResources();

	if (!configure())
		return;

	_inputMgr = new InputManager;
	_inputMgr->initialise(_renderWindow);

	// Registro como key y mouse listener...
	_inputMgr->addKeyListener(this, "GameManager");
	_inputMgr->addMouseListener(this, "GameManager");

	// El GameManager es un FrameListener.
	_root->addFrameListener(this);

	// Creacion del scene manager
	_root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");

	_sceneManager = Ogre::Root::getSingletonPtr()->getSceneManager(
			"SceneManager");
	_sceneManager->addRenderQueueListener(new Ogre::OverlaySystem());
	// Transición al estado inicial.
	changeState(state);

	// Creation of the SFX and Tracks manager
	configureTracks();

	// Creación de toda la interfaz
	createInterface();

	// Creación de los overlays
	//createOverlay();

	// Bucle de rendering.
	_root->startRendering();
}

void GameManager::changeState(GameState* state) {
	// Limpieza del estado actual.
	if (!_states.empty()) {
		// exit() sobre el último estado.
		_states.top()->exit();
		// Elimina el último estado.
		_states.pop();
	}

	// Transición al nuevo estado.
	_states.push(state);
	// enter() sobre el nuevo estado.
	_states.top()->enter();
}

void GameManager::pushState(GameState* state) {
	// Pausa del estado actual.
	if (!_states.empty())
		_states.top()->pause();

	// Transición al nuevo estado.
	_states.push(state);
	// enter() sobre el nuevo estado.
	_states.top()->enter();
}

void GameManager::popState() {
	// Limpieza del estado actual.
	if (!_states.empty()) {
		_states.top()->exit();
		_states.pop();
	}

	// Vuelta al estado anterior.
	if (!_states.empty())
		_states.top()->resume();
}

void GameManager::loadResources() {
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	Ogre::ConfigFile::SectionIterator sI = cf.getSectionIterator();
	Ogre::String sectionstr, typestr, datastr;
	while (sI.hasMoreElements()) {
		sectionstr = sI.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = sI.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i) {
			typestr = i->first;
			datastr = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
					datastr, typestr, sectionstr);
		}
	}
}

bool GameManager::configure() {
	if (!_root->restoreConfig()) {
		if (!_root->showConfigDialog()) {
			return false;
		}
	}

	_renderWindow = _root->initialise(true, "Project Adventure");

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return true;
}

GameManager*
GameManager::getSingletonPtr() {
	return msSingleton;
}

GameManager&
GameManager::getSingleton() {
	assert(msSingleton);
	return *msSingleton;
}

// Las siguientes funciones miembro delegan
// el evento en el estado actual.
bool GameManager::frameStarted(const Ogre::FrameEvent& evt) {
	_inputMgr->capture();
	return _states.top()->frameStarted(evt);
}

bool GameManager::frameEnded(const Ogre::FrameEvent& evt) {
	_inputMgr->capture();
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(
			evt.timeSinceLastFrame);
	return _states.top()->frameEnded(evt);
}

bool GameManager::keyPressed(const OIS::KeyEvent &e) {
	_states.top()->keyPressed(e);
	return true;
}

bool GameManager::keyReleased(const OIS::KeyEvent &e) {
	_states.top()->keyReleased(e);
	return true;
}

bool GameManager::mouseMoved(const OIS::MouseEvent &e) {
	CEGUI::Vector2f mousePos =
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setPosition(
			CEGUI::Vector2f(e.state.X.abs, e.state.Y.abs));
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
			mousePos.d_x / float(e.state.width),
			mousePos.d_y / float(e.state.height));

	_states.top()->mouseMoved(e);
	return true;
}

bool GameManager::mousePressed(const OIS::MouseEvent &e,
		OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
			convertMouseButton(id));
	_states.top()->mousePressed(e, id);
	return true;
}

bool GameManager::mouseReleased(const OIS::MouseEvent &e,
		OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
			convertMouseButton(id));
	_states.top()->mouseReleased(e, id);
	return true;
}

void GameManager::changePlayState() {
	changeState(PlayState::getSingletonPtr());
}

void GameManager::createInterface() {
	//CEGUI
	_renderer = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(
			"DejaVuSans-12");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(
			"TaharezLook/MouseArrow");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow(
			"DefaultWindow", "Sheet");

	//Life label
	CEGUI::Window* lifeLabel =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Label", "lifeLabel");
	lifeLabel->setText("Life: ");
	lifeLabel->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	lifeLabel->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.9, 0)));

	//Event label
	CEGUI::Window* eventLabel =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Label", "eventLabel");
	eventLabel->setText("");
	eventLabel->setSize(
			CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.05, 0)));
	eventLabel->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.95, 0)));
	//Quit Intro button
	CEGUI::Window* quitIntroButton =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Button", "quitIntroButton");
	quitIntroButton->setText("Quit");
	quitIntroButton->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quitIntroButton->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.8, 0)));
	quitIntroButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::quitIntroState, this));

	//Quit Pause button
	CEGUI::Window* quitPauseButton =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Button", "quitPauseButton");
	quitPauseButton->setText("Quit");
	quitPauseButton->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quitPauseButton->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.4, 0)));
	quitPauseButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::quitPauseState, this));

	//Quit Win button
	CEGUI::Window* quitWinButton =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Button", "quitWinButton");
	quitWinButton->setText("Quit");
	quitWinButton->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quitWinButton->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.5, 0)));
	quitWinButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::quitWinState, this));

	//Quit lose button
	CEGUI::Window* quitLoseButton =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Button", "quitLoseButton");
	quitLoseButton->setText("Quit");
	quitLoseButton->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quitLoseButton->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.5, 0)));
	quitLoseButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::quitLoseState, this));

	//Restart Button
	CEGUI::Window* restartButton =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Button", "restartButton");
	restartButton->setText("Restart");
	restartButton->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	restartButton->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.3, 0)));
	restartButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::changeIntroState, this));
	//Play Button
	CEGUI::Window* playButton =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Button", "PlayButton");
	playButton->setText("Play");
	playButton->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	playButton->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.5, 0)));
	playButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::changePlayState, this));
	//Credits button
	CEGUI::Window* creditsButton =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Button", "creditsButton");
	creditsButton->setText("Credits");
	creditsButton->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	creditsButton->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.7, 0)));
	creditsButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::createCredits_window, this));

	//Credits Window
	CEGUI::Window* creditsWindow =
			CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
					"creditsWindow.layout");
	CEGUI::Window* cokButton = creditsWindow->getChild("cokButton");
	cokButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::closeCredits_window, this));

	//Scoreboard button
	CEGUI::Window* scoreboardButton =
			CEGUI::WindowManager::getSingleton().createWindow(
					"TaharezLook/Button", "scoreboardButton");
	scoreboardButton->setText("Scoreboard");
	scoreboardButton->setSize(
			CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	scoreboardButton->setPosition(
			CEGUI::UVector2(CEGUI::UDim(0.5 - 0.15 / 2, 0),
					CEGUI::UDim(0.6, 0)));
	scoreboardButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::createScoreboard_window,
					this));

	//ScoreBoard Window
	CEGUI::Window* scoreboardWindow =
			CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
					"scoreboardsWindow.layout");

	CEGUI::Window* sokButton = scoreboardWindow->getChild("sokButton");
	sokButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::closeScoreboard_window,
					this));

	//IntroduceName Window
	CEGUI::Window* introduceNameWindow =
			CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
					"introduceName.layout");

	CEGUI::Window* iokButton = introduceNameWindow->getChild("okButton");
	iokButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&GameManager::closeIntroduceName_window,
					this));

	quitPauseButton->setVisible(false);
	quitWinButton->setVisible(false);
	creditsWindow->setVisible(false);
	scoreboardWindow->setVisible(false);
	introduceNameWindow->setVisible(false);
	lifeLabel->setVisible(false);
	eventLabel->setVisible(false);
	restartButton->setVisible(false);
	quitLoseButton->setVisible(false);

	sheet->addChild(restartButton);
	sheet->addChild(lifeLabel);
	sheet->addChild(eventLabel);
	sheet->addChild(playButton);
	sheet->addChild(quitIntroButton);
	sheet->addChild(quitPauseButton);
	sheet->addChild(quitWinButton);
	sheet->addChild(quitLoseButton);
	sheet->addChild(creditsButton);
	sheet->addChild(creditsWindow);
	sheet->addChild(scoreboardButton);
	sheet->addChild(scoreboardWindow);
	sheet->addChild(introduceNameWindow);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

	CEGUI::Vector2f mousePos =
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
			-mousePos.d_x, -mousePos.d_y);
}

void GameManager::createOverlay() {

	OverlayManager* overlayManager = OverlayManager::getSingletonPtr();
	Ogre::Overlay *overlay = overlayManager->getByName("Info");
	// Show the overlay
	cout << overlay << endl;
	overlay->show();

}
void GameManager::createCredits_window() {
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setAlwaysOnTop(true);
}

void GameManager::closeCredits_window() {
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"creditsWindow")->setAlwaysOnTop(false);
}

void GameManager::createScoreboard_window() {
	string line;
	ifstream myfile("media/txt/scoreboard.txt");
	if (myfile.is_open()) {
		getline(myfile, line);
		CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
				"scoreboardWindow")->getChild("firstLabel")->setText(line);
		getline(myfile, line);
		CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
				"scoreboardWindow")->getChild("secondLabel")->setText(line);
		getline(myfile, line);
		CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
				"scoreboardWindow")->getChild("thirdLabel")->setText(line);
		getline(myfile, line);
		CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
				"scoreboardWindow")->getChild("fourthLabel")->setText(line);
		getline(myfile, line);
		CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
				"scoreboardWindow")->getChild("fifthLabel")->setText(line);
		myfile.close();
	}

	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setAlwaysOnTop(true);
}

void GameManager::closeScoreboard_window() {
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"scoreboardWindow")->setAlwaysOnTop(false);
}

void GameManager::createIntroduceName_window() {
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"introduceNameWindow")->setVisible(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"introduceNameWindow")->setAlwaysOnTop(true);
}

void GameManager::closeIntroduceName_window() {
	//5: 2 point/s by Edu
	string line;
	string line2;
	string line3;
	string line4;
	int i = 0;
	int position = 0;
	ifstream myfile("media/txt/scoreboard.txt");
	bool continue_ = false;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			if (line[0] - '0' <= PlayState::getSingletonPtr()->getPoints()
					&& continue_ == false) {
				position = i;
				continue_ = true;
			}
			i++;
		}
		myfile.close();
	}
	const char* name;
	name =
			CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
					"introduceNameWindow")->getChild("nameEditbox")->getText().c_str();
	char customline[80];
	sprintf(customline, "%d point/s by %s",
			PlayState::getSingletonPtr()->getPoints(), name);
	ifstream myfile2("media/txt/scoreboard.txt");

	cout << customline << endl;
	getline(myfile2, line);
	cout << line << endl;
	getline(myfile2, line2);
	cout << line2 << endl;
	getline(myfile2, line3);
	cout << line3 << endl;
	getline(myfile2, line4);
	cout << line4 << endl;
	ofstream myfile3("media/txt/scoreboard.txt");
	switch (position) {
	case 0:

		myfile3 << customline << "\n";
		myfile3 << line << "\n";
		myfile3 << line2 << "\n";
		myfile3 << line3 << "\n";
		myfile3 << line4 << "\n";

		break;

	case 1:
		myfile3 << line << "\n";
		myfile3 << customline << "\n";
		myfile3 << line2 << "\n";
		myfile3 << line3 << "\n";
		myfile3 << line4 << "\n";

		break;
	case 2:
		myfile3 << line << "\n";
		myfile3 << line2 << "\n";
		myfile3 << customline << "\n";
		myfile3 << line3 << "\n";
		myfile3 << line4 << "\n";

		break;
	case 3:
		myfile3 << line << "\n";
		myfile3 << line2 << "\n";
		myfile3 << line3 << "\n";
		myfile3 << customline << "\n";
		myfile3 << line4 << "\n";

		break;
	case 4:
		myfile3 << line << "\n";
		myfile3 << line2 << "\n";
		myfile3 << line3 << "\n";
		myfile3 << line4 << "\n";
		myfile3 << customline << "\n";
		break;
	}

	myfile3.close();
	myfile2.close();
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"introduceNameWindow")->setVisible(false);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
			"introduceNameWindow")->setAlwaysOnTop(false);
}

void GameManager::changeIntroState() {
	popState();
	changeState(IntroState::getSingletonPtr());
}
void GameManager::quitIntroState() {
	IntroState::getSingletonPtr()->introStateExit();
}

void GameManager::quitPauseState() {
	PauseState::getSingletonPtr()->pauseStateExit();
}

void GameManager::quitWinState() {
	WinState::getSingletonPtr()->winStateExit();
}

void GameManager::quitLoseState() {
	LoseState::getSingletonPtr()->LoseStateExit();
}

CEGUI::MouseButton GameManager::convertMouseButton(OIS::MouseButtonID id) {
	CEGUI::MouseButton ceguiId;
	switch (id) {
	case OIS::MB_Left:
		ceguiId = CEGUI::LeftButton;
		break;
	case OIS::MB_Right:
		ceguiId = CEGUI::RightButton;
		break;
	case OIS::MB_Middle:
		ceguiId = CEGUI::MiddleButton;
		break;
	default:
		ceguiId = CEGUI::LeftButton;
	}
	return ceguiId;
}

void GameManager::configureTracks() {
	// This is the song for the intro state
	initSDL();
	_pTrackManager = OGRE_NEW TrackManager;
	_pSoundFXManager = OGRE_NEW SoundFXManager;

	_mainTrack = _pTrackManager->load("maintheme.wav");
	_mainTrack->play();

	_simpleEffect = _pSoundFXManager->load("point.wav");
	_deathEffect = _pSoundFXManager->load("nightmare.wav");
	_jumpEffect = _pSoundFXManager->load("jump.wav");
	_shootEffect = _pSoundFXManager->load("shoot.wav");
	_winEffect = _pSoundFXManager->load("applause10.wav");
	_getPointEffect = _pSoundFXManager->load("getPoint.wav");
}

bool GameManager::initSDL() {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		return false;
	}
	// Llamar a  SDL_Quit al terminar.
	atexit(SDL_Quit);

	// Inicializando SDL mixer...
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
	MIX_DEFAULT_CHANNELS, 4096) < 0) {
		return false;
	}

	// Llamar a Mix_CloseAudio al terminar.
	atexit(Mix_CloseAudio);

	return true;
}

void GameManager::changeTrack(char* track) {
	_mainTrack->unload();
	_mainTrack = _pTrackManager->load(track);
	_mainTrack->play();
}

void GameManager::resumeTrack() {
	_mainTrack->play();
}
void GameManager::pauseTrack() {
	_mainTrack->pause();
}

void GameManager::playPointSound() {
	_simpleEffect->play();
}

void GameManager::playShootSound() {
	_shootEffect->play();
}

void GameManager::playJumpSound() {
	_jumpEffect->play();
}

void GameManager::playDeathSound() {
	_deathEffect->play();
}

void GameManager::playGetPointSound() {
	_getPointEffect->play();
}
void GameManager::playWinSound() {
	_winEffect->play();
}
