# The Sandwiches Game (JDB)

![JDB game](img/jdb.jpg "JDB game")

**Project status: Active**


## About

The Sandwiches game, or JDB (Juego De los Bocadillos, in spanish), is an ability 2D game where the player have to combine multiple tools in order to remove the dangers that travel on delicious sandwiches.
Background

JDB's story focuses on Jacob, a young boy and its ambition to get a world record: eating the most sandwiches never seen, thanks to a giant grinder he has just built. The only problem is that in his rush for glory, our hero has made the sandwiches in the garage. No one can imagine what disagreeable surprises could have been added to the sandwiches!

As our hero's best friend Marcos, your mission with consist in removing all the threats from the sandwiches before they get the grinder and damage the protagonist, preventing him from archieve victory.

## Game mechanics

The action in The Sandwiches game focuses on the conveyor belt carrying the dangers-infected sandwiches to the giant grinder and our hero. The player will have to combine multiple tools in order to remove the dangers from the sandwiches before they get to the giant grinder and damage his/her best friend.

### Tools

There is a total of 4 tools for player to combine them and remove dangers.

- **Hand**
- **Extinguisher**
- **Lighter**
- **Gavel**

### Dangers

* [See dangers.md](./share/sandwiches-game/docs/dangers.md) for a complete list.

## Game modes

**The Sandwiches games has two game modes: campaign and survival**.

- **Campaign mode consists of a sequence of levels with time limit**. In order to win, the player must prevent Jacob from dying before archieving his goal. Level after level, new dangers will make things more difficult.

- **In survival mode the player will have to protect Jacob as long as possible**. Like in campaign mode, new dangers will appear to increase difficulty. 

## Multimedia

[Youtube playlist](http://www.youtube.com/playlist?list=PLfi4ueBWh9m7XmYMS8lGjgIC0mvYB2_H9)


## Dependencies

* [Boost](www.boost.org/)
* [tinyxml2](http://www.grinninglizard.com/tinyxml2/)
* [SFML (>= 2.3.2)](http://www.sfml-dev.org/)
* [m2g (v0.3)](https://github.com/moisesjbc/m2g)
* [v0.7-dev)](https://tgui.eu/v0.7-dev/)
* [CMake](https://cmake.org/)
* [Easylogging++](https://github.com/easylogging/easyloggingpp)


## Building from source (Ubuntu 14.04 - x86_64)

1. Open a terminal and clone the last version of [this repository](https://github.com/moisesjbc/JDB).

```
git clone https://github.com/moisesjbc/sandwiches-game.git
```

2. Enter into the project's directory.

```
cd sandwiches-game
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

## Dockerfile (for developers)

This repository comes with a Dockerfile for building a [Docker](https://www.docker.com/) image for continuous integration. This image includes all the required packages and dependencies for building *The Sandwiches Game*. 

To build the image, run the following **on the top directory of the repository**:

        sudo docker build -t moisesjbc/ubuntu-base-for-sandwiches-game:0.1 .

Those third-party dependencies not installed from repository are present in the directory */opt/third-party* in the Docker image.
