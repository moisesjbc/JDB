#ifndef GAME_OVER_SCREEN_HPP
#define GAME_OVER_SCREEN_HPP

#include <game_states/game_state.hpp>
#include <TGUI/TGUI.hpp>

namespace jdb {

class GameOverScreen : public GameState
{
    public:
        /***
         * 1. Construction
         ***/
        GameOverScreen(sf::RenderWindow& window, const GameState& parentGameState);

    protected:
        /***
         * 2. GameState interface
         ***/
        void init();
        void handleEvents();
        void update(unsigned int ms);
        void pause();
        void resume();


    protected:
        /***
         * 3. Drawable interface
         ***/
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;


        /***
         * Attributes
         ***/
        mutable tgui::Gui gui_;
        const GameState& parentGameState_;
};

} // namespace jdb

#endif // GAME_OVER_SCREEN_HPP
