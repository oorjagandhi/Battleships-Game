# Battleships Game 

This repository contains a C implementation of the classic game Battleships. The game can be played in three different modes: Human vs. Human, Human vs. Bot, and Bot vs. Bot.

## Compilation and Execution

To compile and run the game, follow these steps (assuming a GCC environment):
1. Open your terminal.
2. Navigate to the directory containing the source code.
3. Run the following commands:

~~~bash
gcc -o Battleships battleships.c
./Battleships
~~~
## Game Features

-   **Dynamic Map Size**: The game currently supports a map size defined by the macro `MAP_SIZE`.
-   **Random Ship Placement**: Ships are placed randomly on the map at the start of each game.
-   **Multiple Game Modes**:
    -   **Human vs. Human**: Two players can play on the same computer.
    -   **Human vs. Bot**: Play against a simple AI that randomly chooses its shots.
    -   **Bot vs. Bot**: Watch two AIs battle it out.
-   **Visual Display**: The game displays the map state after each move, showing hits, misses, and the positions of sunken ships.

## Functions and Modules

The project is structured around several key functions:

-   `InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE])`: Initializes the map with randomly placed ships.
-   `FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col)`: Processes a shot fired by the player or the bot.
-   `CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE])`: Checks if the game has ended based on the state of the map and shots.
-   `PlayOneGame(int startingPlayer, int gameType)`: Manages the game flow, alternating turns between the players/bots.
-   `PlayBattleships()`: Initializes a new game or a series of games based on the selected game mode.

## Game Controls

For human players, the game inputs are:

-   **Row**: Input as an uppercase letter (e.g., A, B, C, etc.).
-   **Column**: Input as a number corresponding to the column index.

## Customization

To customize the game, such as changing the map size or adding new features, modify the `#define` directives and function implementations in the code. Adding new bot strategies can also enhance the gameplay experience.

## Contact
If you have any questions or suggestions, feel free to open an issue or contact me directly at oorjagandhi@gmail.com
