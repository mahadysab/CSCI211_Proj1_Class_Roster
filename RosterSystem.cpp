// RosterSystem.cpp
// Full implementation of the Roster Management Program

#include "RosterSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

// ---------------------------------------------------------------------------------
// Utility: clear the input buffer after bad input
// ---------------------------------------------------------------------------------
static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ---------------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------------
RosterSystem::RosterSystem()
    : capacity(5), rosterCount(0) {
    rosters = new Roster*[capacity];
    loadFromFile("rosters.txt");
}

RosterSystem::~RosterSystem() {
    saveToFile("rosters.txt");
    for (int i = 0; i < rosterCount; i++)
        delete rosters[i];
    delete[] rosters;
}

// ---------------------------------------------------------------------------------
// Grow the rosters array
// ---------------------------------------------------------------------------------
void RosterSystem::grow() {
    capacity *= 2;
    Roster** temp = new Roster*[capacity];
    for (int i = 0; i < rosterCount; i++)
        temp[i] = rosters[i];
    delete[] rosters;
    rosters = temp;
}

// ---------------------------------------------------------------------------------
// File I/O
// ---------------------------------------------------------------------------------

// Loads all rosters from rosters.txt on startup
void RosterSystem::loadFromFile(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        // File doesn't exist yet – that's fine on first run
        cout << "[INFO] " << filename << " not found. Starting fresh.\n";
        return;
    }

    string line;
    while (getline(in, line)) {
        // Strip carriage return and trailing spaces
        while (!line.empty() &&
               (line.back() == '\r' || line.back() == ' '))
            line.pop_back();

        if (line.empty()) continue;
        if (line.find("end_roster") != string::npos) continue;

        // Any non-empty, non-end_roster line is a roster header
        Roster r = Roster::loadFromFile(in, line);
        if (rosterCount == capacity) grow();
        rosters[rosterCount++] = new Roster(r);
    }

    in.close();
    cout << "[INFO] Loaded " << rosterCount << " roster(s) from " << filename << ".\n";
}

// Saves all rosters back to rosters.txt on exit
void RosterSystem::saveToFile(const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "[ERROR] Cannot write to " << filename << "\n";
        return;
    }
    for (int i = 0; i < rosterCount; i++)
        rosters[i]->saveToFile(out);
    out.close();
    cout << "[INFO] Rosters saved to " << filename << ".\n";
}

// ---------------------------------------------------------------------------------
// Authentication – checks database.txt for username|password pairs
// ---------------------------------------------------------------------------------
bool RosterSystem::authenticate(const string& user, const string& pass) const {
    ifstream db("database.txt");
    if (!db.is_open()) {
        cerr << "[WARN] database.txt not found. Access denied.\n";
        return false;
    }
    string line;
    while (getline(db, line)) {
        // Strip carriage return
        while (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty()) continue;

        // Format: username|password
        size_t sep = line.find('|');
        if (sep == string::npos) continue;

        string fileUser = line.substr(0, sep);
        string filePass = line.substr(sep + 1);

        if (fileUser == user && filePass == pass) {
            db.close();
            return true;
        }
    }
    db.close();
    return false;
}

// ---------------------------------------------------------------------------------
// Finder helpers
// ---------------------------------------------------------------------------------
int RosterSystem::findRosterByCode(const string& code) const {
    for (int i = 0; i < rosterCount; i++)
        if (rosters[i]->getCourseCode() == code)
            return i;
    return -1;
}

// ---------------------------------------------------------------------------------
// Main program loop
// ---------------------------------------------------------------------------------
void RosterSystem::run() {
    int choice;
    do {
        cout << "\n+======================================+\n";
        cout << "|     ROSTER MANAGEMENT SYSTEM         |\n";
        cout << "+======================================+\n";
        cout << "|  A) Supervisor Mode (password)       |\n";
        cout << "|  B) User Mode                        |\n";
        cout << "|  C) Exit                             |\n";
        cout << "+======================================+\n";
        cout << "Select option: ";

        char ch;
        cin >> ch;
        clearInput();
        ch = toupper(ch);

        if      (ch == 'A') supervisorMenu();
        else if (ch == 'B') userMenu();
        else if (ch == 'C') { cout << "Goodbye!\n"; break; }
        else                  cout << "Invalid option. Try again.\n";

    } while (true);
}

