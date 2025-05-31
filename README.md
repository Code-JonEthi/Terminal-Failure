### Terms Vs. Worms!
## A terminal PvZ-like game written in C++
### Video Demo: https://youtu.be/HZAprc-GuDE
### Description:
A clone of Plants Vs. Zombies written in C++ for the terminal!
This game features your favorite programming languages as Termins (Plants), which you must strategically place along your "firewall" to successfully fend off bugs and worms (Zombies)
There are 10 waves of worms total each wave increasing in volume and difficulty of worms

supported on Linux and MacOS only (Or Windows with WSL).

### Installation Instructions
working on Cmake support.
Dependencies:
  gcc compiler
  g++ compiler
  ncurses
just run `g++ main.c -o TermVWorm -lncurses`.
Then to play you can run `./TermVWorm`.

### How to play
Use the arrow keys or 'hjkl' to move your cursor around the firewall.
Use the number keys to select a termin.
't' to place the selected termin on the firewalls cursor
'p' to pause the game

Make sure to place a 'Pay-HP' termin at the start to ensure you maintain cashflow and prevent softlock.
Keep track of your cash at the top of the screen to know when you can afford to place more termins.

### The Termins
Each termin is slightly unique to one another and can have different abilities.
# C-Shooter
Shoots a single bullet at a constant speed
# C-Squared
Shoots two bullets simultaneously at a constant speed
# P-ice-thon
Shoots freezing bullets that temporarily slowdown whichever worm it hits
# Tetanis Rust
Shoots a high-speed and high-damage bullet
# Pay-HP
Generates cash every couple of seconds

### The Worms
Much like the termins, the worms are all different in their health and speed
# Worm
The base worm is the middle of the road worm, not too fast, not to strong
# Beetle
The beetle has a slightly slower speed with a large amount of health, takes more than just one termin to take it down
# Mosquito
this is the super fast crazy worm that needs to be taken down asap, if you have no towers in your lane when the mosquito spawns then you've already lost..


### How it works
  This project is broken down into 4 main files, bullet.h, cursor.h, terms.h, and worm.h. All of these files are imported and used together
  in main.cpp to turn it all into a functioning game. This project makes use of the ncurses library in order to efficiently draw ascii art to the screen to make a (mostly) visually appealing game in my opinion. This project was written on and off over the span of a couple months so one bit of code may be drastically different from another. For example the bullet is written as a struct whereas the cursor is written as a class, both could be written in either way but they are different purely because of the time difference between writing these files.

# bullet.h
This is fairly simple, all it really holds is a struct for the bullet, so that each bullets position, damage, effects, and 'sprite' can be stored and manipulated separately.The bullet sctruct holds two functions update, and print. Update checks if the proper amount of time has passed for the bullet to be moved forward while the print function simply draws the bullet to the screen based on its stored type and position. The Bullet struct also has a constructor for when its initialized to take in a starting position, type, starting time, and which row its on. The row is stored so that the bullet knows it's not hitting worms on other rows.

# cursor.h
Similarly to the bullet, the cursor has two functions, one to print it to the screen and another just for moving it. The print function uses a for loop to print each line of the cursor, printing a full 'bar' of 0's only at the start and end of the loop to ensure just the edges of the cursor are highlighted. The position of the cursor is also altered in the print function to make sure the cursor aligns to a grid. The move function just takes a direction and will alter the position accordingly. And finally there is a constructor for the cursor that just takes a position and size. Although the size is never used, it was originally intended for the whole game to be resizable but I later decided that would be too difficult or impossible given the ascii nature of the game.

# terms.h
This file mainly serves as a quick and dirty way for me to store the different variations of all the termins for the game. It only has one function for printing out it's image variable (which is an array of strings) to it's position aligned to the grid. The rest of the file is it's own constructer which (very inefficiently) has a long if statement to set the stats of the term based on the type provided.

# worm.h
This file is the most complex out of the rest of the header files. There are 4 functions total, with one just being the print which does exactly the same thing as the print function in terms.h. The other 3 functions use C++'s templates to 'bypass' the static typed nature of the language, and allows me to define the type as I call it as opposed to when the function is defined. check_collisions takes a list of objects to iterate through and determine whether the worm is within the same 'zone' as any of the objects in the list and returns true if a collision is detected. This function is called by the move function, which uses it to determine whether or not to keep moving forward. If there is a collision the worm will attempt to attack whatever is in front of it. Finally is the update function which calls on the move function at a constant speed taking into account whether or not the worm is 'slowed' by a P-ice-thon. Then the constructor for the class is the same as the terms.h, just setting the stats of the worm based on the type provided.

# main.h
The file where everything comes together. There are two structs unique to this file. Those being Tile and Wave. Tile is solely for drawing tile borders on the screen and whether or not is is occupied. Wave serves as a way to organize the data for each wave and switch between each one. After that the first ~100 or so lines of the main function are just for initializing ncurses, vectors for storing each individual object, and other UI elements. The game loop on line 160 can be broken up into 4 states, start, play, pause, and end. start, pause, and end just display messages on the screen for how to exit or proceed to continue playing the game. The play function starts by checking if the wave conditions have been met and will move on to the next wave if true. Followed by handling input and updating the ui elements. It then works on updating items in the 'menu' page, which is used to select termins and check cash and wave number. Next is the actual game window which updates and renders each object individually, starting with the termins, then worms, then bullets. When updating terms it also spawns bullets when necessary. Then when updating the bullet it checks for bullet hits to deal damage to worms. It then finally renders the cursor to ensure it displays over everything else. Outside of the gameplay loop I just destruct the ncurses windows and exit the program.

