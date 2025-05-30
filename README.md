### Terms Vs. Worms!
## A terminal PvZ-like game written in C++ (WIP)
A clone of Plants Vs. Zombies written in C++ for the terminal!
This game features your favorite programming languages as Termins (Plants), which you must strategically place along your "firewall" to successfully fend off bugs and worms (Zombies)
There are 10 waves of worms total each wave increasing in volume and difficulty of worms

supported on Linux and MacOS only (Or Windows with WSL).
=======
supported on Linux and MacOS only (Or Windows on WSL).

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
=======
## Gameplay
Your program is being attacked by a fleet of Bugs!
you can't let them get to the left side or else your program will crash
Use languages such as C-shooter, P-ice-thon and more to defend your code.
Complete all 5 waves to win.

## Controls
WASD - Move your cursor around the map
t/Space - Place a Term
p - pause/unpause
q - quit