// ---------------------------------------------------------------------------------
// SUPERVISOR MODE
// ---------------------------------------------------------------------------------
void RosterSystem::supervisorMenu() {
    // Password authentication
    string user, pass;
    cout << "\n-- Supervisor Login --\n";
    cout << "Username: "; getline(cin, user);
    cout << "Password: "; getline(cin, pass);

    if (!authenticate(user, pass)) {
        cout << "[ACCESS DENIED] Invalid username or password.\n";
        return;
    }
    cout << "[ACCESS GRANTED] Welcome, " << user << "!\n";

    int choice;
    do {
        cout << "\n+======================================+\n";
        cout << "|          SUPERVISOR MENU             |\n";
        cout << "+======================================+\n";
        cout << "|  1) Create a new Roster              |\n";
        cout << "|  2) Drop a Roster (by course code)   |\n";
        cout << "|  3) Display one Roster's info        |\n";
        cout << "|  4) Display ALL Rosters              |\n";
        cout << "|  5) Select Roster - student ops      |\n";
        cout << "|  0) Back to main menu                |\n";
        cout << "+======================================+\n";
        cout << "Choice: ";
        cin >> choice;
        clearInput();

        switch (choice) {
            case 1: createRoster();           break;
            case 2: dropRoster();             break;
            case 3: displayRosterInfo();      break;
            case 4: displayAllRosters();      break;
            case 5: selectRosterSupervisor(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// 1) Create a new roster
void RosterSystem::createRoster() {
    cout << "\n-- Create New Roster --\n";
    string name, code, instructor;
    int credits;

    cout << "Course name    : "; getline(cin, name);
    cout << "Course code    : "; getline(cin, code);
    cout << "Credits        : "; cin >> credits; clearInput();
    cout << "Instructor name: "; getline(cin, instructor);

    // Check for duplicate course code
    if (findRosterByCode(code) != -1) {
        cout << "[ERROR] A roster with code '" << code << "' already exists.\n";
        return;
    }

    if (rosterCount == capacity) grow();
    rosters[rosterCount++] = new Roster(name, code, credits, instructor);
    cout << "[OK] Roster '" << code << "' created.\n";
}

// 2) Drop a roster by course code
void RosterSystem::dropRoster() {
    if (rosterCount == 0) { cout << "No rosters to drop.\n"; return; }

    cout << "\n-- Drop Roster --\n";
    cout << "Enter course code: ";
    string code; getline(cin, code);

    int idx = findRosterByCode(code);
    if (idx == -1) { cout << "Roster not found.\n"; return; }

    delete rosters[idx];
    // Shift remaining pointers left
    for (int i = idx; i < rosterCount - 1; i++)
        rosters[i] = rosters[i + 1];
    rosterCount--;
    cout << "[OK] Roster '" << code << "' dropped.\n";
}

// 3) Display one roster's info
void RosterSystem::displayRosterInfo() {
    if (rosterCount == 0) { cout << "No rosters available.\n"; return; }

    cout << "\n-- Display Roster Info --\n";
    cout << "Enter course code: ";
    string code; getline(cin, code);

    int idx = findRosterByCode(code);
    if (idx == -1) { cout << "Roster not found.\n"; return; }

    cout << *rosters[idx];
}

// 4) Display all rosters
void RosterSystem::displayAllRosters() {
    if (rosterCount == 0) { cout << "No rosters in the system.\n"; return; }

    cout << "\n== ALL ROSTERS ==\n\n";
    for (int i = 0; i < rosterCount; i++) {
        cout << "[" << i + 1 << "] ";
        cout << *rosters[i] << "\n";
    }
}

// 5) Select a roster and perform student operations (supervisor has all 4)
void RosterSystem::selectRosterSupervisor() {
    if (rosterCount == 0) { cout << "No rosters available.\n"; return; }

    cout << "\n-- Select Roster --\n";
    cout << "Enter course code: ";
    string code; getline(cin, code);

    int idx = findRosterByCode(code);
    if (idx == -1) { cout << "Roster not found.\n"; return; }

    Roster& r = *rosters[idx];

    int choice;
    do {
        cout << "\n -- Roster: " << r.getCourseCode()
             << " (" << r.getStudentCount() << " students) --\n";
        cout << "  a) Insert student\n";
        cout << "  b) Remove student\n";
        cout << "  c) Update student\n";
        cout << "  d) List all students (sorted)\n";
        cout << "  0) Back\n";
        cout << "Choice: ";

        char ch; cin >> ch; clearInput();
        ch = tolower(ch);

        if      (ch == 'a') insertStudent(r);
        else if (ch == 'b') removeStudent(r);
        else if (ch == 'c') updateStudent(r);
        else if (ch == 'd') listStudents(r);
        else if (ch == '0') break;
        else cout << "Invalid choice.\n";

    } while (true);
}

// ---------------------------------------------------------------------------------
// USER MODE
// ---------------------------------------------------------------------------------
void RosterSystem::userMenu() {
    if (rosterCount == 0) { cout << "No rosters available.\n"; return; }
    selectRosterUser();
}

// User mode: can only insert, remove, or update (no list-all)
void RosterSystem::selectRosterUser() {
    cout << "\n-- User Mode: Select Roster --\n";
    cout << "Enter course code: ";
    string code; getline(cin, code);

    int idx = findRosterByCode(code);
    if (idx == -1) { cout << "Roster not found.\n"; return; }

    Roster& r = *rosters[idx];

    int choice;
    do {
        cout << "\n -- Roster: " << r.getCourseCode()
             << " (" << r.getStudentCount() << " students) --\n";
        cout << "  a) Insert student\n";
        cout << "  b) Remove student\n";
        cout << "  c) Update student\n";
        cout << "  0) Back\n";
        cout << "Choice: ";

        char ch; cin >> ch; clearInput();
        ch = tolower(ch);

        if      (ch == 'a') insertStudent(r);
        else if (ch == 'b') removeStudent(r);
        else if (ch == 'c') updateStudent(r);
        else if (ch == '0') break;
        else cout << "Invalid choice.\n";

    } while (true);
}

// ---------------------------------------------------------------------------------
// Student operations (shared between supervisor and user modes)
// ---------------------------------------------------------------------------------

// Insert a new student into a roster
void RosterSystem::insertStudent(Roster& r) {
    cout << "\n-- Insert Student into " << r.getCourseCode() << " --\n";
    Student s;
    cin >> s;       // uses Student::operator>>

    // Check for duplicate ID
    if (r.searchStudent(s.getStudentID()) != nullptr) {
        cout << "[ERROR] A student with ID '" << s.getStudentID()
             << "' is already in this roster.\n";
        return;
    }

    r.addStudent(s);
    cout << "[OK] Student added and roster sorted.\n";
}

// Remove a student by ID
void RosterSystem::removeStudent(Roster& r) {
    if (r.getStudentCount() == 0) {
        cout << "Roster is empty.\n"; return;
    }
    cout << "\n -- Remove Student --\n";
    cout << "Enter student ID: ";
    string id; getline(cin, id);

    if (r.deleteStudent(id))
        cout << "[OK] Student with ID '" << id << "' removed.\n";
    else
        cout << "[ERROR] Student not found.\n";
}

// Update an existing student's information field by field
void RosterSystem::updateStudent(Roster& r) {
    if (r.getStudentCount() == 0) {
        cout << "Roster is empty.\n"; return;
    }
    cout << "\n -- Update Student --\n";
    cout << "Enter student ID to update: ";
    string id; getline(cin, id);

    Student* s = r.searchStudent(id);
    if (!s) { cout << "[ERROR] Student not found.\n"; return; }

    cout << "Current info:\n" << *s;

    cout << "\nWhat would you like to update?\n";
    cout << "  1) First name\n";
    cout << "  2) Last name\n";
    cout << "  3) Credits\n";
    cout << "  4) GPA\n";
    cout << "  5) Date of birth\n";
    cout << "  6) Matriculation date\n";
    cout << "  0) Cancel\n";
    cout << "Choice: ";

    int ch; cin >> ch; clearInput();

    switch (ch) {
        case 1: {
            cout << "New first name: ";
            string fn; getline(cin, fn);
            s->setFirstName(fn);
            break;
        }
        case 2: {
            cout << "New last name: ";
            string ln; getline(cin, ln);
            s->setLastName(ln);
            break;
        }
        case 3: {
            cout << "New credits: ";
            int c; cin >> c; clearInput();
            s->setCredits(c);
            break;
        }
        case 4: {
            cout << "New GPA: ";
            double g; cin >> g; clearInput();
            s->setGPA(g);
            break;
        }
        case 5: {
            cout << "New DOB (mm dd yyyy): ";
            Date d; cin >> d; clearInput();
            s->setDOB(d);
            break;
        }
        case 6: {
            cout << "New matriculation date (mm dd yyyy): ";
            Date m; cin >> m; clearInput();
            s->setMatriculation(m);
            break;
        }
        case 0: cout << "Update cancelled.\n"; return;
        default: cout << "Invalid choice.\n"; return;
    }

    // Re-sort roster after possible name/ID change
    r.sortStudents();
    cout << "[OK] Student updated.\n";
    cout << "Updated info:\n" << *s;
}

// List all students in sorted order (supervisor only)
void RosterSystem::listStudents(const Roster& r) {
    if (r.getStudentCount() == 0) {
        cout << "No students in this roster.\n";
        return;
    }
    cout << "\n-- Students in " << r.getCourseCode()
         << " (sorted) --\n";
    cout << r;
}