#include "Student.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;

// Standing helper 
void Student::updateStanding() {
    if      (credits <  15)  standing = "Lower Freshman";
    else if (credits <  30)  standing = "Upper Freshman";
    else if (credits <  45)  standing = "Lower Sophomore";
    else if (credits <  60)  standing = "Upper Sophomore";
    else if (credits <  75)  standing = "Lower Junior";
    else if (credits <  90)  standing = "Upper Junior";
    else if (credits < 105)  standing = "Lower Senior";
    else                     standing = "Upper Senior";
}

// Constructors 
Student::Student()
    : lastName(""), firstName(""), credits(0), gpa(0.0),
      dob(), matriculation(), studentID("") {
    updateStanding();
}

Student::Student(string last, string first, int c, double g,
                 Date d, Date m, string id)
    : lastName(last), firstName(first), credits(c), gpa(g),
      dob(d), matriculation(m), studentID(id) {
    updateStanding();
}

//  Accessors 
string Student::getLastName()      const { return lastName; }
string Student::getFirstName()     const { return firstName; }
string Student::getStanding()      const { return standing; }
int    Student::getCredits()       const { return credits; }
double Student::getGPA()           const { return gpa; }
Date   Student::getDOB()           const { return dob; }
Date   Student::getMatriculation() const { return matriculation; }
string Student::getStudentID()     const { return studentID; }

// Mutators
void Student::setLastName(const string& last)   { lastName = last; }
void Student::setFirstName(const string& first) { firstName = first; }
void Student::setStudentID(const string& id)    { studentID = id; }
void Student::setDOB(const Date& d)             { dob = d; }
void Student::setMatriculation(const Date& m)   { matriculation = m; }

void Student::setCredits(int c) {
    if (c < 0) { cout << "Invalid credits.\n"; return; }
    credits = c;
    updateStanding();
}

void Student::setGPA(double g) {
    if (g < 0.0 || g > 4.0) { cout << "Invalid GPA.\n"; return; }
    gpa = g;
}

// Comparison operators
// Compare by: last name → first name → student ID
bool Student::operator<(const Student& o) const {
    if (lastName  != o.lastName)  return lastName  < o.lastName;
    if (firstName != o.firstName) return firstName < o.firstName;
    return studentID < o.studentID;
}
bool Student::operator>(const Student& o)  const { return o < *this; }
bool Student::operator<=(const Student& o) const { return !(*this > o); }
bool Student::operator>=(const Student& o) const { return !(*this < o); }
bool Student::operator==(const Student& o) const {
    return lastName == o.lastName && firstName == o.firstName && studentID == o.studentID;
}
bool Student::operator!=(const Student& o) const { return !(*this == o); }

// Stream operators

// Prints formatted student info to any output stream
ostream& operator<<(ostream& out, const Student& s) {
    out << "  Name       : " << s.firstName << " " << s.lastName << "\n";
    out << "  ID         : " << s.studentID << "\n";
    out << "  Credits    : " << s.credits   << "\n";
    out << "  Standing   : " << s.standing  << "\n";
    out << "  GPA        : " << fixed << setprecision(2) << s.gpa << "\n";
    out << "  DOB        : " << s.dob        << "\n";
    out << "  Matriculated: " << s.matriculation << "\n";
    return out;
}

// Reads student data interactively from the user
istream& operator>>(istream& in, Student& s) {
    cout << "  First name        : "; in >> s.firstName;
    cout << "  Last name         : "; in >> s.lastName;
    cout << "  Student ID        : "; in >> s.studentID;
    cout << "  Credits           : "; in >> s.credits;
    cout << "  GPA (0.0-4.0)     : "; in >> s.gpa;
    cout << "  DOB (mm dd yyyy)  : "; in >> s.dob;
    cout << "  Matric (mm dd yyyy): "; in >> s.matriculation;
    s.updateStanding();
    return in;
}

//  File I/O

// Helper: format a date as mm/dd/yyyy for rosters.txt
static string dateToFileStr(const Date& d) {
    // zero-pad month and day
    string m  = (d.getMonthNum() < 10 ? "0" : "") + to_string(d.getMonthNum());
    string dy = (d.getDay()      < 10 ? "0" : "") + to_string(d.getDay());
    return m + "/" + dy + "/" + to_string(d.getYear());
}

// Writes one pipe delimited student line to the roster file
// Format: first|last|id|standing|credits|gpa|dob|matric
void Student::saveToFile(ofstream& out) const {
    out << firstName      << "|"
        << lastName       << "|"
        << studentID      << "|"
        << standing       << "|"
        << credits        << "|"
        << fixed << setprecision(2) << gpa << "|"
        << dateToFileStr(dob)          << "|"
        << dateToFileStr(matriculation) << "\n";
}

// Helper: parse "mm/dd/yyyy" string into a Date object
static Date parseDateStr(const string& s) {
    // s is "mm/dd/yyyy"
    int m = stoi(s.substr(0, 2));
    int d = stoi(s.substr(3, 2));
    int y = stoi(s.substr(6, 4));
    return Date(m, d, y);
}

// Helper: split a string by a delimiter character
static vector<string> splitLine(const string& line, char delim) {
    vector<string> parts;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delim)) {
        // trim leading/trailing spaces
        size_t start = token.find_first_not_of(" \t");
        size_t end   = token.find_last_not_of(" \t");
        if (start != string::npos)
            parts.push_back(token.substr(start, end - start + 1));
        else
            parts.push_back("");
    }
    return parts;
}

// Builds a Student from one pipe delimited line in rosters.txt
// Format: first|last|id|standing|credits|gpa|mm/dd/yyyy|mm/dd/yyyy
Student Student::loadFromFile(const string& line) {
    vector<string> p = splitLine(line, '|');
    if (p.size() < 8) return Student(); // safety check

    string first    = p[0];
    string last     = p[1];
    string id       = p[2];
    // p[3] = standing (recomputed automatically)
    int    credits  = stoi(p[4]);
    double gpa      = stod(p[5]);
    Date   dob      = parseDateStr(p[6]);
    Date   matric   = parseDateStr(p[7]);

    return Student(last, first, credits, gpa, dob, matric, id);
}