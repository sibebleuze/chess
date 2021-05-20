/*
According to https://unix.stackexchange.com/questions/604260/best-range-for-custom-exit-code-in-linux
the ranges free for custom defined exit codes are:
8-63, 79-125, 166-199 and 243-255.
*/

// an empty field has moves it can make, something clearly went wrong here
#define EMPTY_FIELD_MOVE 80
// while trying to find one of the kings, the program struck out, so one of them is missing
#define KING_MISSING 81
// a color parameter that is supposed to be white or black, is empty
#define COLOR_MISSING 82
// while starting up a client or server, the parameter that distinguishes between the two is empty
#define ONLINE_TYPE_MISSING 83
