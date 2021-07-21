[![CMake](https://github.com/MainIntelligence/Memorable/actions/workflows/cmake.yml/badge.svg)](https://github.com/MainIntelligence/Memorable/actions/workflows/cmake.yml)
# Memorable
A library with types allowing for quick and easy implementation of advanced memory functionalities.

# Memory Management
Memorable lib provides the functionality of standard dynamic memory with a more predictable layout and some added functionality:
  
    -Maybe you want to save a group of things to disk, and be able to load them back into memory.
    
    -Maybe you want to linearize node allocation and deallocation simply and effectively.
    
    -Maybe you just want to be confident that allocations won't fail in the middle of a process.
    
  -Memorable types can take care of that for you! With good consideration for efficiency, understandability, and even binary code size.
  
To Install: -Use basic cmake commands! (See https://github.com/MainIntelligence/CMakeScripts) for a reference.
  -If using the scripts I put there, you can install just using:
    
    sudo MakeInstall
  
  To make sure you have things installed properly, you can make the build files (just run make in the Build directory)
    and use the CMTest script I left over at the link above, or otherwise follow the steps it uses.
      
      -Note the package test is not included in the CTest, since its an interactive demonstration, 
        you can modify the CMakeLists.txt in the tests directory to try it out! (a simple program that gets your name on the first run,
        then remembers it from there on out. (run it again))
