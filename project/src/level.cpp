/***
    Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)

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
#include "dependencies/m2g/src/dependencies/tinyxml2/tinyxml2.h"

namespace jdb {

const unsigned int N_SANDWICHES = 5;
const float SANDWICHES_END_POINT = 0.0f;
const float DISTANCE_BETWEEN_SANDWICHES = 300.0f;
const unsigned int N_DANGERS = N_SANDWICHES * 3;


/***
 * 1. Initialization and destruction
 ***/

Level::Level( SDL_Window* window_, SDL_Surface* screen_, unsigned int screenWidth, unsigned int screenHeight )
    : window( window_ ),
      screen( screen_ )
{
    projectionMatrix = glm::ortho( 0.0f, (float)screenWidth, (float)screenHeight, 0.0f, 1.0f, -1.0f );

    // Initialize the GUI.
    initGUI();
}


void Level::initGUI()
{
    // Load the "tools" graphics library.
    graphicsLibrary_.loadAll( "data/img/tools" );

    // Load the player's tool.
    tool_ = ToolPtr( new Tool( graphicsLibrary_.getAnimationData( "tools.png" ) ) );
}


void Level::runSurvivalLevel( unsigned int index )
{
    tinyxml2::XMLNode* levelNode = nullptr;
    tinyxml2::XMLElement* levelElement = nullptr;

    unsigned int i = 0;
    float initialSpeed;
    float speedStep;
    unsigned int timeLapse;

    // Set the level type.
    levelType_ = LevelType::SURVIVAL;

    // Open the levels configuration file.
    xmlFile.LoadFile( "data/config/levels.xml" );

    // Iterate over the survival level XML nodes until de number index.
    levelNode = ( xmlFile.FirstChildElement( "levels" )->FirstChildElement( "survival_levels" )->FirstChildElement( "survival_level" ) );
    while( levelNode && ( i < index ) ){
        levelNode = levelNode->NextSiblingElement( "survival_level" );
        i++;
    }

    // If the index XML node doesn't exist, throw an exception.
    if( i < index ){
        throw std::runtime_error( "ERROR: Survival level not found" );
    }

    // Load the sandwiches data.
    loadSandwichData();

    // Load the dangers data.
    loadDangerData();

    // Get the level parameters.
    levelElement = (tinyxml2::XMLElement*)levelNode->FirstChildElement( "speed" );
    initialSpeed = levelElement->FloatAttribute( "initial" );
    speedStep = levelElement->FloatAttribute( "step" );
    timeLapse = (unsigned int)( levelElement->IntAttribute( "time_lapse" ) );

    // Execute the main loop.
    mainLoop( initialSpeed, speedStep, timeLapse, std::bind( &Level::isSurvivalLevelFinished, this ) );
}


void Level::runCampaignLevel( unsigned int index )
{
    // Set the level type.
    levelType_ = LevelType::CAMPAIGN;

    // TODO: Change this when campaign level are fully implemented.
    tinyxml2::XMLNode* levelNode = nullptr;
    tinyxml2::XMLElement* levelElement = nullptr;

    unsigned int i = 0;
    float initialSpeed;
    float speedStep;
    unsigned int timeLapse;

    // Open the levels configuration file.
    xmlFile.LoadFile( "data/config/levels.xml" );

    // Iterate over the survival level XML nodes until de number index.
    levelNode = ( xmlFile.FirstChildElement( "levels" )->FirstChildElement( "survival_levels" )->FirstChildElement( "survival_level" ) );
    while( levelNode && ( i < index ) ){
        levelNode = levelNode->NextSiblingElement( "survival_level" );
        i++;
    }

    // If the index XML node doesn't exist, throw an exception.
    if( i < index ){
        throw std::runtime_error( "ERROR: Survival level not found" );
    }

    // Load the sandwiches data.
    loadSandwichData();

    // Load the dangers data.
    loadDangerData();

    // Get the level parameters.
    levelElement = (tinyxml2::XMLElement*)levelNode->FirstChildElement( "speed" );
    initialSpeed = levelElement->FloatAttribute( "initial" );
    speedStep = levelElement->FloatAttribute( "step" );
    timeLapse = (unsigned int)( levelElement->IntAttribute( "time_lapse" ) );

    // Execute the main loop.
    mainLoop( initialSpeed, speedStep, timeLapse, std::bind( &Level::isCampaignLevelFinished, this ) );
}


