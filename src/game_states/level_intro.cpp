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

#include <game_states/level_intro.hpp>
#include <SFML/Window/Event.hpp>
#include <utilities/level_book.hpp>
#include <dangers/danger_data_parser.hpp>
#include <paths.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

sf::Vector2u getTileSize( std::string dangerName)
{
    m2g::GraphicsLibrary dangersGraphicsLibrary(DATA_DIR_PATH + "/img/dangers/dangers.xml");
    m2g::AnimationDataPtr dangerAnimData =
            dangersGraphicsLibrary.getAnimationDataByName( dangerName );

    return dangerAnimData->tileset().tileDimensions();
}

LevelIntro::LevelIntro(const GameState& parentGameState,
                        sf::RenderWindow& window,
                        unsigned int levelIndex,
                        const std::vector<std::string> &dangerIDs,
                        tinyxml2::XMLElement* levelBookXmlElement,
                        bool presentNewDangers ) :
    GameState( window ),
    levelIndex_( levelIndex ),
    parentGameState_( parentGameState ),
    gui_( window )
{
    char text[250];
    sprintf( text, "Level %u", levelIndex_ );

    gui_.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );
    LevelBook::Ptr levelBook = std::make_shared<LevelBook>();

    if( levelBookXmlElement != nullptr ){
        tinyxml2::XMLElement* pageXmlElement = levelBookXmlElement->FirstChildElement( "page" );
        while( pageXmlElement ){
            levelBook->addPage( pageXmlElement->GetText() );
            pageXmlElement = pageXmlElement->NextSiblingElement( "page" );
        }
    }else{
        levelBook->addPage( text );
    }
    if( presentNewDangers ){
        DangerDataParser dangerDataParser;
        std::vector<DangerInfo> dangersInfo =
                dangerDataParser.LoadDangersInfoByName(DATA_DIR_PATH + "/config/dangers.json", dangerIDs);
        int i = 0;
        for( DangerInfo& dangerInfo : dangersInfo ){
            sf::Vector2u dangerTextureSize = getTileSize(dangerIDs[i] + "_01.png");

            std::unique_ptr< tgui::Texture > dangerTexture(
                        new tgui::Texture(
                            dangerInfo.texturePath,
                            { 0, 0, static_cast<int>( dangerTextureSize.x ), static_cast<int>( dangerTextureSize.y ) }
                        ) );

            unsigned int minX = 0;
            unsigned int minY = 0;
            unsigned int maxX = 0;
            unsigned int maxY = 0;
            for( unsigned int x = 0; x < dangerTextureSize.x; x++ ){
                for( unsigned int y = 0; y < dangerTextureSize.y; y++ ){
                    if( !( dangerTexture->isTransparentPixel( x, y ) ) ){
                        if( minX == 0 || x < minX ){
                            minX = x;
                        }
                        if( minY == 0 || y < minY ){
                            minY = y;
                        }
                        if( x > maxX ){
                            maxX = x;
                        }
                        if( y > maxY ){
                            maxY = y;
                        }
                    }
                }
            }

            dangerTexture = std::unique_ptr< tgui::Texture >(
                        new tgui::Texture(
                            dangerInfo.texturePath,
                            { static_cast<int>(minX),
                              static_cast<int>(minY),
                              static_cast<int>(maxX - minX),
                              static_cast<int>(maxY - minY) }
                        ) );

            levelBook->addPage( "New danger: \n\t" + dangerInfo.name + "\n\n" +
                                "Description: \n\t" + dangerInfo.description + "\n\n" +
                                "Instructions: \n\t" + dangerInfo.removalInstructions,
                                std::move( dangerTexture ) );
            i++;
        }
    }

    levelBook->setPosition(
                (window_.getSize().x - levelBook->getSize().x) / 2,
                (window_.getSize().y - levelBook->getSize().y) / 2 );


    std::function<void(void)> levelBookCallback =
            std::bind( &LevelIntro::requestStateExit, this, 0 );
    levelBook->connect( "BookClosed", levelBookCallback );

    gui_.add( levelBook );
}


/***
 * 2. GameState interface
 ***/

void LevelIntro::init()
{
    window_.setMouseCursorVisible( true );
}


void LevelIntro::handleEvents()
{
    sf::Event event;

    while( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            requestGameExit();
        }else{
            gui_.handleEvent( event );
        }
    }
}

void LevelIntro::update( unsigned int ms )
{
    (void)(ms);
    handleEvents();
}


void LevelIntro::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
    target.draw( parentGameState_, states );
    gui_.draw();
}


void LevelIntro::pause()
{

}


void LevelIntro::resume()
{

}


} // namespace jdb
