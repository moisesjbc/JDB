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
    shaderLoader = ShaderLoader::getInstance();
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


void JDB::run()
{
    SDL_Event event;
    bool quit = false;

    // Make the cursor invisible.
    SDL_ShowCursor( SDL_DISABLE );

    Uint32 t0 = 0;
    Uint32 t1 = 0;

    jdb::Sprite staticTool, dynamicTool;
    jdb::Sprite sandwich;

    GLfloat dx = 10.0f;

    tinyxml2::XMLDocument tilesetsFile;
    tilesetsFile.LoadFile( "data/img/tilesets.xml" );

    sandwich.setTileset( Sprite::loadTileset( tilesetsFile.FirstChildElement( "tileset" )->NextSiblingElement( "tileset" ) ) );
    staticTool.setTileset( Sprite::loadTileset( tilesetsFile.FirstChildElement( "tileset" ) ) );
    dynamicTool.setTileset( staticTool.getTileset() );

    dynamicTool.setTile( 0 );

    staticTool.translate( 300, 300 );

    // Set Sprite VAO as the active one.

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
                                dynamicTool.previousTile();
                            break;
                            case SDLK_RIGHT:
                                dynamicTool.nextTile();
                            break;
                            case SDLK_ESCAPE:
                                quit = true;
                            break;
                        }
                    break;
                    case SDL_MOUSEMOTION:
                        dynamicTool.translate( event.motion.xrel, event.motion.yrel );

                        if( dynamicTool.collide( staticTool ) ){
                            staticTool.setTile( 1 );
                        }else{
                            staticTool.setTile( 0 );
                        }
                    break;
                }
            }
            t1 = SDL_GetTicks();
        }
        t0 = SDL_GetTicks();
        t1 = SDL_GetTicks();

        // Clear screen.
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        sandwich.translate( dx, 0 );
        if( ( sandwich.getX() > WINDOW_WIDTH ) || ( sandwich.getX() < 0 ) ){
            dx = -dx;
        }

        staticTool.draw( projectionMatrix );
        dynamicTool.draw( projectionMatrix );
        sandwich.draw( projectionMatrix );

        SDL_GL_SwapWindow( window );
    }
}

} // namespace jdb