void Level::mainLoop( float initialSpeed, float speedStep, unsigned int timeLapse, FinishPredicate finishPredicate )
{
    unsigned int i;

    // Variables used for sandwich reseting.
    unsigned int firstSandwich;
    unsigned int lastSandwich;

    // Conveyor belt's speed management.
    std::mutex speedMutex;
    float speed = initialSpeed;

    // Time management.
    Uint32 t0;
    Uint32 t1;
    int seconds, minutes;

    // Event handling.
    SDL_Event event;
    bool quit = false;
    bool userResponded = false;

    // Text rendering.
    m2g::TextRenderer textRenderer;
    char buffer[16];
    const SDL_Color HEALTH_FONT_COLOR = { 131, 60, 60, 255 };
    const SDL_Color TIMER_FONT_COLOR = { 8, 31, 126, 255 };

    // GUI sprites.
    m2g::DrawablesSet guiSprites;
    m2g::SpritePtr guiToolSelector;

    // Background sprites.
    m2g::DrawablesSet backgroundSprites;
    m2g::SpritePtr grinderFront;

    // Text sprites.
    m2g::SpritePtr startAgainText;

    // Level countdown (a countdown of zero means a survival level).
    int countdown;

    // Set the level countdown.
    if( levelType_ == LevelType::CAMPAIGN ){
        countdown = 60;
    }else{
        countdown = 0;
    }

    try
    {
        // Initialize the text renderer.
        coutMutex.lock();
        std::cout << "loadFont: " << textRenderer.loadFont( "data/fonts/LiberationSans-Bold.ttf", 50, HEALTH_FONT_COLOR ) << std::endl;
        std::cout << "loadFont: " << textRenderer.loadFont( "data/fonts/LiberationSans-Bold.ttf", 50, TIMER_FONT_COLOR ) << std::endl;
        coutMutex.unlock();

        // Create the sprite pointers.
        Sandwich* sandwiches[N_SANDWICHES];

        // Make the cursor invisible.
        SDL_ShowCursor( SDL_DISABLE );

        // Load all the needed tilesets and animations (the graphics for
        // dangers and sandwiches are loaded in the methods "loadDangers" and
        // "loadSandwiches".
        graphicsLibrary_.loadAll( "data/img/background" );
        graphicsLibrary_.loadAll( "data/img/gui" );

        // Load the GUI sprites.
        guiSprites.addSprite( graphicsLibrary_.getTileset( "health.png" ) );
        guiSprites.addSprite( graphicsLibrary_.getTileset( "time.png" ), 367.0f, 0.0f );
        guiToolSelector = guiSprites.addSprite( graphicsLibrary_.getTileset( "tool_selector.png" ), 384.0f, 660.0f );

        // Load the background sprites
        backgroundSprites.addSprite( graphicsLibrary_.getTileset( "grinder_back.png" ), 0.0f, -256.0f );
        backgroundSprites.addSprite( graphicsLibrary_.getTileset( "conveyor_belt.png" ), 0.0, 256.0f );
        grinderFront = m2g::SpritePtr( new m2g::Sprite( graphicsLibrary_.getTileset( "grinder_front.png" ) ) );
        grinderFront->moveTo( 0.0f, -256.0f );

        // Restarting loop. Keep restarting this level until the player tell us
        // to stop.
        quit = false;
        while( !quit ){
            // Initialize the variables used for time management.
            t0 = 0;
            t1 = 0;

            // Initialize jacob's life and the sandwich indicators.
            jacobHp_ = 100;
            firstSandwich = 0;
            lastSandwich = N_SANDWICHES - 1;

            // Start the timer and make it increase the speed every timeLapse
            // seconds.
            timer.init( timeLapse, [&](){
                speedMutex.lock();
                speed += speedStep;
                speedMutex.unlock();

                coutMutex.lock();
                std::cout << "New speed! (" << speed << ")" << std::endl;
                coutMutex.unlock();
            }, countdown );

            // Load the sandwiches, move them to their final positions and
            // populate them with dangers.
            for( i=0; i < N_SANDWICHES; i++ ){
                sandwiches[i] = new Sandwich( sandwichData[0], &dangerData );

                sandwiches[i]->moveTo( 1024 + i * DISTANCE_BETWEEN_SANDWICHES, 410 );

                sandwiches[i]->populate( dangerData );
            }

            // Main loop: run the game until player ask us or the finish
            // predicate is true.
            while( !quit && !finishPredicate() ){
                // Clear screen.
                glClear ( GL_COLOR_BUFFER_BIT );

                // Handle user input.
                t0 = SDL_GetTicks();
                while( (t1 - t0) < REFRESH_TIME ){
                    if( SDL_PollEvent( &event ) != 0 ){
                        switch( event.type ){
                            case SDL_QUIT:
                                // Player wants to exit the game.
                                quit = true;
                            break;
                            case SDL_MOUSEBUTTONDOWN:
                                // Player clicked on screen.
                                tool_->handleMouseButtonDown( sandwiches, N_SANDWICHES );
                            break;
                            case SDL_MOUSEBUTTONUP:
                                tool_->handleMouseButtonUp();
                            break;
                            case SDL_KEYDOWN:
                                // Player pressed a key. If the key pressed is
                                // ESCAPE we exit the game.
                                switch( event.key.keysym.sym ){
                                    case SDLK_ESCAPE:
                                        quit = true;
                                    break;
                                    case SDLK_a:
                                        tool_->setToolType( ToolType::HAND );
                                        guiToolSelector->setTile( 0 );
                                    break;
                                    case SDLK_s:
                                        tool_->setToolType( ToolType::EXTINGUISHER );
                                        guiToolSelector->setTile( 1 );
                                    break;
                                    case SDLK_d:
                                        tool_->setToolType( ToolType::LIGHTER );
                                        guiToolSelector->setTile( 2 );
                                    break;
                                    case SDLK_f:
                                        tool_->setToolType( ToolType::GAVEL );
                                        guiToolSelector->setTile( 3 );
                                    break;
                                }
                            break;
                            case SDL_MOUSEMOTION:
                                // Player wants to move the mouse / tool.
                                tool_->moveTo( event.motion.x, event.motion.y );
                            break;
                        }
                    }
                    t1 = SDL_GetTicks();
                }
                t0 = SDL_GetTicks();
                t1 = SDL_GetTicks();


                tool_->handleMouseHover( sandwiches, N_SANDWICHES );


                // Game logic: Check if the first sandwich reached the
                // sandwiches' end point and, in that case, restart it.
                if( sandwiches[firstSandwich]->getX() < SANDWICHES_END_POINT ){

                    // Hurt Jacob! (muahahaha!)
                    jacobHp_ -= sandwiches[firstSandwich]->getDamage();

                    // Repopulate the sandwich.
                    sandwiches[firstSandwich]->populate( dangerData );

                    // Translate the sandwich behind the last one.
                    sandwiches[firstSandwich]->translate(
                                sandwiches[lastSandwich]->getX()
                                - sandwiches[firstSandwich]->getX()
                                + DISTANCE_BETWEEN_SANDWICHES,
                                0.0f );

                    // Change the indices for the first and last sandwiches.
                    firstSandwich = (firstSandwich + 1) % N_SANDWICHES;
                    lastSandwich = (lastSandwich + 1) % N_SANDWICHES;
                }

                // Clear screen.
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                // Bind the tileset's buffer.
                m2g::Tileset::bindBuffer();

                // Draw the background
                backgroundSprites.draw( projectionMatrix );

                // Draw the sandwiches
                for( i=0; i < N_SANDWICHES; i++ ){
                    sandwiches[i]->draw( projectionMatrix );
                    sandwiches[i]->update();
                }

                // Move the sandwiches
                speedMutex.lock();
                for( i=0; i < N_SANDWICHES; i++ ){
                    sandwiches[i]->translate( -speed, 0.0f );
                }
                speedMutex.unlock();

                // Draw the grinder's front.
                grinderFront->draw( projectionMatrix );

                // Draw the tool.
                tool_->draw( projectionMatrix );
                tool_->update();

                // Draw the GUI sprites.
                guiSprites.draw( projectionMatrix );

                // Compute the current game time.
                seconds = timer.getSeconds();
                minutes = seconds / 60;
                seconds = seconds % 60;

                // Write Jacob's life and game time.
                sprintf( buffer, "%03d", (int)jacobHp_ );
                textRenderer.drawText( projectionMatrix, buffer, 0, 75, 5 );
                sprintf( buffer, "%02d:%02d", minutes, seconds );
                textRenderer.drawText( projectionMatrix, buffer, 1, 450, 3 );

                // Refresh screen.
                SDL_GL_SwapWindow( window );
            }

            // Stop the timer.
            timer.stop();

            startAgainText = textRenderer.drawText( "RESTART? (Y/N)",                       // Text
                                                    "data/fonts/LiberationSans-Bold.ttf",   // Font
                                                    50,                                     // Font size
                                                    HEALTH_FONT_COLOR                       // Font color
                                                    );

            // Ask the user if he/she wants to play again.
            std::cout << "quit: " << quit << std::endl;
            if( !quit ){
                std::cout << "ENTRA" << std::endl;

                // Show the user a text asking him / her for restarting.
                m2g::Tileset::bindBuffer();
                startAgainText->moveTo( 400, 200 );
                startAgainText->draw( projectionMatrix );
                SDL_GL_SwapWindow( window );

                std::cout << "1" << std::endl;

                // Wait for user to respond.
                userResponded = false;
                while( !userResponded ){
                    SDL_WaitEvent( &event );
                    if( event.type == SDL_KEYDOWN ){
                        userResponded = true;
                        switch( event.key.keysym.sym ){
                            case SDLK_y:
                                quit = false;
                            break;
                            case SDLK_n:
                                quit = true;
                            break;
                            default:
                                userResponded = false;
                            break;
                        }
                    }
                }

                std::cout << "2" << std::endl;
            }else{
                std::cout << "NO ENTRA (WTF!)" << std::endl;
            }
        }


        // Free resources
        for( i=0; i < N_SANDWICHES; i++ ){
            delete sandwiches[i];
        }

    }catch( std::runtime_error& e ){
        std::cerr << e.what() << std::endl;
    }
}


