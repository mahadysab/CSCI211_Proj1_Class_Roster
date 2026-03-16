// Date.cpp

#include "Date.h"
#include <iostream>
using namespace std;

// Initializes a date to the default value of January 1, 1970
Date::Date() {
   month = 1;;
   day = 1;
   year = 1970;
}
// Initializes the class taking in a day number (over the whole year)
// and a year number.
Date::Date(int dayNum, int y) {
    if (dayNum < 1) dayNum = 1;
    if ((y % 4 == 0) && (dayNum > 366)) dayNum = 366;
    else if (dayNum > 365)              dayNum = 365;
    year = y;

    if (dayNum <= 31) { month = 1; day = dayNum; return; }
    dayNum -= 31;
    if ((year % 4 == 0) && (dayNum <= 29)) { day = dayNum; month = 2; return; }
    else if (year % 4 == 0)                  dayNum -= 29;
    else if (dayNum <= 28)                  { day = dayNum; month = 2; return; }
    else                                      dayNum -= 28;

    if (dayNum <= 31) { month = 3; day = dayNum; return; } dayNum -= 31;
    if (dayNum <= 30) { month = 4; day = dayNum; return; } dayNum -= 30;
    if (dayNum <= 31) { month = 5; day = dayNum; return; } dayNum -= 31;
    if (dayNum <= 30) { month = 6; day = dayNum; return; } dayNum -= 30;
    if (dayNum <= 31) { month = 7; day = dayNum; return; } dayNum -= 31;
    if (dayNum <= 31) { month = 8; day = dayNum; return; } dayNum -= 31;
    if (dayNum <= 30) { month = 9; day = dayNum; return; } dayNum -= 30;
    if (dayNum <= 31) { month = 10; day = dayNum; return; } dayNum -= 31;
    if (dayNum <= 30) { month = 11; day = dayNum; return; }
    month = 12; day = dayNum - 30;
}

// Initializes a date to the values in the parameters.  If the
// date is not legal, sets the date to 1 of the illegal value.
// (Illegal years are set to 2000)
Date::Date(int m, int d, int y) {
   month = m;
   day = d;
   year = y;
   fixDate();
}


// Returns the month stored by the class
string Date::getMonth() const { 
   if (month == 1)
      return "January";
   if (month == 2)
      return "February";
   if (month == 3)
      return "March";
   if (month == 4)
      return "April";
   if (month == 5)
      return "May";
   if (month == 6)
      return "June";
   if (month == 7)
      return "July";
   if (month == 8)
      return "August";
   if (month == 9)
      return "September";
   if (month == 10)
      return "October";
   if (month == 11)
      return "November";
   return "December";
}

// Returns numeric month (1-12)
int Date::getMonthNum() const { return month; }

// Returns day
int Date::getDay() const { return day; }

// Returns year
int Date::getYear() const { return year; }

// Returns a Date n days forward
Date Date::addDays(int n) const {
    int dayNum   = dayOfYear() + n;
    int tempYear = year;
    while (dayNum < 1) {
        tempYear--;
        dayNum += (tempYear % 4 == 0) ? 366 : 365;
    }
    while (((tempYear % 4 == 0) && (dayNum > 366)) ||
           ((tempYear % 4 != 0) && (dayNum > 365))) {
        dayNum -= (tempYear % 4 == 0) ? 366 : 365;
        tempYear++;
    }
    return Date(dayNum, tempYear);
}

// Returns which day of the year
int Date::dayOfYear() const {
    int temp = day;
    if (month == 1)  return temp;
    temp += 31;
    if (month == 2)  return temp;
    temp += 28 + (year % 4 == 0 ? 1 : 0);
    if (month == 3)  return temp;
    temp += 31; if (month == 4)  return temp;
    temp += 30; if (month == 5)  return temp;
    temp += 31; if (month == 6)  return temp;
    temp += 30; if (month == 7)  return temp;
    temp += 31; if (month == 8)  return temp;
    temp += 31; if (month == 9)  return temp;
    temp += 30; if (month == 10) return temp;
    temp += 31; if (month == 11) return temp;
    temp += 30;
    return temp;
}

// Clamps values to legal ranges
void Date::fixDate() {
    if (month < 1)  month = 1;
    if (month > 12) month = 12;
    if (day < 1)    day = 1;
    if (day > 31)   day = 31;
    if (((month==4)||(month==6)||(month==9)||(month==11)) && day > 30) day = 30;
    if ((month==2) && (year % 4 != 0) && day > 28) day = 28;
    if ((month==2) && (year % 4 == 0) && day > 29) day = 29;
}

// Private helper: int to string
string Date::toString(int i) {
    if (i == 0) return "0";
    string result = "";
    bool neg = (i < 0);
    if (neg) i = -i;
    while (i > 0) { result = char('0' + i % 10) + result; i /= 10; }
    if (neg) result = "-" + result;
    return result;
}

// Static: Date to string "Month day, year"
string Date::toString(const Date d) {
    return d.getMonth() + " " + toString(d.getDay()) + ", " + toString(d.getYear());
}

// Output operator: prints "Month day, year"
ostream& operator<<(ostream& out, const Date& d) {
    out << d.getMonth() << " " << d.getDay() << ", " << d.getYear();
    return out;
}

// Input operator: reads "month day year" (numeric)
istream& operator>>(istream& in, Date& d) {
    int m, day, y;
    in >> m >> day >> y;
    d = Date(m, day, y);
    return in;
}