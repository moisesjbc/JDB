/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

    This file is part of JDB.

    JDB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JDB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JDB.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "level.hpp"
#include <tinyxml2.h>
#include <algorithm>

namespace jdb {

const float SANDWICHES_END_POINT = 0.0f;
const float DISTANCE_BETWEEN_SANDWICHES = 300.0f;


/***
 * 1. Construction
 ***/

Level::Level( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex )
    : GameState( window ),
      soundManager_( *soundManager ),
      levelIndex_( levelIndex ),
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


void Level::loadDangerData( unsigned int levelIndex )
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement* dangerXMLElement = nullptr;

    dangerGraphicsLibrary_ =
            std::unique_ptr< m2g::GraphicsLibrary >( new m2g::GraphicsLibrary( DATA_DIR_PATH + "/img/dangers/dangers.xml" ) );

    // Load the dangers data.
    document.LoadFile( (DATA_DIR_PATH + "/config/dangers.xml").c_str() );
    dangerXMLElement = ( document.RootElement() )->FirstChildElement( "danger" );

    dangerData.clear();
    while( dangerXMLElement ){
        if( static_cast< unsigned int >( dangerXMLElement->IntAttribute( "first_level" ) ) <= levelIndex ){
            dangerData.emplace_back( new DangerData( dangerXMLElement, *dangerGraphicsLibrary_, dangerData ) );
        }
        dangerXMLElement = dangerXMLElement->NextSiblingElement();
    }

    // Sort the vector of dangers with a descending order in their
    // base lines' widths.
    std::sort( dangerData.begin(),
                dangerData.end(),
                []( DangerDataPtr a, DangerDataPtr b ){
        return (a->baseLine.width > b->baseLine.width );
    });
}


