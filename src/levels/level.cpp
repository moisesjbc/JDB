/***
    Copyright 2013 - 2016 Moises J. Bonilla Caraballo (moisesjbc)

    This file is part of sandwiches-game.

    sandwiches-game is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    sandwiches-game is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with sandwiches-game.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include <levels/level.hpp>
#include <tinyxml2.h>
#include <algorithm>
#include <game_states/end_of_campaign_screen.hpp>
#include <game_states/game_over_screen.hpp>
#include <profiles/profile_json_parser.hpp>
#include <boost/filesystem.hpp>
#include <dangers/danger_data_parser.hpp>
#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING

INITIALIZE_EASYLOGGINGPP

namespace jdb {

/***
 * Construction and destruction
 ***/

Level::Level( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex, Profile& playerProfile )
    : GameState( window ),
      levelScore_( 0 ),
      soundManager_( *soundManager ),
      levelIndex_( levelIndex ),
      playerProfile_(playerProfile),
      difficultyFactor_(1.0f)
{
    switch(playerProfile_.gameDifficulty()){
        case GameDifficulty::EASY:
            difficultyFactor_ = 0.75f;
        break;
        case GameDifficulty::NORMAL:
            difficultyFactor_ = 1.0f;
        break;
        default:
            difficultyFactor_ = 1.25f;
        break;
    }
}


/***
 * Getters
 ***/

unsigned int Level::levelIndex() const
{
    return levelIndex_;
}


unsigned int Level::score() const
{
    return levelScore_;
}


unsigned int Level::toolIndex() const
{
    return tool_->index();
}


int Level::jacobHp() const
{
    return jacobHp_;
}


float Level::difficultyFactor() const
{
    return difficultyFactor_;
}


/***
 * Drawable interface
 ***/

void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Background
    window_.clear(sf::Color(0xDC, 0xF1, 0xF1, 0xFF));
    for(const m2g::TileSpritePtr& backgroundSprite : backgroundSprites){
        target.draw(*backgroundSprite, states);
    }

    // Sandwiches
    target.draw(*sandwichesManager_, states);

    // Front elements
    target.draw(*grinderFront, states);
    target.draw(*tool_, states);
    target.draw(*levelUI_, states);
}


/***
 * Player profile management (public)
 ***/

void Level::savePlayerProfile(Profile &playerProfile)
{
    boost::filesystem::path savegamePath(SAVEGAME_PATH);
    LOG(INFO) << "Saving player profile to [" + savegamePath.string() + "]";
    if(!boost::filesystem::exists(savegamePath.parent_path())){
        boost::filesystem::create_directory(savegamePath.parent_path());
    }

    ProfileJSONParser profileParser;
    profileParser.writeToJSON(playerProfile, savegamePath.string());
}


/***
 * Loading
 ***/

bool Level::load(unsigned int levelIndex)
{
    tinyxml2::XMLDocument xmlFile;
    xmlFile.LoadFile( (DATA_DIR_PATH + "/config/levels.xml").c_str() );

    LOG(INFO) << "Loading level configuration ...";
    tinyxml2::XMLElement* levelNode = getLevelXmlNode(xmlFile, levelIndex);
    // If the index XML node doesn't exist, return false.
    if( levelNode == nullptr ){
        return false;
    }
    LOG(INFO) << "Loading level configuration ...OK";

    // Load the sandwiches data.
    LOG(INFO) << "Loading sandwich data ...";
    std::vector<SandwichDataPtr> sandwichData = loadSandwichData();
    LOG(INFO) << "Loading sandwich data ...OK";

    // Load the dangers data.
    LOG(INFO) << "Loading danger data ...";
    tinyxml2::XMLElement* dangersXmlNode =
            (tinyxml2::XMLElement*)levelNode->FirstChildElement("dangers");
    std::map<std::string, float> dangersRatios;
    std::vector<std::string> newDangersIDs;
    std::unique_ptr<std::vector<DangerDataPtr>> dangerData(new std::vector<DangerDataPtr>);
    std::unique_ptr< m2g::GraphicsLibrary > dangerGraphicsLibrary =
        std::unique_ptr< m2g::GraphicsLibrary >( new m2g::GraphicsLibrary( DATA_DIR_PATH + "/img/dangers/dangers.xml" ) );
    loadDangerData(dangersXmlNode, dangersRatios, newDangersIDs, *dangerData, *dangerGraphicsLibrary);
    LOG(INFO) << "Loading danger data ...OK";

    int nDangers = 0;
    if(strcmp(dangersXmlNode->Attribute("number"), "unlimited") == 0){
        nDangers = UNLIMITED_DANGERS;
    }else{
        nDangers = dangersXmlNode->IntAttribute("number");
    }
    std::unique_ptr<DangersCounter> dangersCounter(
                new DangersCounter(
                    nDangers,
                    dangersRatios));


    // Get the conveyor belt parameters.
    conveyorBelt_.load( (tinyxml2::XMLElement*)levelNode->FirstChildElement( "speed" ), difficultyFactor() );

    sandwichesManager_ =
            std::unique_ptr<SandwichesManager>(
                new SandwichesManager(sandwichData, std::move(dangerData), std::move(dangersCounter), std::move(dangerGraphicsLibrary), conveyorBelt_));

    LOG(INFO) << "Creating level intro ...";
    levelIntro_ = std::move(generateLevelIntro(newDangersIDs, levelNode->FirstChildElement( "level_book" )));
    LOG(INFO) << "Creating level intro ...OK";

    return true;
}


