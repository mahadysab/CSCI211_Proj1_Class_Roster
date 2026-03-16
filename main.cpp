// main.cpp
// Project 1 - Roster Management Program
// Entry point: creates a RosterSystem and starts the main loop.
// On construction, rosters are loaded from rosters.txt.
// On destruction (program exit), rosters are saved back to rosters.txt.

#include "RosterSystem.h"

int main(int argc, char* argv[]) {
    RosterSystem system;
    system.run();
    return 0;
}