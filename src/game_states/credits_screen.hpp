#ifndef CREDITSSCREEN_HPP
#define CREDITSSCREEN_HPP

#include "game_state.hpp"
#include <TGUI/TGUI.hpp>
#include "../paths.hpp"

namespace jdb {

class CreditsScreen : public GameState
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
        void init();
        void handleEvents();
        void update(unsigned int ms);
        void pause();
        void resume();


        /***
         * 3. Drawable interface
         ***/
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    private:
        /***
         * 4. Credits generation
         ***/
        std::string generateCredits() const;


    private:
        mutable tgui::Gui gui_;
};

} // namespace jdb

#endif // CREDITSSCREEN_HPP
