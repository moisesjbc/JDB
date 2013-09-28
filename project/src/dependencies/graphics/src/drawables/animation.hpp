/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of M2G.
 *
 * M2G is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * M2G is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2G.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "animation_data.hpp"
#include "sprite.hpp"

namespace m2g {

class Animation : public Sprite
{
    private:
        std::shared_ptr< AnimationData > animationData;
        int currentState;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Animation( const std::shared_ptr< AnimationData >& animationData );


        /***
         * 2. Getters
         ***/        
        int getAnimationState() const ;
        GLuint getFrame() const ;


        /***
         * 3. Setters
         ***/
        // TODO: Overload Sprite setters.
        virtual void setAnimationData( const std::shared_ptr< AnimationData >& animationData );
        virtual void setAnimationState( int newState );


        /***
         * 4. Updating
         ***/
        void update();
};

} // namespace m2g

#endif // ANIMATION_HPP
