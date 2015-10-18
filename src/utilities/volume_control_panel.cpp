#include "volume_control_panel.hpp"
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
