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

#ifndef LEVEL_INTRO_HPP
#define LEVEL_INTRO_HPP

#include "game_state.hpp"
#include <m2g/text/text_renderer.hpp>

namespace jdb {

class LevelIntro : public GameState
{
    public:
        /***
         * 1. Construction
         ***/
        LevelIntro( Window& window, unsigned int levelIndex );


    protected:
        /***
         * 2. GameState interface
         ***/
        virtual void init();
        virtual void handleEvents();
        virtual void update();
        virtual void draw();
        virtual void pause();
        virtual void resume();


    private:
        unsigned int levelIndex_;
        m2g::SpritePtr levelIntroText_;
};

} // namespace jdb

#endif // LEVEL_INTRO_HPP
