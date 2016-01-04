# ToDo list 

## Next release (v0.1.0)

- Add music!
- Add a credits screen.
- Add an "End of demo" screen with contact information.
- Add a game over screen (to both campaign and survival modes).
- Check memory usage (ie. find memory leacks using Valgrind).
- Update README.
- Add Git tag 'v0.1.0'
- Set a minimum required version for g++ in deps building script.
- Build DEB package.
- Build for Windows.
- Release on Github and web.

## Future releases

### Features

- Include a level selection screen showing locked and unlocked levels 
(campaign mode).
- Make the game fairer among users.
    - Because of the dangers being selected randomly every time, two players
    playing the same level aspire to different maximum scores. Fix this.
    - Note: this affects to the contents of enigma bombs and shells too.
- Display the index of the current level on the GUI.
- Add a life bar to dangers such as the fire.
- Add the rest of levels (plan this one).
- Downgrade dependency TGUI from v0.7-dev to v0.6?.
- Create a "Release notes" document.
- Allow user to save / load the game.
- Create a changelog.
- Allow user to set its own build directory with CMake.
- Give a greater appearance probability to the new dangers of each level.
- More dangers!
- Add a wiki.
    - How to add new dangers to the game.
    - How to add new levels to the game.
    - Etc.

## Refactoring and quality

- Add automated tests to JDB (to all classes).
- Add continuous integration script to JDB (Jenkins).
- Make class Level stop being a God object!
- Separate auxiliar structs in their own files.
- Find and remove unused #includes.

## Optional goals

- Add tags to dangers. Examples of tags:
    - "Indirect danger": this danger can only appear as a result of a 
    transition (i.e. medic kits inside enigma bombs).
    - "Common danger"
    - "Unusual danger"
- Keep Jacob's life unchanged when passing from one level to the next.
- Display the current speed of the conveyor belt on the GUI.
- (Refactoring) When user closes the game window, call 
GameState::requestStateExit() with an "special exit code" instead of calling 
exit();
- Include License note on main menu.
- Make GameState::init() call GameState::resume().
- Move sandwiches.push_back(...) from Level::reset() to Level::init().
