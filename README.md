# The Sandwiches Game (JDB)

![JDB game](img/jdb.jpg "JDB game")

**Project status: Active**

## About

The Sandwiches game, or JDB (Juego De los Bocadillos, in spanish), is an ability 2D game where the player have to combine multiple tools in order to remove the dangers that travel on delicious sandwiches.
Background

JDB's story focuses on a young boy and its ambition to get a world record: eating the most sandwiches never seen, thanks to a giant grinder he has just built. The only problem is that in his rush for glory, our hero has made the sandwiches in the garage. No one can imagine what disagreeable surprises could have been added to the sandwiches!

As our hero's best friend, your mission with consist in removing all the threats from the sandwiches before they get the grinder and damage the protagonist, preventing him from archieve victory.

## Game mechanics

The action in The Sandwiches game focuses on the conveyor belt carrying the dangers-infected sandwiches to the giant grinder and our hero. The player will habe to combine multiple tools in order to remove the dangers from the sandwiches before they get to the giant grinder and damage his/her best friend.


## Multimedia

[Youtube playlist](http://www.youtube.com/playlist?list=PLfi4ueBWh9m7XmYMS8lGjgIC0mvYB2_H9)


## Dependencies

    * [Library tinyxml2](http://www.grinninglizard.com/tinyxml2/)
    * [Library SFML (>= 2.2)](http://www.sfml-dev.org/)
    * [Library m2g](https://github.com/moisesjbc/m2g)
    * [Library TGUI (v0.7-dev)](https://tgui.eu/v0.7-dev/)


## Building from source (Ubuntu 14.04 - x86_64)

1. Open a terminal and clone the last version of [this repository](https://github.com/moisesjbc/JDB).

```
git clone https://github.com/moisesjbc/sandwiches-game.git
```

2. Enter into the project's directory.

```
cd JDB
```

3. Build dependencies using the script provided.

```
./build_deps_ubuntu_14_04_x86_64.sh
```

4. Build and install the game.

```
cmake .
sudo make install
```

5. Run the game and enjoy!

```
sandwiches-game
```

## ToDo list 

### Next release (v0.1.0)

- Set a minimum required version for g++ in deps building script.
- Fix layout problems in LevelBook so elements doesn't overlay each other.
- Build DEB package.
- Build for Windows.
- Create a "Release notes" document.
- Add a game over screen to survival mode.


### Future releases
- Create a changelog.
- Allow user to set its own build directory with CMake.
- More dangers!
- Add a wiki.
    - How to add new dangers to the game.
    - How to add new levels to the game.
    - Etc.
