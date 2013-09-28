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

#include "jdb.hpp"
#include <iostream>

namespace jdb {

const GLfloat WINDOW_WIDTH = 1024;
const GLfloat WINDOW_HEIGHT = 768;

JDB::JDB() :
    window( NULL ),
    screen( NULL )
{
    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        throw std::runtime_error( std::string( "ERROR initializing SDL: " ) + SDL_GetError() );
    }

    // Initialize SDL_image
    if( IMG_Init( IMG_INIT_PNG ) != IMG_INIT_PNG ){
        throw std::runtime_error( IMG_GetError() );
    }

    // Initialize some OpenGL attributes.
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );

    // Create main window
    window = SDL_CreateWindow(
      "JDB v3",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

    if( window == NULL ){
        throw std::runtime_error( std::string( "ERROR creating window: " ) + std::string( SDL_GetError() ) );
    }

    // Retrieve the window's screen.
    screen = SDL_GetWindowSurface( window );

    // Create an OpenGL context.
    glContext = SDL_GL_CreateContext( window );

    // Initialize OpenGL and create a GL context.
    if( gl3wInit() ){
        throw std::runtime_error( "ERROR when using gl3wInit()" );
    }
    if (!gl3wIsSupported(4, 2)) {
        throw std::runtime_error( "OpenGL 4.2 not supported" );
    }

     std::cout << "OpenGL " << glGetString(GL_VERSION) << ", GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Retrieve and display the context's version.
    int majorVersion, minorVersion;
    SDL_GL_GetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion );
    SDL_GL_GetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion );
    std::cout << "Context version (SDL/OpenGL): " << majorVersion << "." << minorVersion << std::endl;


    // Display a string with the OpenGL version.
    const unsigned char* version = glGetString( GL_VERSION );
    if( version == NULL ){
        std::runtime_error( std::string( "ERROR retrieving OpenGL's version: " ) + std::string( (GLchar* )( gluErrorString( glGetError() ) ) ) );
    }else{
        std::cout << "Version: " << version << std::endl;
    }

    // Display a string showing the GLSL version.
    std::cout << "GLSL Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

    // Load and use shaders.
    shaderLoader = msl::ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    shaderLoader->destroy();

    // Initialize OpenGL.
    glDisable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glClearColor( 0xF5/255.0f, 0xF6/255.0f, 0xCE/255.0f, 1.0f );

    // Set projection mode.
    projectionMatrix = glm::ortho( 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 1.0f, -1.0f );

    std::cout << "JDB constructor finished - " << gluErrorString( glGetError() ) << std::endl;
}


JDB::~JDB()
{
    // Destroy the OpenGL context (muahaha!).
    SDL_GL_DeleteContext( glContext );

    // Destroy the main window (muahaha! x2).
    SDL_DestroyWindow( window );
    window = NULL;

    // Destroy SDL_image (muahaha! x3).
    IMG_Quit();

    // Destroy SDL (muahaha! x4).
    SDL_Quit();
}


