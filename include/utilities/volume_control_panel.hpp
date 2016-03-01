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

#ifndef VOLUME_CONTROL_PANEL_HPP
#define VOLUME_CONTROL_PANEL_HPP

#include <TGUI/VerticalLayout.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Slider.hpp>

namespace jdb {

class VolumeControlPanel : public tgui::VerticalLayout
{
    public:
        typedef std::shared_ptr<VolumeControlPanel> Ptr;
        typedef std::shared_ptr<const VolumeControlPanel> ConstPtr;


        /***
         * 1. Construction
         ***/
        VolumeControlPanel();


    protected:
        /***
         * 2. Drawable interface
         ***/
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    private:
        /***
         * 3. Auxiliar methods
         ***/
        void refreshVolumeValue() const;


        /*** Attributes ***/
        mutable tgui::Label::Ptr volumeLabel_;
        mutable tgui::Slider::Ptr volumeSlider_;
};

} // namespace jdb

#endif // VOLUME_CONTROL_PANEL_HPP
