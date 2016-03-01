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

#include <utilities/volume_control_panel.hpp>
#include <SFML/Audio/Listener.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

VolumeControlPanel::VolumeControlPanel()
{
    // Create widgets.
    volumeSlider_ = std::make_shared<tgui::Slider>();
    volumeLabel_ = std::make_shared<tgui::Label>();

    // Initialize the slider with the curren volume value.
    volumeSlider_->setMinimum( 0 );
    volumeSlider_->setMaximum( 100 );
    volumeSlider_->setValue( sf::Listener::getGlobalVolume() );

    // Update the displayed value when changed.
    volumeSlider_->connect( "ValueChanged", [this](float newVolume){
        sf::Listener::setGlobalVolume( newVolume );
    });

    // Add widgets to layout.
    VerticalLayout::add( volumeLabel_ );
    VerticalLayout::insertSpace( 1, 0.5f );
    VerticalLayout::add( volumeSlider_ );
}


/***
 * 2. Drawable interface
 ***/

void VolumeControlPanel::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    refreshVolumeValue();
    VerticalLayout::draw( target, states );
}


/***
 * 3. Auxiliar methods
 ***/

void VolumeControlPanel::refreshVolumeValue() const
{
    char volumeStr[20];

    sprintf( volumeStr,
             "Volume: %u",
             static_cast<unsigned int>( sf::Listener::getGlobalVolume() ) );

    volumeLabel_->setText( volumeStr );
    volumeSlider_->setValue( sf::Listener::getGlobalVolume() );
}

} // namespace jdb
