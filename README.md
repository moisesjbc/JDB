JDB
===

A 2D ability game.

## Dependencies

    * [Library tinyxml2](http://www.grinninglizard.com/tinyxml2/)
    * [Library SFML (>= 2.2)](http://www.sfml-dev.org/)
    * [Library m2g](https://github.com/moisesjbc/m2g)
    * [Library TGUI (v0.7-dev)](https://tgui.eu/v0.7-dev/)


## Building from source (Ubuntu 14.04 - x86_64)

1. Open a terminal and clone the last version of [this repository](https://github.com/moisesjbc/JDB).

```
git clone https://github.com/moisesjbc/JDB.git
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
jdb
```
