// Date.h

#ifndef DATE_H
#define DATE_H

// Holds a date, which is a month-day-year combination.  There
// are also certain functions for manipulating dates that are
// associated with dates

#include <string>
#include <iostream>
using namespace std;

class Date {
public:
    // Default constructor: January 1, 1970
    Date();

    // Constructor from day-of-year and year
    Date(int dayNum, int y);

    // Constructor from month, day, year
    Date(int m, int d, int y);

    // Returns month name as string
    string getMonth() const;

    // Returns numeric month derived from dayOfYear
    int getMonthNum() const;

    // Returns day as integer
    int getDay() const;

    // Returns year as integer
    int getYear() const;

    // Returns a new Date that is n days forward
    Date addDays(int n) const;

    // Returns which day of the year this date falls on
    int dayOfYear() const;

    // Converts a Date object to a string /static
    static string toString(const Date d);

    // Stream operators added in HW5
    friend ostream& operator<<(ostream& out, const Date& d);
    friend istream& operator>>(istream& in, Date& d);

 private:
      // Fixes the date so that it is within legal range
      void fixDate();

      // Helper function for toString(const Date)
      static string toString(int);

      int month, day, year;
};

#endif