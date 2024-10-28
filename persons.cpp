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

// to read input
void Person::updateInfo() {
    cout << "Enter name: ";
    getline(cin, this->name);

    cout << "Enter age: ";
    cin >> this->age;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    this->updateInfoExtra(); // virtual function invoke, to read additional input in child class
}

void Person::display(shared_ptr<CourseList> crsList) {
    cout << "Name: " << this->name << endl;
    cout << "Age: " << this->age << endl;
    this->displayExtra(crsList); // virtual function invoke, to display additional input in child class
}

string Person::getName() {
    return this->name;
}

// student class methods

// reads prn and year of study
void Student::updateInfoExtra() {
    cout << "Enter PRN: ";
    cin >> this->prn;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter year of study: ";
    cin >> this->year;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// displays prn, year of study, and courses
void Student::displayExtra(shared_ptr<CourseList> crsList) {
    cout << "PRN: " << this->prn << endl;
    cout << "Year: " << this->year << endl;
    cout << "Enrolled Courses: " << endl;

    // for each course in the course list
    auto courseList = crsList->getList();
    for (auto it = courseList.begin(); it != courseList.end(); it++) { // iterate over the course list
        if ((*it)->isStudent(this->prn)) {                             // if course has this student, print the course
            cout << (*it)->getID() << " " << (*it)->getName() << endl;
        }
    }
}

string Student::getID() {
    return this->prn;
}

// faculty class methods

// reads empID and department
void Faculty::updateInfoExtra() {
    cout << "Enter employee ID: ";
    getline(cin, this->empID);

    cout << "Enter department: ";
    getline(cin, this->department);
}

// displays empID, department, and courses that this faculty teaches
void Faculty::displayExtra(shared_ptr<CourseList> crsList) {
    cout << "Employee ID: " << this->empID << endl;
    cout << "Department: " << this->department << endl;
    cout << "Courses taught: " << endl;

    // for each course in the course list
    auto courseList = crsList->getList();
    for (auto it = courseList.begin(); it != courseList.end(); it++) { // iterate over the course list
        if (auto fac = (*it)->getFaculty().lock()) {                   // we will get the faculty at this idx, and try to lock it
            if (fac->empID == this->empID) {                           // compare empID if locked successfully, and print if it matches
                cout << (*it)->getID() << " " << (*it)->getName() << endl;
            }
        }
    }
}

string Faculty::getID() {
    return this->empID;
}