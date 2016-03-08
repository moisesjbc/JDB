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
#include <game_states/end_of_demo_screen.hpp>
#include <game_states/game_over_screen.hpp>
#include <profiles/profile_json_parser.hpp>
#include <boost/filesystem.hpp>
#include <dangers/danger_data_parser.hpp>
#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING

INITIALIZE_EASYLOGGINGPP

namespace jdb {

const float SANDWICHES_END_POINT = 0.0f;
const float DISTANCE_BETWEEN_SANDWICHES = 300.0f;


/***
 * 1. Construction
 ***/

Level::Level( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex, Profile& playerProfile )
    : GameState( window ),
      soundManager_( *soundManager ),
      levelIndex_( levelIndex ),
      playerProfile_(playerProfile),
      acumScore_( 0 ),
      levelScore_( 0 )
{}


/***
 * 3. Level Loading
 ***/

void Level::loadSandwichData()
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement* sandwichXMLElement = nullptr;

    // Load the sandwiches data.
    m2g::GraphicsLibrary graphicsLibrary( DATA_DIR_PATH + "/img/sandwiches/sandwiches.xml" );

    // Load the dangers data.
    document.LoadFile( (DATA_DIR_PATH + "/config/sandwiches.xml").c_str() );
    sandwichXMLElement = ( document.RootElement() )->FirstChildElement( "sandwich" );
    sandwichData.clear();
    while( sandwichXMLElement ){
        sandwichData.emplace_back( new SandwichData( sandwichXMLElement, graphicsLibrary ) );

        sandwichXMLElement = sandwichXMLElement->NextSiblingElement();
    }
}


void Level::loadDangerData(
            tinyxml2::XMLElement* dangersXmlNode,
            std::map<std::string, float>& dangersRatios,
            std::vector<std::string>& newDangersIDs )
{
    dangerGraphicsLibrary_ =
            std::unique_ptr< m2g::GraphicsLibrary >( new m2g::GraphicsLibrary( DATA_DIR_PATH + "/img/dangers/dangers.xml" ) );

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

    // FIXME: don't use dangers counter in survival level.
    dangersCounter_ =
        std::unique_ptr<DangersCounter>(
            new DangersCounter(
                dangersXmlNode->IntAttribute("number"),
                dangersRatios));

    // Load the dangers data from config file.
    DangerDataParser dangerDataParser;
    dangerDataParser.LoadDangersDataByName(
        (DATA_DIR_PATH + "/config/dangers.json").c_str(),
        dangersIDs,
        *dangerGraphicsLibrary_,
        dangerData);
}


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
 * 4. Main loop
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
            tool_->handleMouseButtonDown(sandwiches, jacobHp_, levelScore_, *dangerGraphicsLibrary_);
            // FIXME: Duplicated code.
            if( jacobHp_ > 130 ){
                jacobHp_ = 130;
            }
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

    nDangersRemoved_ = 0;

    // Initialize jacob's life and the sandwich indicators.
    jacobHp_ = 100;

    dangersCounter_->reset();

    // Load the sandwiches, move them to their final positions and
    // populate them with dangers.
    sandwiches.clear();
    for( unsigned int i=0; i < N_SANDWICHES; i++ ){
        sandwiches.push_back(
                    std::unique_ptr< Sandwich >(
                        new Sandwich( sandwichData[0], &dangerData ) ) );

        sandwiches[i]->setPosition( 1024 + i * DISTANCE_BETWEEN_SANDWICHES, 410 );

        sandwiches[i]->populate( dangerData, dangersCounter_.get() );
    }
    firstSandwich = 0;
    lastSandwich = sandwiches.size() - 1;

    conveyorBelt_.reset();

    resetClock();

    levelUI_->update();

    // Present level intro to player.
    switchState( *levelIntro_ );
}


/***
 * 5. Auxiliar methods
 ***/

unsigned int Level::levelIndex() const
{
    return levelIndex_;
}

unsigned int Level::score() const
{
    return levelScore_;
}


