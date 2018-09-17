Wumpus_World
==============

Hunt the Wumpus is an early video game where the player navigates a cave in search for gold avoiding dangerous monsters (Wumpus) and bottomless pits. In modern studies of intelligent systems, this world is very often modeled using various programming languages where the computer tries to intelligently find gold without being killed. This is my implementation of a Wumpus World solution in C++ programming language.

Requirements
------------

* Any standard C++ library and compiler.

Instructions for a Linux based operating system
----------------------------------------------

First make sure you have C++ library and G++ compiler installed. If you're using Ubuntu based Linux distribution you can use the following command to install required tools:

    sudo apt-get install build-essential

Now you can get the latest package, unpack it and compile the code using the following commands:

    wget -c https://github.com/chanmj96/Wumpus_World/archive/master.zip && unzip master.zip
    cd Wumpus_World-master
    make
    cd bin
    ./MyAI
