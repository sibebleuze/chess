/*
According to https://unix.stackexchange.com/questions/604260/best-range-for-custom-exit-code-in-linux
the ranges free for custom defined exit codes are:
8-63, 79-125, 166-199 and 243-255.
*/

// for now there isn't much in here, but who knows when i'll need more of these
#define EMPTY_FIELD_MOVE 80 // an empty field has moves it can make, something clearly went wrong here
#define KING_MISSING 81 // while trying to find one of the kings, the program struck out, so one of them is missing, which is impossible
#define EMPTY_TURN 82 // the parameter that keeps track of whose turn it is, is empty, which is impossible