/***
 * 6. GameState interface
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
            handleUserInput( event, sandwiches );
        }
        t1 = clock.getElapsedTime();
    }

    tool_->handleMouseHover( sandwiches, jacobHp_, levelScore_, *dangerGraphicsLibrary_ );
    // FIXME: Duplicated code.
    if( jacobHp_ > 130 ){
        jacobHp_ = 130;
    }
}


void Level::update( unsigned int ms )
{
    LOG(INFO) << "Level::update()";

    unsigned int i;

    tool_->applyStun( sandwiches );

    // Game logic: Check if the first sandwich reached the
    // sandwiches' end point and, in that case, restart it.
    if( sandwiches[firstSandwich]->getBoundaryBox().left < SANDWICHES_END_POINT ){
        // Hurt Jacob! (muahahaha!)
        jacobHp_ -= sandwiches[firstSandwich]->getDamage();

        // Decrease the counter for the dangers of the sandwich.
        if(dangersCounter_ != nullptr){
            nDangersRemoved_ += sandwiches[firstSandwich]->nDangers();
        }

        if(dangersCounter_->nDangers() > 0){
            // Repopulate the sandwich.
            sandwiches[firstSandwich]->populate( dangerData, dangersCounter_.get() );

            // Translate the sandwich behind the last one.
            sandwiches[firstSandwich]->translate(
                        sandwiches[lastSandwich]->getBoundaryBox().left
                        - sandwiches[firstSandwich]->getBoundaryBox().left
                        + DISTANCE_BETWEEN_SANDWICHES,
                        0.0f );

            // Change the indices for the first and last sandwiches.
            firstSandwich = (firstSandwich + 1) % sandwiches.size();
            lastSandwich = (lastSandwich + 1) % sandwiches.size();
        }else{
            SandwichesVector::iterator sandwichIt = sandwiches.begin();
            std::advance( sandwichIt, firstSandwich );
            sandwiches.erase( sandwichIt );
            if( sandwiches.size() ){
                firstSandwich = (firstSandwich + 1) % sandwiches.size();
            }
        }
    }

    conveyorBelt_.update( ms );

    // Update the sandwiches
    for( i=0; i < sandwiches.size(); i++ ){
        sandwiches[i]->update( ms, jacobHp_, levelScore_, *dangerGraphicsLibrary_ );
    }

    // Move the sandwiches
    // Conveyor belt's speed management.
    float speed = conveyorBelt_.getSpeed();
    for( i=0; i < sandwiches.size(); i++ ){
        sandwiches[i]->translate( -speed, 0.0f );
    }

    // Update the tool
    tool_->update( ms );

    levelUI_->update();

    if( victory() ){
        acumScore_ += levelScore_;
        levelIndex_++; // TODO: This should go inside "load()".
        updateAndSavePlayerProfile(playerProfile_);
        if( load( levelIndex_ ) ){
            reset();
        }else{
            std::unique_ptr<EndOfDemoScreen> endOfDemoScreen( new EndOfDemoScreen( window_, *this ) );
            switchState( *endOfDemoScreen );
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



void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    unsigned int i;

    window_.clear( sf::Color( 0xDC, 0xF1, 0xF1, 0xFF ) );

    // Draw the background
    for( const m2g::TileSpritePtr& backgroundSprite : backgroundSprites ){
        target.draw( *backgroundSprite, states );
    }

    // Draw the sandwiches
    for( i=0; i < sandwiches.size(); i++ ){
        target.draw( *( sandwiches[i] ), states );
    }

    // Draw the grinder's front.
    target.draw( *grinderFront, states );

    // Draw the tool.
    target.draw( *tool_, states );

    target.draw(*levelUI_, states);
}


void Level::pause()
{
}


void Level::resume()
{
    window_.setMouseCursorVisible( false );
}


void Level::cleanUp()
{
    updateAndSavePlayerProfile(playerProfile_);
}


void Level::savePlayerProfile(Profile &playerProfile) const
{
    boost::filesystem::path savegamePath(SAVEGAME_PATH);
    LOG(INFO) << "Saving player profile to [" + savegamePath.string() + "]";
    if(!boost::filesystem::exists(savegamePath.parent_path())){
        boost::filesystem::create_directory(savegamePath.parent_path());
    }

    ProfileJSONParser profileParser;
    profileParser.writeToJSON(playerProfile, savegamePath.string());
}


void Level::updateAndSavePlayerProfile(Profile &playerProfile) const
{
    if(updatePlayerProfile(playerProfile)){
        savePlayerProfile(playerProfile);
    }
}

} // namespace jdb
