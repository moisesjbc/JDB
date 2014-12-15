/***
    Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)

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
#include "../dependencies/m2g/src/dependencies/tinyxml2/tinyxml2.h"

namespace jdb {

const unsigned int N_SANDWICHES = 5;
const float SANDWICHES_END_POINT = 0.0f;
const float DISTANCE_BETWEEN_SANDWICHES = 300.0f;
const unsigned int N_DANGERS = N_SANDWICHES * 3;


/***
 * 1. Construction
 ***/

Level::Level( SDL_Window* window_,
              SDL_Surface* screen_,
              unsigned int screenWidth,
              unsigned int screenHeight )
    : window( window_ ),
      screen( screen_ )
{
    projectionMatrix = glm::ortho( 0.0f, (float)screenWidth, (float)screenHeight, 0.0f, 1.0f, -1.0f );

    // Initialize the GUI.
    initGUI();
}


/***
 * 2. Level execution
 ***/

void Level::run( unsigned int levelIndex )
{
    // Load the required level.
    load( levelIndex );

    // Run the main loop.
    mainLoop();
}


/***
 * 3. Level Loading
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


void Level::initGUI()
{
    // Load the "gui" graphics library.
    graphicsLibrary_.loadAll( "data/img/gui" );

    // Load the GUI sprites.
    guiSprites_.addSprite( graphicsLibrary_.getTileset( "health.png" ) );
    guiSprites_.addSprite( graphicsLibrary_.getTileset( "time.png" ), 367.0f, 0.0f );
    guiToolSelector_ = guiSprites_.addSprite( graphicsLibrary_.getTileset( "tool_selector.png" ), 384.0f, 660.0f );

    // Load the "tools" graphics library.
    graphicsLibrary_.loadAll( "data/img/tools" );

    // Load the player's tool.
    tool_ = ToolPtr( new Tool( graphicsLibrary_.getAnimationData( "tools.png" ) ) );
}


/***
 * 4. Main loop
 ***/

