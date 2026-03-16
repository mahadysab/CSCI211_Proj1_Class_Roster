#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <iostream>
#include <fstream>
#include "Date.h"
using namespace std;

class Student {
private:
    string lastName;
    string firstName;
    string standing;       // auto-computed from credits
    int    credits;
    double gpa;
    Date   dob;            // date of birth  (Date object)
    Date   matriculation;  // date of matriculation (Date object)
    string studentID;

    // Recalculates standing whenever credits change
    void updateStanding();

public:
    // Default constructor
    Student();

    // parameterized constructor
    Student(string last, string first, int credits, double gpa,
            Date dob, Date matric, string id);

    // Accessors 
    string getLastName()      const;
    string getFirstName()     const;
    string getStanding()      const;
    int    getCredits()       const;
    double getGPA()           const;
    Date   getDOB()           const;
    Date   getMatriculation() const;
    string getStudentID()     const;

    // Mutators
    void setLastName(const string& last);
    void setFirstName(const string& first);
    void setCredits(int c);
    void setGPA(double g);
    void setDOB(const Date& d);
    void setMatriculation(const Date& m);
    void setStudentID(const string& id);

    //  Comparison operators (by last, first, id) ──────────────
    bool operator==(const Student& o) const;
    bool operator!=(const Student& o) const;
    bool operator< (const Student& o) const;
    bool operator> (const Student& o) const;
    bool operator<=(const Student& o) const;
    bool operator>=(const Student& o) const;

    // ── Stream operators ───────────────────────────────────────
    // << prints human-readable student info
    // >> reads interactively from user
    friend ostream& operator<<(ostream& out, const Student& s);
    friend istream& operator>>(istream& in,  Student& s);

    // File I/O (rosters.txt pipe-delimited format) 
    // Writes one line to file: first|last|id|standing|credits|gpa|mm/dd/yyyy|mm/dd/yyyy
    void saveToFile(ofstream& out) const;

    // Reads one line from file in the same format
    static Student loadFromFile(const string& line);
};

#endif