std::vector<SandwichDataPtr> Level::loadSandwichData()
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement* sandwichXMLElement = nullptr;

    // Load the sandwiches data.
    m2g::GraphicsLibrary graphicsLibrary( DATA_DIR_PATH + "/img/sandwiches/sandwiches.xml" );

    // Load the dangers data.
    document.LoadFile( (DATA_DIR_PATH + "/config/sandwiches.xml").c_str() );
    sandwichXMLElement = ( document.RootElement() )->FirstChildElement( "sandwich" );

    std::vector<SandwichDataPtr> sandwichData;
    while( sandwichXMLElement ){
        sandwichData.emplace_back( new SandwichData( sandwichXMLElement, graphicsLibrary ) );

        sandwichXMLElement = sandwichXMLElement->NextSiblingElement();
    }
    return sandwichData;
}


void Level::loadDangerData(
            tinyxml2::XMLElement* dangersXmlNode,
            std::map<std::string, float>& dangersRatios,
            std::vector<std::string>& newDangersIDs,
            std::vector<DangerDataPtr>& dangerData,
            const m2g::GraphicsLibrary& dangersGraphicsLibrary )
{
    tinyxml2::XMLElement* dangerXmlNode =
        dangersXmlNode->FirstChildElement("danger");
    dangersRatios.clear();
    std::vector<std::string> dangersIDs;
    newDangersIDs.clear();
    while(dangerXmlNode != nullptr){
        dangersIDs.push_back(dangerXmlNode->GetText());
        dangersRatios[dangerXmlNode->GetText()] =
                dangerXmlNode->FloatAttribute("ratio");
        const char* present_danger_to_player =
                dangerXmlNode->Attribute("present_to_player");
        if(present_danger_to_player != nullptr && !strcmp(present_danger_to_player, "true")){
            newDangersIDs.push_back(dangerXmlNode->GetText());
        }
        dangerXmlNode = dangerXmlNode->NextSiblingElement("danger");
    }

    // Load the dangers data from config file.
    DangerDataParser dangerDataParser;
    dangerDataParser.LoadDangersDataByName(
        (DATA_DIR_PATH + "/config/dangers.json").c_str(),
        dangersIDs,
        dangersGraphicsLibrary,
        dangerData);
}


/***
 * Main loop
 ***/

void Level::handleUserInput( const sf::Event& event, SandwichesVector& sandwiches )
{
    switch( event.type ){
        case sf::Event::Closed:
            // Player wants to exit the game.
            requestGameExit();
        break;
        case sf::Event::MouseButtonPressed:{
            // Player clicked on screen.
            tool_->handleMouseButtonDown(sandwiches, jacobHp_, levelScore_, *sandwichesManager_->dangerGraphicsLibrary_, difficultyFactor());
        }break;
        case sf::Event::MouseButtonReleased:
            tool_->handleMouseButtonUp();
        break;
        case sf::Event::KeyPressed:
            // Player pressed a key. If the key pressed is
            // ESCAPE we exit the game.
            switch( event.key.code ){
                case sf::Keyboard::Escape:{
                    PauseMenu pauseMenu( window_, *this );
                    if( switchState( pauseMenu ) == RETURN_TO_MAIN_MENU_REQUESTED ){
                        requestStateExit( RETURN_TO_MAIN_MENU_REQUESTED );
                    }
                }break;
                case sf::Keyboard::A:
                    tool_->setToolType( ToolType::HAND );
                break;
                case sf::Keyboard::S:
                    tool_->setToolType( ToolType::EXTINGUISHER );
                break;
                case sf::Keyboard::D:
                    tool_->setToolType( ToolType::LIGHTER );
                break;
                case sf::Keyboard::F:
                    tool_->setToolType( ToolType::GAVEL );
                break;
                default:
                break;
            }
        break;
        case sf::Event::MouseMoved:
            // Player wants to move the mouse / tool.
            tool_->setPosition( event.mouseMove.x, event.mouseMove.y );
        break;
        default:
        break;
    }
}


