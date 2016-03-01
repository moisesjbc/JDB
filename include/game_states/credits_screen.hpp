#ifndef CREDITSSCREEN_HPP
#define CREDITSSCREEN_HPP

#include <game_states/gui_menu.hpp>
#include <TGUI/TGUI.hpp>
#include "../paths.hpp"
#include <utilities/unicode_file_reader.hpp>

namespace jdb {

class CreditsScreen : public GUIMenu
{
    public:
        /***
         * 1. Construction
         ***/
        CreditsScreen( sf::RenderWindow& window );


    protected:
        /***
         * 2. GameState interface
         ***/
        void initGUI(tgui::Gui &gui);


    private:
        /***
         * 4. Credits generation
         ***/
        std::wstring generateCredits() const;
};

} // namespace jdb

#endif // CREDITSSCREEN_HPP
