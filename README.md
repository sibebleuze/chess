# chess

Chess game with local, engine and remote options for C++ course

Custom exit codes:
80: an empty field somehow managed to get moves from the move calculator, since this is impossible, the game doesn't
know how to continue and exits to prevent further unexpected behaviour 81: while trying to find one of the kings, the
program struck out, so one of them is missing, which is impossible; here too an exit is triggered to prevent further
unexpected behaviour
