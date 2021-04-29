# chess

Chess game with local, engine and remote options for C++ course

Custom exit codes:

* 80: an empty field somehow managed to get moves from the move calculator, since this is impossible, the game doesn't
  know how to continue and exits to prevent further unexpected behaviour
* 81: while trying to find one of the kings, the program struck out, so one of them is missing, which is impossible;
  here too an exit is triggered to prevent further unexpected behaviour
* 82: the parameter that keeps track of whose turn it is, is empty, which is undefined behaviour, so any further
  unpredictable action is avoided by exiting

To build from source, run the following commands from the chess directory:

* unzip nvwa-1.1.zip
* cmake .
* make The executable should now be in the chess directory.
