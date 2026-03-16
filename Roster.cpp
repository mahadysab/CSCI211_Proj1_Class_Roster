// Roster.cpp
// Full implementation of the Roster class

#include "Roster.h"
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

// ── Private helpers ────────────────────────────────────────────────────────

// Doubles the internal array capacity
void Roster::grow() {
    capacity *= 2;
    Student** temp = new Student*[capacity];
    for (int i = 0; i < studentCount; i++)
        temp[i] = students[i];
    delete[] students;
    students = temp;
}

// Selection sort using Student::operator<
// Sorts by last name → first name → student ID
void Roster::sort() {
    for (int i = 0; i < studentCount - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < studentCount; j++) {
            if (*students[j] < *students[minIdx])
                minIdx = j;
        }
        if (minIdx != i) {
            Student* tmp    = students[i];
            students[i]     = students[minIdx];
            students[minIdx] = tmp;
        }
    }
}

// ── Constructors / Destructor ──────────────────────────────────────────────

Roster::Roster()
    : courseName(""), courseCode(""), courseCredits(0),
      instructorName(""), capacity(5), studentCount(0) {
    students = new Student*[capacity];
}

Roster::Roster(string name, string code, int credits, string instructor)
    : courseName(name), courseCode(code), courseCredits(credits),
      instructorName(instructor), capacity(5), studentCount(0) {
    students = new Student*[capacity];
}

// Destructor: free every Student, then free the pointer array
Roster::~Roster() {
    for (int i = 0; i < studentCount; i++)
        delete students[i];
    delete[] students;
}

// Deep copy constructor
Roster::Roster(const Roster& other)
    : courseName(other.courseName), courseCode(other.courseCode),
      courseCredits(other.courseCredits), instructorName(other.instructorName),
      capacity(other.capacity), studentCount(other.studentCount) {
    students = new Student*[capacity];
    for (int i = 0; i < studentCount; i++)
        students[i] = new Student(*other.students[i]);
}

// Deep copy assignment operator
Roster& Roster::operator=(const Roster& other) {
    if (this == &other) return *this;

    // Free existing students
    for (int i = 0; i < studentCount; i++)
        delete students[i];
    delete[] students;

    courseName     = other.courseName;
    courseCode     = other.courseCode;
    courseCredits  = other.courseCredits;
    instructorName = other.instructorName;
    capacity       = other.capacity;
    studentCount   = other.studentCount;

    students = new Student*[capacity];
    for (int i = 0; i < studentCount; i++)
        students[i] = new Student(*other.students[i]);

    return *this;
}

// ── Accessors ──────────────────────────────────────────────────────────────
string Roster::getCourseName()     const { return courseName; }
string Roster::getCourseCode()     const { return courseCode; }
int    Roster::getCourseCredits()  const { return courseCredits; }
string Roster::getInstructorName() const { return instructorName; }
int    Roster::getStudentCount()   const { return studentCount; }

// ── Mutators ───────────────────────────────────────────────────────────────
void Roster::setCourseName(const string& n)     { courseName = n; }
void Roster::setCourseCode(const string& c)     { courseCode = c; }
void Roster::setCourseCredits(int c)            { courseCredits = c; }
void Roster::setInstructorName(const string& i) { instructorName = i; }

// ── Student operations ─────────────────────────────────────────────────────

// Adds a student (deep copy), grows if needed, then re-sorts
bool Roster::addStudent(const Student& s) {
    if (studentCount == capacity) grow();
    students[studentCount++] = new Student(s);
    sort();
    return true;
}

// Deletes a student by ID; shifts remaining pointers left
bool Roster::deleteStudent(const string& id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->getStudentID() == id) {
            delete students[i];
            for (int j = i; j < studentCount - 1; j++)
                students[j] = students[j + 1];
            studentCount--;
            return true;
        }
    }
    return false;
}

// Returns a pointer to the student with matching ID, or nullptr
Student* Roster::searchStudent(const string& id) const {
    for (int i = 0; i < studentCount; i++)
        if (students[i]->getStudentID() == id)
            return students[i];
    return nullptr;
}

// Returns a pointer to the student with matching name, or nullptr
Student* Roster::searchStudentByName(const string& first, const string& last) const {
    for (int i = 0; i < studentCount; i++)
        if (students[i]->getFirstName() == first &&
            students[i]->getLastName()  == last)
            return students[i];
    return nullptr;
}

// Public wrapper to trigger sort from outside
void Roster::sortStudents() { sort(); }

// Immutable index operator
const Student& Roster::operator[](int index) const {
    return *students[index];
}

// ── Output operator ────────────────────────────────────────────────────────
ostream& operator<<(ostream& out, const Roster& r) {
    out << "-------------------------------------------------\n";
    out << "  Course    : " << r.courseName
        << " (" << r.courseCode << ")\n";
    out << "  Instructor: " << r.instructorName << "\n";
    out << "  Credits   : " << r.courseCredits  << "\n";
    out << "  Students  : " << r.studentCount   << "\n";
    out << "-------------------------------------------------\n";

    for (int i = 0; i < r.studentCount; i++) {
        out << "  [" << i + 1 << "]\n";
        out << *r.students[i];
        out << "  -------------------------------------------------\n";
    }
    return out;
}

// ── File I/O ───────────────────────────────────────────────────────────────

// Helper: split a string by a delimiter
static vector<string> splitRosterLine(const string& line, char delim) {
    vector<string> parts;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delim)) {
        size_t start = token.find_first_not_of(" \t");
        size_t end   = token.find_last_not_of(" \t");
        if (start != string::npos)
            parts.push_back(token.substr(start, end - start + 1));
        else
            parts.push_back("");
    }
    return parts;
}

// Saves this roster to file in the required format:
//   courseName | courseCode | credits | instructor
//   first|last|id|standing|credits|gpa|dob|matric
//   end_roster|
void Roster::saveToFile(ofstream& out) const {
    out << courseName     << " | "
        << courseCode     << " | "
        << courseCredits  << " | "
        << instructorName << "\n";

    for (int i = 0; i < studentCount; i++)
        students[i]->saveToFile(out);

    out << "end_roster|\n";
}

// Reads one roster block from file.
// headerLine is already read by caller (the course info line).
// Reads student lines until "end_roster|".
Roster Roster::loadFromFile(ifstream& in, const string& headerLine) {
    // Parse header: courseName | courseCode | credits | instructor
    vector<string> h = splitRosterLine(headerLine, '|');
    string name       = (h.size() > 0) ? h[0] : "";
    string code       = (h.size() > 1) ? h[1] : "";
    int    credits    = (h.size() > 2) ? stoi(h[2]) : 0;
    string instructor = (h.size() > 3) ? h[3] : "";

    Roster r(name, code, credits, instructor);

    string line;
    while (getline(in, line)) {
        // Strip trailing whitespace / carriage return
        while (!line.empty() &&
               (line.back() == '\r' || line.back() == ' ' || line.back() == '\t'))
            line.pop_back();

        if (line.empty()) continue;

        // End-of-roster marker
        if (line.find("end_roster") != string::npos) break;

        // Otherwise it is a student line
        Student s = Student::loadFromFile(line);
        r.addStudent(s);
    }

    return r;
}