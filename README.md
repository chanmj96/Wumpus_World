Wumpus_World
==============

Hunt the Wumpus is an early video game where the player navigates a cave in search for gold while avoiding dangerous monsters (Wumpus) and bottomless pits. This world is very often modeled in modern studies of intelligent systems, where the goal is to improve the AI of a computer which takes control of the player and tries to find gold without being killed. This is my implementation of a Wumpus World algorithm in the C++ programming language.

Requirements
------------

* Any standard C++ library and compiler.

Instructions for a Linux based operating system
----------------------------------------------

First, make sure you have the C++ library and G++ compiler installed. If you're using an Ubuntu-based Linux distribution you can use the following command to install required tools:

    sudo apt-get install build-essential

Now you can get the latest package, unpack it and compile the code using the following commands:

    wget -c https://github.com/chanmj96/Wumpus_World/archive/master.zip && unzip master.zip
    cd Wumpus_World-master
    make
    cd bin
    ./MyAI


Final Note
----------
In cases where gold is unreachable to the player, the computer will move back to the starting position to avoid loss of more points. This strategy maintains a high average score which you can see with the following commands:
    
    cd .. (should be the Wumpus_World-master directory)
    make score
