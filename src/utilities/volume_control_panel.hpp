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


    private:
        /***
         * 2. Auxiliar methods
         ***/
        void refreshVolumeValue();


        /*** Attributes ***/
        tgui::Label::Ptr volumeLabel_;
        tgui::Slider::Ptr volumeSlider_;
};

} // namespace jdb

#endif // VOLUME_CONTROL_PANEL_HPP
