# chess

Chess game with local, engine and remote options for C++ course

Windows instructions:

Unzip the file nvwa-1.1.zip in the folder where the zip is located. Load the CMake project (your IDE probably has an
option for this). Build the project. If you are working in the command line, follow the steps below, where you replace '
cmake' and 'make' with the corresponding executables (or run them as commands if their executables are included in your
system path).

Linux instructions:

To build from source, run the following commands from the chess directory:

* ```unzip nvwa-1.1.zip``` (or get it from [the nvwa Github repo](https://github.com/adah1972/nvwa))
* ```cmake .```
* ```make```

The executable should now be in the chess directory. If any of the above steps fail due to Qt not being found, specify
the correct CMAKE_PREFIX_PATH in CMakeLists.txt (where it says "C:/Qt/5.15.2/msvc2019" now).

Chess engine:

For the chess engine to work, a stockfish executable must be in your path variable. You can test this by opening a
command prompt and typing `stockfish`. You can get Stockfish [here](https://stockfishchess.org/download/). This program
is tested to work with Stockfish 13. Stockfish is ditributed under the
[GNU General Public License v3.0](https://github.com/official-stockfish/Stockfish/blob/master/Copying.txt).

The program consists of a basic chess game, only mouse clicks are supported to interact with the program. On the right
side of the screen next to the board is a live report of the moves played
in [reversible algebraic notation](https://en.wikipedia.org/wiki/Chess_notation). You can play against a human opponent
using the same computer or play against stockfish on 3 different levels.

This program has a few custom exit codes (passed on through signals and slots by Qt until the main application is
reached and the program exits):

* 80: an empty field somehow managed to get moves from the move calculator, since this is impossible, the game doesn't
  know how to continue and exits to prevent further unexpected behaviour
* 81: while trying to find one of the kings, the program struck out, so one of them is missing, which is impossible;
  here too an exit is triggered to prevent further unexpected behaviour
* 82: a parameter that should be white or black, is empty, which is undefined behaviour, so any further unpredictable
  action is avoided by exiting
* 83: a parameter that should distinguish between client and server mode is empty, since the program needs this
  information to continue, it just breaks off here
* 84: while trying to start up stockfish as a command line process, an error occurred, since this is probably due to
  stockfish being absent, the program does not retry but exits instead
* 85: stockfish normally gets 30 seconds to respond after each command and didn't respond in time here
* 86: the separator between the origin and the destination field in the history table is missing for one or more moves
* 87: the stockfish engine was ordered to play a move after the game has finished, it is not supposed to be

These situations are all impossible to reach as far as I know (except for 84 and maybe 85 with stockfish), but who knows
where I might have made a mistake. If any of the above situations occur, the program will exit. Please do let me know if
this happens (by submitting an issue on the [Github repo](https://github.com/sibebleuze/chess)). Keep in mind that
problems with stockfish are especially hard to fix, since the stockfish output is not reproducible.
