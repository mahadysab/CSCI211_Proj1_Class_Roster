// Manages a growable array of Roster objects.
// Handles:
//   - loading/saving rosters.txt
//   - supervisor authentication via database.txt
//   - supervisor and user mode menus

#ifndef ROSTERSYSTEM_H
#define ROSTERSYSTEM_H

#include "Roster.h"
#include <string>
using namespace std;

class RosterSystem {
private:
    Roster** rosters;   // dynamic array of Roster pointers
    int      capacity;  // allocated size
    int      rosterCount; // number of rosters currently loaded

    // Doubles the rosters array capacity
    void grow();

    // File helpers
    void loadFromFile(const string& filename);
    void saveToFile(const string& filename) const;

    //  Auth helper 
    // Returns true if username/password found in database.txt
    bool authenticate(const string& user, const string& pass) const;

    // Internal finders
    // Returns index of roster with matching courseCode, or -1
    int findRosterByCode(const string& code) const;

    // Menu helpers
    void supervisorMenu();
    void userMenu();

    // Supervisor operations
    void createRoster();
    void dropRoster();
    void displayRosterInfo();
    void displayAllRosters();
    void selectRosterSupervisor();

    // Student operations within a selected roster
    void insertStudent(Roster& r);
    void removeStudent(Roster& r);
    void updateStudent(Roster& r);
    void listStudents(const Roster& r);

    // User operations 
    void selectRosterUser();

public:
    RosterSystem();
    ~RosterSystem();

    // Starts the main program loop
    void run();
};

#endif