void Level::mainLoop()
{
    unsigned int i;

    // Variables used for sandwich reseting.
    unsigned int firstSandwich;
    unsigned int lastSandwich;

    // Conveyor belt's speed management.
    std::mutex speedMutex;
    float speed = conveyorBelt_.getInitialSpeed();

    // Time management.
    Uint32 t0;
    Uint32 t1;
    int seconds, minutes;

    // Event handling.
    SDL_Event event;
    bool userResponded = false;

    // Sandwiches
    Sandwich* sandwiches[N_SANDWICHES];

    // Text rendering.
    m2g::TextRenderer textRenderer;
    char buffer[16];
    const SDL_Color HEALTH_FONT_COLOR = { 131, 60, 60, 255 };
    const SDL_Color TIMER_FONT_COLOR = { 8, 31, 126, 255 };

    // Background sprites.
    m2g::DrawablesSet backgroundSprites;
    m2g::SpritePtr grinderFront;

    // Text sprites.
    m2g::SpritePtr startAgainText;

    try
    {
        // Initialize the text renderer.
        coutMutex.lock();
        std::cout << "loadFont: " << textRenderer.loadFont( "data/fonts/LiberationSans-Bold.ttf", 50, HEALTH_FONT_COLOR ) << std::endl;
        std::cout << "loadFont: " << textRenderer.loadFont( "data/fonts/LiberationSans-Bold.ttf", 50, TIMER_FONT_COLOR ) << std::endl;
        coutMutex.unlock();

        // Make the cursor invisible.
        SDL_ShowCursor( SDL_DISABLE );

        // Load all the needed tilesets and animations (the graphics for
        // dangers and sandwiches are loaded in the methods "loadDangers" and
        // "loadSandwiches".
        graphicsLibrary_.loadAll( "data/img/background" );

        // Load the background sprites
        backgroundSprites.addSprite( graphicsLibrary_.getTileset( "grinder_back.png" ), 0.0f, -256.0f );
        backgroundSprites.addSprite( graphicsLibrary_.getTileset( "conveyor_belt.png" ), 0.0, 256.0f );
        grinderFront = m2g::SpritePtr( new m2g::Sprite( graphicsLibrary_.getTileset( "grinder_front.png" ) ) );
        grinderFront->moveTo( 0.0f, -256.0f );

        // Restarting loop. Keep restarting this level until the player tell us
        // to stop.
        quitLevel_ = false;
        while( !quitLevel_ ){
            // Initialize the variables used for time management.
            t0 = 0;
            t1 = 0;

            // Initialize jacob's life and the sandwich indicators.
            jacobHp_ = 100;
            firstSandwich = 0;
            lastSandwich = N_SANDWICHES - 1;

            // Load the sandwiches, move them to their final positions and
            // populate them with dangers.
            for( i=0; i < N_SANDWICHES; i++ ){
                sandwiches[i] = new Sandwich( sandwichData[0], &dangerData );

                sandwiches[i]->moveTo( 1024 + i * DISTANCE_BETWEEN_SANDWICHES, 410 );

                sandwiches[i]->populate( dangerData );
            }

            // Reset the timer.
            resetTimer();

            // Start the timer.
            timer_.play();

            // Main loop: run the game until player ask us or the finish
            // predicate is true.
            while( !quitLevel_ && !finishPredicate() ){
                // Clear screen.
                glClear ( GL_COLOR_BUFFER_BIT );

                // Handle user input.
                t0 = SDL_GetTicks();
                while( (t1 - t0) < REFRESH_TIME ){
                    if( SDL_PollEvent( &event ) != 0 ){
                        handleUserInput( event, sandwiches );
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
                guiSprites_.draw( projectionMatrix );

                // Compute the current game time.
                seconds = timer_.getSeconds();
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
            timer_.stop();

            startAgainText = textRenderer.drawText( "RESTART? (Y/N)",                       // Text
                                                    "data/fonts/LiberationSans-Bold.ttf",   // Font
                                                    50,                                     // Font size
                                                    HEALTH_FONT_COLOR                       // Font color
                                                    );

            // Ask the user if he/she wants to play again.
            if( !quitLevel_ ){
                // Show the user a text asking him / her for restarting.
                m2g::Tileset::bindBuffer();
                startAgainText->moveTo( 400, 200 );
                startAgainText->draw( projectionMatrix );
                SDL_GL_SwapWindow( window );

                // Wait for user to respond.
                userResponded = false;
                while( !userResponded ){
                    SDL_WaitEvent( &event );
                    if( event.type == SDL_KEYDOWN ){
                        userResponded = true;
                        switch( event.key.keysym.sym ){
                            case SDLK_y:
                                quitLevel_ = false;
                            break;
                            case SDLK_n:
                                quitLevel_ = true;
                            break;
                            default:
                                userResponded = false;
                            break;
                        }
                    }
                }
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

void Level::handleUserInput( const SDL_Event& event, Sandwich** sandwiches )
{
    switch( event.type ){
        case SDL_QUIT:
            // Player wants to exit the game.
            quitLevel_ = true;
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
                    quitLevel_ = true;
                break;
                case SDLK_a:
                    tool_->setToolType( ToolType::HAND );
                    guiToolSelector_->setTile( 0 );
                break;
                case SDLK_s:
                    tool_->setToolType( ToolType::EXTINGUISHER );
                    guiToolSelector_->setTile( 1 );
                break;
                case SDLK_d:
                    tool_->setToolType( ToolType::LIGHTER );
                    guiToolSelector_->setTile( 2 );
                break;
                case SDLK_f:
                    tool_->setToolType( ToolType::GAVEL );
                    guiToolSelector_->setTile( 3 );
                break;
            }
        break;
        case SDL_MOUSEMOTION:
            // Player wants to move the mouse / tool.
            tool_->moveTo( event.motion.x, event.motion.y );
        break;
    }
}


/***
 * 5. Auxiliar methods
 ***/

void Level::drawTimer( int time )
{
    std::cout << "seconds: " << time << std::endl;
}


int Level::getSeconds() const
{
    return timer_.getSeconds();
}


} // namespace jdb
