# Rock, Paper, Scissors

Watch rock beats scissors and scissors beat paper! 

But wait, there's more! Paper also beats rock!!


![Screenshot](/ingame.png)

Made with SFML as a small experiment.

### How to build
Just a regular CMake. **SFML** will be acquired automatically (using FetchContent) by the CMake script. **C++20 is required.**

Tried on Linux with Debian/Ubuntu.

```
cd build
cmake .. .
make
bin/RockPaperScissors
```