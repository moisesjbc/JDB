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
#include "dependencies/graphics/src/dependencies/tinyxml2/tinyxml2.h"

namespace jdb {

const unsigned int N_SANDWICHES = 5;
const float SANDWICHES_END_POINT = -300.0f;
const float DISTANCE_BETWEEN_SANDWICHES = 300.0f;
const unsigned int N_DANGERS = N_SANDWICHES * 3;
const unsigned int FPS = 25;
const unsigned int REFRESH_TIME = 1000 / FPS;


Level::Level( SDL_Window* window_, unsigned int screenWidth, unsigned int screenHeight )
    : window( window_ )
{
    projectionMatrix = glm::ortho( 0.0f, (float)screenWidth, (float)screenHeight, 0.0f, 1.0f, -1.0f );
}


void Level::runSurvivalLevel( unsigned int index )
{
    tinyxml2::XMLNode* levelNode = nullptr;
    tinyxml2::XMLElement* levelElement = nullptr;

    unsigned int i = 0;
    float initialSpeed;
    float speedStep;
    unsigned int timeLapse;

    // Open the levels configuration file.
    xmlFile.LoadFile( "data/config/levels.xml" );

    std::cout << "1" << std::endl;

    // Iterate over the survival level XML nodes until de number index.
    levelNode = ( xmlFile.FirstChildElement( "levels" )->FirstChildElement( "survival_levels" )->FirstChildElement( "survival_level" ) );
    std::cout << "LevelNode: " << levelNode << std::endl;
    while( levelNode && ( i < index ) ){
        levelNode = levelNode->NextSiblingElement( "survival_level" );
        i++;
    }

    // If the index XML node doesn't exist, throw an exception.
    if( i < index ){
        throw std::runtime_error( "ERROR: Survival level not found" );
    }

        std::cout << "2" << std::endl;
    // Load the sandwiches data.
    loadSandwichData();

    // Load the dangers data.
    loadDangerData();

    // Get the level parameters.
    levelElement = (tinyxml2::XMLElement*)levelNode->FirstChildElement( "speed" );
    initialSpeed = levelElement->FloatAttribute( "initial" );
    speedStep = levelElement->FloatAttribute( "step" );
    timeLapse = (unsigned int)( levelElement->IntAttribute( "time_lapse" ) );

        std::cout << "3" << std::endl;
    // Execute the main loop.
    survivalLoop( initialSpeed, speedStep, timeLapse );
}


void Level::survivalLoop( float initialSpeed, float speedStep, unsigned int timeLapse )
{
    unsigned int i;
    unsigned int firstSandwich = 0;
    unsigned int lastSandwich = N_SANDWICHES - 1;
    float speed = initialSpeed;
    unsigned int nDraws = 0;

    try
    {
        SDL_Event event;
        bool quit = false;

        quit = true;
        while( quit ){
            SDL_WaitEvent( &event );
            quit = ( event.type != SDL_KEYDOWN );
        }

        // Variable for time management.
        Uint32 t0 = 0;
        Uint32 t1 = 0;

        // Create the graphic Library and the sprite pointers.
        m2g::Sprite* sandwiches[N_SANDWICHES];
        m2g::Animation* dangers[N_DANGERS];
        //m2g::Animation* tool = nullptr;
        //m2g::Sprite* conveyorBelt = nullptr;

        // Make the cursor invisible.
        SDL_ShowCursor( SDL_DISABLE );

        // Load the sandwiches' sprites and move them to their positions.
        for( i=0; i < N_SANDWICHES; i++ ){
            sandwiches[i] = new m2g::Sprite( sandwichesData[0] );

            sandwiches[i]->translate( 1024 + i * DISTANCE_BETWEEN_SANDWICHES, 410 );
        }

        // Load the danger's animations and move them to their positions.
        for( i=0; i < N_SANDWICHES; i++ ){
            for( int j=0; j< 3; j++ ){
                dangers[i*3+j] = new m2g::Animation( dangersData[0] );

                dangers[i*3+j]->translate( 1024 + 30 + i * DISTANCE_BETWEEN_SANDWICHES + j * (DISTANCE_BETWEEN_SANDWICHES / 4), 330 );
            }
        }

        /*
        // Load the rest of the sprites and animations.
        tool = new m2g::Animation( library.getAnimationData( "tools.png" ) );
        conveyorBelt = new m2g::Sprite( library.getTileset( "conveyor_belt.png" ) );

        // Set the conveyor belt's sprite at its final position.
        conveyorBelt->translate( 0, 256 );
        */

        // Start the timer.
        timer.init();

        // Keep rendering a black window until player tell us to stop.
        while( !quit ){
            t0 = SDL_GetTicks();
            while( (t1 - t0) < REFRESH_TIME ){
                if( SDL_PollEvent( &event ) != 0 ){
                    switch( event.type ){
                        case SDL_QUIT:
                            quit = true;
                        break;
                        case SDL_MOUSEBUTTONDOWN:
                            // User has clicked. Check if the hand "collides" with any of the
                            // dangers.
                            /*
                            i = 0;
                            while( ( i < N_DANGERS ) && !tool->collide( *(dangers[i] ) ) ){
                                i++;
                            }

                            // The hand collided with a danger, change that danger's state.
                            if( i < N_DANGERS ){
                                dangers[i]->setAnimationState( 1 );
                            }
                            */
                        break;
                        case SDL_KEYDOWN:
                            switch( event.key.keysym.sym ){
                                case SDLK_ESCAPE:
                                    quit = true;
                                break;
                            }
                        break;
                        case SDL_MOUSEMOTION:
                            //tool->moveTo( event.motion.x, event.motion.y );
                        break;
                    }
                }
                t1 = SDL_GetTicks();
            }
            t0 = SDL_GetTicks();
            t1 = SDL_GetTicks();

            // Clear screen.
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            // Bind the tileset's buffer.
            m2g::Tileset::bindBuffer();

            // Draw the conveyor belt.
            //conveyorBelt->draw( projectionMatrix );

            // Draw and move the sandwiches.
            for( i=0; i < N_SANDWICHES; i++ ){
                sandwiches[i]->draw( projectionMatrix );
                sandwiches[i]->translate( -speed, 0.0f );
            }

            // Draw and move the dangers.
            for( i=0; i < N_DANGERS; i++ ){
                dangers[i]->draw( projectionMatrix );
                dangers[i]->translate( -speed, 0.0f );
            }

            /*
            // Draw the tool
            tool->draw( projectionMatrix );
            */

            // Check if the first sandwich reached the sandwiches end point and, in that case,
            // translate it and is dangers behind the last sandwich.
            if( sandwiches[firstSandwich]->getX() < SANDWICHES_END_POINT ){

                // Dangers translation.
                for( i=firstSandwich*3; i<firstSandwich*3+3; i++ ){
                    dangers[i]->translate(
                                sandwiches[lastSandwich]->getX()
                                - sandwiches[firstSandwich]->getX()
                                + DISTANCE_BETWEEN_SANDWICHES,
                                0.0f
                                );
                    dangers[i]->setAnimationState( 0 );
                }

                // Sandwich translation.
                sandwiches[firstSandwich]->translate(
                            sandwiches[lastSandwich]->getX()
                            - sandwiches[firstSandwich]->getX()
                            + DISTANCE_BETWEEN_SANDWICHES,
                            0.0f );

                // Change the indices for the first and last sandwiches.
                firstSandwich = (firstSandwich + 1) % N_SANDWICHES;
                lastSandwich = (lastSandwich + 1) % N_SANDWICHES;
            }

            SDL_GL_SwapWindow( window );


            nDraws++;
            if( nDraws >= FPS ){
                nDraws = 0;
                std::cout << "seconds: " << timer.getSeconds() << std::endl;
            }
        }


        // Free resources
        //delete tool;
        //delete conveyorBelt;
        for( i=0; i < N_SANDWICHES; i++ ){
            delete sandwiches[i];
        }
        for( i=0; i < N_DANGERS; i++ ){
            delete dangers[i];
        }

        // Stop the timer.
        timer.stop();

    }catch( std::runtime_error& e ){
        std::cerr << e.what() << std::endl;
    }
}


/***
 * 2. Loading
 ***/

void Level::loadSandwichData()
{
    // Load the sandwiches data.
    graphicsLoader.loadTilesets( sandwichesData, "data/img/sandwiches" );
}


void Level::loadDangerData()
{
    // Load the dangers data.
    graphicsLoader.loadAnimationsData( dangersData, "data/img/dangers" );
}


void Level::drawTime( int time )
{
    std::cout << "seconds: " << time << std::endl;
}


} // namespace jdb
