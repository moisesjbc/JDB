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

class Animation : public Drawable
{
    private:
        Sprite sprite;
        std::shared_ptr< AnimationData > animationData;
        int currentState;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Animation( std::shared_ptr< AnimationData > animationData );


        /***
         * 2. Getters and setters
         ***/
        void setAnimationData( std::shared_ptr< AnimationData > animationData );
        void setState( int newState );
        int getState() const ;


        /***
         * 3. Transformations
         ***/
        virtual void translate( const float& tx, const float& ty );


        /***
         * 4. Collision test
         ***/
    private:
        virtual const std::vector<Rect>* getCollisionRects() const ;
    public:

        /***
         * 5. Updating
         ***/
        void update();


        /***
         * 6. Drawing
         ***/
        virtual void draw( const glm::mat4& projectionMatrix ) const ;

};

} // namespace m2g

#endif // ANIMATION_HPP
