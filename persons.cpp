#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "courses.hpp"
#include "lists.hpp"
#include "persons.hpp"

using namespace std;

// person class methods

void Person::updateInfo() {
    cout << "Enter name: ";
    getline(cin, this->name);

    cout << "Enter age: ";
    cin >> this->age;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    this->updateInfoExtra();
}

void Person::display(weak_ptr<CourseList> cList) {
    cout << "Name: " << this->name << endl;
    cout << "Age: " << this->age << endl;
    this->displayExtra(cList);
}

string Person::getName() {
    return this->name;
}

// student class methods

void Student::updateInfoExtra() {

    cout << "Enter PRN: ";
    cin >> this->prn;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter year of study: ";
    cin >> this->year;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Student::displayExtra(weak_ptr<CourseList> cList) {
    cout << "PRN: " << this->prn << endl;
    cout << "Year: " << this->year << endl;
    cout << "Enrolled Courses: " << endl;

    // for each course in the course list
    if (auto sptr = cList.lock()) {
        auto cList = sptr->getList();                            // get the actual list if locking successful
        for (auto it = cList.begin(); it != cList.end(); it++) { // iterate over the course list
            if ((*it)->isStudent(this->prn)) {                   // if course has this student, print the course
                cout << (*it)->getID() << " " << (*it)->getName() << endl;
            }
        }
    }
}

string Student::getID() {
    return this->prn;
}

// faculty class methods

void Faculty::updateInfoExtra() {
    cout << "Enter employee ID: ";
    getline(cin, this->empID);

    cout << "Enter department: ";
    getline(cin, this->department);
}

void Faculty::displayExtra(weak_ptr<CourseList> cList) {
    cout << "Employee ID: " << this->empID << endl;
    cout << "Department: " << this->department << endl;
    cout << "Courses taught: " << endl;

    // for each course in the course list
    if (auto sptr = cList.lock()) {
        auto cList = sptr->getList();                            // get the actual list if locking successful
        for (auto it = cList.begin(); it != cList.end(); it++) { // iterate over the course list
            if (auto fac = (*it)->getFaculty().lock()) {
                if (fac->empID == this->empID) { // compare empID if locked successfully, and print if match
                    cout << (*it)->getID() << " " << (*it)->getName() << endl;
                }
            }
        }
    }
}

string Faculty::getID() {
    return this->empID;
}