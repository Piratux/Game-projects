Hello there, reader!
This is an early version of "Mage Adventure" game (currently name is just a placeholder).
It's a RPG genre with very basic gameplay implemented so far.
Currently it's being developed by me(Piratux) and Norbiuxx.
The idea for this game is heavily inspired by "Hands of War", so you can expect the end result 
to look somewhat similar to it.


To try out the game, download and extract "Mage Adventure.zip" file.
Then just simply launch "RPG_Project.exe" file.
To compile it yourself, you can find the instructions how to do it below.


How to compile for Visual Studio:
- include all the code from Code folder to the proejct
- open project properties

- select Configuration: All Configurations
- select Platform: All Platforms

- go to Configuration Properties -> General -> C++ Language Standard
- set standard to cpp17

- go to Configuration Properties -> C/C++ -> General -> Additional Include Directories
- add path to Includes/Box2D folder

- go to Configuration Properties -> Linker -> Input -> Additional Dependencies
- add box2d.lib

- apply changes

- go to Configuration Properties -> Linker -> General -> Additional Library Directories
- select Configuration: Debug
- add path to Libraries/Box2D/Debug folder

- apply changes

- select Configuration: Release
- add path to Libraries/Box2D/Release folder

- apply changes
- that's it! now you can try build and run