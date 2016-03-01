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