void Level::initGUI()
{
    // Load the "gui" graphics library.
    m2g::GraphicsLibrary guiGraphicsLibrary( DATA_DIR_PATH + "/img/gui/gui.xml" );

    // Load the GUI sprites.
    guiSprites_.push_back( m2g::TileSpritePtr( new m2g::TileSprite( guiGraphicsLibrary.getTilesetByName( "health.png" ) ) ) );

    guiTilesets_.push_back( guiGraphicsLibrary.getTilesetByName( "time.png" ) );
    m2g::TileSpritePtr tileSprite =
            m2g::TileSpritePtr( new m2g::TileSprite( *( guiTilesets_.back() ) ) );
    tileSprite->move( 367.0f, 0.0f );
    guiSprites_.push_back( std::move( tileSprite ) );

    guiTilesets_.push_back( guiGraphicsLibrary.getTilesetByName( "score.png" ) );
    tileSprite =
            m2g::TileSpritePtr( new m2g::TileSprite( *( guiTilesets_.back() ) ) );
    tileSprite->move( 768.0f, 0.0f );
    guiSprites_.push_back( std::move( tileSprite ) );

    guiTilesets_.push_back( guiGraphicsLibrary.getTilesetByName( "tool_selector.png" ) );
    tileSprite =
            m2g::TileSpritePtr( new m2g::TileSprite( *( guiTilesets_.back() ) ) );
    tileSprite->move( 384.0f, 660.0f );
    guiToolSelector_ = tileSprite.get();
    guiSprites_.push_back( std::move( tileSprite ) );

    // Load the "tools" graphics library.
    m2g::GraphicsLibrary toolsGraphicsLibrary( DATA_DIR_PATH + "/img/tools/tools.xml" );

    // Load the player's tool.
    tool_ = ToolPtr( new Tool( toolsGraphicsLibrary.getAnimationDataByName( "tools.png" ), soundManager_ ) );

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
            quitLevel_ = true;
            exit( 0 );
        break;
        case sf::Event::MouseButtonPressed:{
            // Player clicked on screen.
            unsigned int hpBonus = 0;
            tool_->handleMouseButtonDown( sandwiches, levelScore_, hpBonus );
            // FIXME: Duplicated code.
            jacobHp_ += hpBonus;
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
                    guiToolSelector_->setTile( 0 );
                break;
                case sf::Keyboard::S:
                    tool_->setToolType( ToolType::EXTINGUISHER );
                    guiToolSelector_->setTile( 1 );
                break;
                case sf::Keyboard::D:
                    tool_->setToolType( ToolType::LIGHTER );
                    guiToolSelector_->setTile( 2 );
                break;
                case sf::Keyboard::F:
                    tool_->setToolType( ToolType::GAVEL );
                    guiToolSelector_->setTile( 3 );
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

    // Load the sandwiches, move them to their final positions and
    // populate them with dangers.
    sandwiches.clear();
    for( unsigned int i=0; i < N_SANDWICHES; i++ ){
        sandwiches.push_back(
                    std::unique_ptr< Sandwich >(
                        new Sandwich( sandwichData[0], &dangerData ) ) );

        sandwiches[i]->setPosition( 1024 + i * DISTANCE_BETWEEN_SANDWICHES, 410 );

        sandwiches[i]->populate( dangerData );
    }
    firstSandwich = 0;
    lastSandwich = sandwiches.size() - 1;

    resetLevelTime();

    conveyorBelt_.reset();

    // Present level intro to player.
    switchState( *levelIntro_ );

    resetLevelTime();
    resetClock();
}


/***
 * 5. Auxiliar methods
 ***/

int Level::getSeconds() const
{
    return levelTime_ / 1000;
}


unsigned int Level::levelIndex() const
{
    return levelIndex_;
}


unsigned int Level::nSandwiches() const
{
    return sandwiches.size();
}


/***
 * 6. GameState interface
 ***/

void Level::init()
{
    quitLevel_ = false;

    // Initialize the GUI.
    initGUI();

    // Load the required level.
    load( levelIndex_ );

    // Initialize GUI's texts.
    guiFont_.loadFromFile( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    healthText_.setFont( guiFont_ );
    healthText_.setCharacterSize( 50 );
    healthText_.setColor( sf::Color( 131, 60, 60, 255 ) );
    healthText_.setPosition( 75, 5 );

    timerText_.setFont( guiFont_ );
    timerText_.setCharacterSize( 50 );
    timerText_.setColor( sf::Color( 8, 31, 126, 255 ) );
    timerText_.setPosition( 450, 3 );

    scoreText_.setFont( guiFont_ );
    scoreText_.setCharacterSize( 50 );
    scoreText_.setColor( sf::Color( 11, 109, 36, 255 ) );
    scoreText_.setPosition( 785, 5 );

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

    unsigned int hpBonus = 0;
    tool_->handleMouseHover( sandwiches, levelScore_, hpBonus );
    // FIXME: Duplicated code.
    jacobHp_ += hpBonus;
    if( jacobHp_ > 130 ){
        jacobHp_ = 130;
    }
}


void Level::update( unsigned int ms )
{
    updateLevelTime( ms );

    unsigned int i;

    tool_->applyStun( sandwiches );

    // Game logic: Check if the first sandwich reached the
    // sandwiches' end point and, in that case, restart it.
    if( sandwiches[firstSandwich]->getBoundaryBox().left < SANDWICHES_END_POINT ){

        // Hurt Jacob! (muahahaha!)
        jacobHp_ -= sandwiches[firstSandwich]->getDamage();

        if( levelTime_ > 1000 ){
            // Repopulate the sandwich.
            sandwiches[firstSandwich]->populate( dangerData );

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
        sandwiches[i]->update( ms );
    }

    // Move the sandwiches
    // Conveyor belt's speed management.
    float speed = conveyorBelt_.getSpeed();
    for( i=0; i < sandwiches.size(); i++ ){
        sandwiches[i]->translate( -speed, 0.0f );
    }

    // Update the tool
    tool_->update( ms );

    if( victory() ){
        acumScore_ += levelScore_;
        levelIndex_++; // TODO: This should go inside "load()".
        load( levelIndex_ );
        reset();
        //init();
    }else if( defeat() ){
        reset();
    }else if( quitLevel_ ){
        requestStateExit();
    }
}



void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    unsigned int i;

    // Time management.
    int seconds, minutes;

    // Text rendering.
    char buffer[16];

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

    // Draw the GUI sprites.
    for( const m2g::TileSpritePtr& guiSprite : guiSprites_ ){
        target.draw( *guiSprite, states );
    }

    // Compute the current game time.
    seconds = levelTime_ / 1000;
    minutes = seconds / 60;
    seconds = seconds % 60;

    // Write Jacob's life, game time and score.
    sprintf( buffer, "%03d", (int)jacobHp_ );
    healthText_.setString( buffer );
    window_.draw( healthText_ );

    sprintf( buffer, "%02d:%02d", minutes, seconds );
    timerText_.setString( buffer );
    window_.draw( timerText_ );

    sprintf( buffer, "%08u", acumScore_ + levelScore_ );
    scoreText_.setString( buffer );
    window_.draw( scoreText_ );
}


void Level::pause()
{
}


void Level::resume()
{
    window_.setMouseCursorVisible( false );
}


} // namespace jdb