/***
 * 2. Loading
 ***/

void Level::loadSandwichData()
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement* sandwichXMLElement = nullptr;

    // Load the sandwiches data.
    //graphicsLoader.loadTilesets( sandwichesData, "data/img/sandwiches" );
    graphicsLibrary_.loadAll( "data/img/sandwiches" );

    // Load the dangers data.
    document.LoadFile( "./data/config/sandwiches.xml" );
    sandwichXMLElement = ( document.RootElement() )->FirstChildElement( "sandwich" );
    while( sandwichXMLElement ){
        sandwichData.emplace_back( new SandwichData( sandwichXMLElement, graphicsLibrary_ ) );

        sandwichXMLElement = sandwichXMLElement->NextSiblingElement();
    }
}


void Level::loadDangerData()
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement* dangerXMLElement = nullptr;

    graphicsLibrary_.loadAll( "data/img/dangers" );

    // Load the dangers data.
    document.LoadFile( "./data/config/dangers.xml" );
    dangerXMLElement = ( document.RootElement() )->FirstChildElement( "danger" );
    while( dangerXMLElement ){
        dangerData.emplace_back( new DangerData( dangerXMLElement, graphicsLibrary_ ) );

        dangerXMLElement = dangerXMLElement->NextSiblingElement();
    }
}


void Level::drawTimer( int time )
{
    std::cout << "seconds: " << time << std::endl;
}


/***
 * 3. Predicates
 ***/


bool Level::isSurvivalLevelFinished()
{
    return( jacobHp_ <= 0 );
}


bool Level::isCampaignLevelFinished()
{
    return( ( timer.getSeconds() <= 0 ) || ( jacobHp_ <= 0 ) );
}

} // namespace jdb
