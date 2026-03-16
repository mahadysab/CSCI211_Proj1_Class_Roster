//    dynamic array of Student* pointers
//    capacity grows automatically via grow()
//    Big Three (destructor, copy ctor, assignment op)
//    selection sort using Student::operator<
//    [] operator (immutable access)
//    << stream output
//    file I/O for rosters.txt

#ifndef ROSTER_H
#define ROSTER_H

#include "Student.h"
#include <string>
#include <fstream>
using namespace std;

class Roster {
private:
    string   courseName;
    string   courseCode;
    int      courseCredits;
    string   instructorName;

    Student** students;    // dynamic array of pointers to Student
    int       capacity;    // current allocated capacity
    int       studentCount;// number of students actually stored

    // Doubles the capacity of the students array
    void grow();

    // Selection sort using Student::operator<
    void sort();

public:
    //  Constructors / Destructor
    Roster();
    Roster(string name, string code, int credits, string instructor);

    ~Roster();                          // Destructor
    Roster(const Roster& other);        // Copy constructor
    Roster& operator=(const Roster& other); // Assignment operator

    // Accessors 
    string getCourseName()     const;
    string getCourseCode()     const;
    int    getCourseCredits()  const;
    string getInstructorName() const;
    int    getStudentCount()   const;

    //  Mutators
    void setCourseName(const string& name);
    void setCourseCode(const string& code);
    void setCourseCredits(int credits);
    void setInstructorName(const string& instructor);

    // Student operations
    bool     addStudent(const Student& s);
    bool     deleteStudent(const string& id);
    Student* searchStudent(const string& id) const;
    Student* searchStudentByName(const string& first, const string& last) const;

    // Public wrapper so main can trigger a sort
    void sortStudents();

    // Immutable access by index 
    const Student& operator[](int index) const;

    // Stream / file operators 
    // << prints the roster in human-readable form
    friend ostream& operator<<(ostream& out, const Roster& r);

    // Saves roster to an already-open ofstream in rosters.txt format
    void saveToFile(ofstream& out) const;

    // Reads one roster block from an already open ifstream
    static Roster loadFromFile(ifstream& in, const string& headerLine);
};

#endif