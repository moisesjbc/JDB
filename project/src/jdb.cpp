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

#include "jdb.hpp"
#include <iostream>

namespace jdb {

const GLfloat WINDOW_WIDTH = 1024;
const GLfloat WINDOW_HEIGHT = 768;


/***
 * 1. Construction
 ***/

JDB::JDB() :
    window( NULL ),
    screen( NULL )
{
    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        throw std::runtime_error( std::string( "ERROR initializing SDL: " ) +
                                  SDL_GetError() );
    }

    // Initialize SDL_image
    if( IMG_Init( IMG_INIT_PNG ) != IMG_INIT_PNG ){
        throw std::runtime_error( IMG_GetError() );
    }

    // Init TTF
    if( TTF_Init() < 0 ){
        throw std::runtime_error( std::string( "ERROR initializing TTF: " ) +
                                  std::string( TTF_GetError() ) );
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
      "JDB",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_OPENGL );

    if( window == NULL ){
        throw std::runtime_error( std::string( "ERROR creating window: " ) +
                                  std::string( SDL_GetError() ) );
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
        std::runtime_error( std::string( "ERROR retrieving OpenGL's version: " ) +
                            std::string( (GLchar* )( gluErrorString( glGetError() ) ) ) );
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

    m2g::checkOpenGL( "JDB constructor" );
}


/***
 * 2. Destruction
 ***/

JDB::~JDB()
{
    // Destroy the OpenGL context (muahaha!).
    SDL_GL_DeleteContext( glContext );

    // Destroy the main window (muahaha! x2).
    SDL_DestroyWindow( window );
    window = NULL;

    // Destroy TTF (muahaha! x3)
    TTF_Quit();

    // Destroy SDL_image (muahaha! x4).
    IMG_Quit();

    // Destroy SDL (muahaha! x5).
    SDL_Quit();
}


/***
 * 3. Game run
 ***/

void JDB::run()
{
    // Text rendering.
    m2g::TextRenderer textRenderer;
    const SDL_Color FONT_COLOR = { 131, 60, 60, 255 };
    m2g::SpritePtr menuText;
    bool quitGame;
    bool optionSelected;
    SDL_Event event;

    // Create a sprite with the "menu".
    menuText = textRenderer.drawText(
                "MENU\n---\nPRESS A KEY\n---\n\nC - CAMPAIGN MODE\nS - SURVIVAL MODE\nESC - EXIT",   // Text
                "data/fonts/LiberationSans-Bold.ttf",   // Font
                30,                                     // Font size
                FONT_COLOR );                           // Font color

    quitGame = false;
    while( !quitGame ){
        // Show the "menu" to the player.
        glClear ( GL_COLOR_BUFFER_BIT );
        m2g::Tileset::bindBuffer(); // TODO: Make this line unuseful.
        menuText->draw( projectionMatrix );
        SDL_GL_SwapWindow( window );

        // Wait for the player to press 'c' or 's'.
        optionSelected = false;
        while( !optionSelected ){
            SDL_WaitEvent( &event );

            quitGame = ( event.type == SDL_QUIT ) || ( ( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_ESCAPE ) );
            optionSelected = quitGame ||
                    ( ( event.type == SDL_KEYDOWN ) && ( ( event.key.keysym.sym == SDLK_s ) || ( event.key.keysym.sym == SDLK_c ) ) );
        }

        // If the user didn't exit the menu by trying to quit the game, it means
        // that he/she selected one game mode.
        if( !quitGame ){
            // Run a campaign level or a survival one according to player's selection.
            if(  event.key.keysym.sym == SDLK_s ){
                level_ = new SurvivalLevel( window, screen, WINDOW_WIDTH, WINDOW_HEIGHT );
            }else{
                level_ = new CampaignLevel( window, screen, WINDOW_WIDTH, WINDOW_HEIGHT );
            }

            level_->run( 0 );
        }
        delete level_;
        level_ = nullptr;
    }
}


} // namespace jdb