void JDB::runInfiniteSandwichesDemo()
{
    const unsigned int N_SANDWICHES = 5;
    const float SANDWICHES_END_POINT = -300.0f;
    const float DISTANCE_BETWEEN_SANDWICHES = 300.0f;
    const unsigned int N_DANGERS = N_SANDWICHES * 3;

    unsigned int i;
    unsigned int firstSandwich = 0;
    unsigned int lastSandwich = N_SANDWICHES - 1;

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
        m2g::Library library;
        m2g::Sprite* sandwiches[N_SANDWICHES];
        m2g::Animation* dangers[N_DANGERS];
        m2g::Animation* tool = nullptr;
        m2g::Sprite* conveyorBelt = nullptr;

        // X velocity for sprite "sandwich".
        GLfloat dx = -8.0f;

        // Make the cursor invisible.
        SDL_ShowCursor( SDL_DISABLE );

        // Load the graphic library.
        library.load( "data/img/infinite_sandwiches_demo" );

        // Load the sandwiches' sprites and move them to their positions.
        for( i=0; i < N_SANDWICHES; i++ ){
            if( (i % 3) == 0 ){
                sandwiches[i] = new m2g::Sprite( library.getTileset( "sandwich_02.png" ) );
            }else if( (i % 3) == 1 ){
                sandwiches[i] = new m2g::Sprite( library.getTileset( "sandwich_03.png" ) );
            }else{
                sandwiches[i] = new m2g::Sprite( library.getTileset( "sandwich_04.png" ) );
            }

            sandwiches[i]->translate( 1024 + i * DISTANCE_BETWEEN_SANDWICHES, 410 );
        }

        // Load the danger's animations and move them to their positions.
        for( i=0; i < N_SANDWICHES; i++ ){
            for( int j=0; j< 3; j++ ){
                dangers[i*3+j] = new m2g::Animation( library.getAnimationData( "knife_02.png" ) );

                dangers[i*3+j]->translate( 1024 + 30 + i * DISTANCE_BETWEEN_SANDWICHES + j * (DISTANCE_BETWEEN_SANDWICHES / 4), 330 );
            }
        }

        // Load the rest of the sprites and animations.
        tool = new m2g::Animation( library.getAnimationData( "tools.png" ) );
        conveyorBelt = new m2g::Sprite( library.getTileset( "conveyor_belt.png" ) );

        // Set the conveyor belt's sprite at its final position.
        conveyorBelt->translate( 0, 256 );

        // Keep rendering a black window until player tell us to stop.
        while( !quit ){
            t0 = SDL_GetTicks();
            while( (t1 - t0) < 40 ){
                if( SDL_PollEvent( &event ) != 0 ){
                    switch( event.type ){
                        case SDL_QUIT:
                            quit = true;
                        break;
                        case SDL_MOUSEBUTTONDOWN:
                            // User has clicked. Check if the hand "collides" with any of the
                            // dangers.
                            i = 0;
                            while( ( i < N_DANGERS ) && !tool->collide( *(dangers[i] ) ) ){
                                i++;
                            }

                            // The hand collided with a danger, change that danger's state.
                            if( i < N_DANGERS ){
                                dangers[i]->setAnimationState( 1 );
                            }
                        break;
                        case SDL_KEYDOWN:
                            switch( event.key.keysym.sym ){
                                case SDLK_ESCAPE:
                                    quit = true;
                                break;
                            }
                        break;
                        case SDL_MOUSEMOTION:
                            tool->moveTo( event.motion.x, event.motion.y );
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
            conveyorBelt->draw( projectionMatrix );

            // Draw and move the sandwiches.
            for( i=0; i < N_SANDWICHES; i++ ){
                sandwiches[i]->draw( projectionMatrix );
                sandwiches[i]->translate( dx, 0.0f );
            }

            // Draw and move the dangers.
            for( i=0; i < N_DANGERS; i++ ){
                dangers[i]->draw( projectionMatrix );
                dangers[i]->translate( dx, 0.0f );
            }

            // Draw the tool
            tool->draw( projectionMatrix );

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
        }


        // Free resources
        delete tool;
        delete conveyorBelt;
        for( i=0; i < N_SANDWICHES; i++ ){
            delete sandwiches[i];
        }
        for( i=0; i < N_DANGERS; i++ ){
            delete dangers[i];
        }

    }catch( std::runtime_error& e ){
        std::cerr << e.what() << std::endl;
    }
}



void JDB::runCollisionDemo()
{
    try
    {
        unsigned int animationCount = 0;
        SDL_Event event;
        bool quit = false;

        // Variable for time management.
        Uint32 t0 = 0;
        Uint32 t1 = 0;

        // Create the graphic Library and the sprite pointers.
        m2g::Library library;
        m2g::Sprite* staticTool = nullptr;
        m2g::Sprite* dynamicTool = nullptr;
        m2g::Animation* animation = nullptr;

        // Make the cursor invisible.
        SDL_ShowCursor( SDL_DISABLE );

        // Load the graphic library.
        library.load( "data/img/collision_test_demo/" );

        // Set the sprite's tilesets.
        staticTool = new m2g::Sprite( library.getTileset( "tileset_test.png" ) );
        dynamicTool = new m2g::Sprite( staticTool->getTileset() );
        animation = new m2g::Animation( library.getAnimationData( 0 ) );

        // Set the static tool and the animation at a fixed position.
        staticTool->translate( 100, 400 );
        animation->translate( 700, 400 );

        // Keep rendering a black window until player tell us to stop.
        while( !quit ){
            t0 = SDL_GetTicks();
            while( (t1 - t0) < 40 ){
                if( SDL_PollEvent( &event ) != 0 ){
                    switch( event.type ){
                        case SDL_QUIT:
                            quit = true;
                        break;
                        case SDL_KEYDOWN:
                            switch( event.key.keysym.sym ){
                                case SDLK_LEFT:
                                    dynamicTool->previousTile();
                                break;
                                case SDLK_RIGHT:
                                    dynamicTool->nextTile();
                                break;
                                case SDLK_ESCAPE:
                                    quit = true;
                                break;
                            }
                        break;
                        case SDL_MOUSEMOTION:
                            dynamicTool->translate( event.motion.xrel, event.motion.yrel );
                        break;
                    }
                }
                t1 = SDL_GetTicks();
            }
            t0 = SDL_GetTicks();
            t1 = SDL_GetTicks();

            // Clear screen.
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            if( dynamicTool->collide( *staticTool ) ){
                staticTool->setTile( 1 );
            }else{
                staticTool->setTile( 0 );
            }

            if( dynamicTool->collide( *animation ) ){
                animation->setAnimationState( !animation->getAnimationState() );
            }

            m2g::Tileset::bindBuffer();
            staticTool->draw( projectionMatrix );
            dynamicTool->draw( projectionMatrix );
            animation->draw( projectionMatrix );

            // Update the animation every 5 frames.
            animationCount++;
            if( animationCount >= 20 ){
                animationCount = 0;
                animation->update();
            }

            SDL_GL_SwapWindow( window );
        }


        // Free resources
        delete staticTool;
        delete dynamicTool;
        delete animation;
    }catch( std::runtime_error& e ){
        std::cerr << e.what() << std::endl;
    }
}

} // namespace jdb