void Level::reset()
{
    levelScore_ = 0;

    // Initialize jacob's life and the sandwich indicators.
    jacobHp_ = 100;

    sandwichesManager_->reset();

    conveyorBelt_.reset();

    resetClock();

    levelUI_->update();

    // Present level intro to player.
    switchState( *levelIntro_ );
}


/***
 * GameState interface
 ***/

void Level::init()
{
    // Load the required level.
    if( load( levelIndex_ ) == false ){
        throw std::runtime_error( "Couldn't load level " + std::to_string( levelIndex_ ) );
    }

    // Initialize the GUI.
    initGUI();

    // Load all the needed tilesets and animations (the graphics for
    // dangers and sandwiches are loaded in the methods "loadDangers" and
    // "loadSandwiches".
    m2g::GraphicsLibrary graphicsLibrary( DATA_DIR_PATH + "/img/background/background.xml" );

    // Load the background sprites
    backgroundSprites.push_back(
                m2g::TileSpritePtr(
                    new m2g::TileSprite(
                        std::move( graphicsLibrary.getTilesetByName( "grinder_back.png" ) ) ) ) );
    backgroundSprites.back()->move( 0.0f, -256.0f );

    backgroundSprites.push_back(
                m2g::TileSpritePtr(
                    new m2g::TileSprite(
                        std::move( graphicsLibrary.getTilesetByName( "conveyor_belt.png" ) ) ) ) );
    backgroundSprites.back()->move( 0.0, 256.0f );

    grinderFront =
            m2g::TileSpritePtr(
                new m2g::TileSprite( graphicsLibrary.getTilesetByName( "grinder_front.png" ) ) );
    grinderFront->move( 0.0f, -256.0f );

    reset();
}


void Level::handleEvents()
{
    sf::Time t0, t1;
    sf::Event event;

    // Handle user input.
    sf::Clock clock;
    t0 = t1 = clock.getElapsedTime();
    while( static_cast< unsigned int >( (t1 - t0).asMilliseconds() ) < REFRESH_TIME ){
        if( window_.pollEvent( event ) != 0 ){
            handleUserInput( event, sandwichesManager_->sandwiches() );
        }
        t1 = clock.getElapsedTime();
    }

    tool_->handleMouseHover( sandwichesManager_->sandwiches(), jacobHp_, levelScore_, *sandwichesManager_->dangerGraphicsLibrary_, difficultyFactor() );
}


void Level::update( unsigned int ms )
{
    LOG(INFO) << "Level::update()";

    tool_->applyStun( sandwichesManager_->sandwiches() );

    sandwichesManager_->update(ms, jacobHp_, levelScore_, difficultyFactor());
    if( jacobHp_ > MAX_JACOB_LIFE ){
        jacobHp_ = MAX_JACOB_LIFE;
    }

    conveyorBelt_.update( ms );

    // Update the tool
    tool_->update( ms );

    levelUI_->update();

    if( victory() ){
        updateAndSavePlayerProfile(playerProfile_);
        levelIndex_++;
        if( load( levelIndex_ ) ){
            reset();
        }else{
            std::unique_ptr<EndOfCampaignScreen> endOfCampaignScreen( new EndOfCampaignScreen( window_, *this ) );
            switchState( *endOfCampaignScreen );
            requestStateExit();
        }
        //init();
    }else if( defeat() ){
        updateAndSavePlayerProfile(playerProfile_);
        std::unique_ptr<GameOverScreen> gameOverScreen( new GameOverScreen(window_, *this) );
        if( switchState(*gameOverScreen) == RETURN_TO_MAIN_MENU_REQUESTED ){
            requestStateExit(RETURN_TO_MAIN_MENU_REQUESTED);
        }
        reset();
    }
}


void Level::pause()
{
    tool_->setActive(false);
}


void Level::resume()
{
    window_.setMouseCursorVisible( false );
}


void Level::cleanUp()
{
    updateAndSavePlayerProfile(playerProfile_);
}


/***
 * Private initializations
 ***/

void Level::initGUI()
{
    jacobHp_ = 100;

    // Load the "gui" graphics library.
    m2g::GraphicsLibrary guiGraphicsLibrary( DATA_DIR_PATH + "/img/gui/gui.xml" );

    // Load the "tools" graphics library.
    m2g::GraphicsLibrary toolsGraphicsLibrary( DATA_DIR_PATH + "/img/tools/tools.xml" );

    // Load the player's tool.
    tool_ = ToolPtr( new Tool( toolsGraphicsLibrary.getAnimationDataByName( "tools.png" ), soundManager_ ) );

    levelUI_ = std::move(generateLevelUI(guiGraphicsLibrary));

    levelUI_->update();

    window_.setMouseCursorVisible( false );
}


/***
 * Player profile management (private)
 ***/

void Level::updateAndSavePlayerProfile(Profile &playerProfile) const
{
    if(updatePlayerProfile(playerProfile)){
        savePlayerProfile(playerProfile);
    }
}

} // namespace jdb
