#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "courses.hpp"
#include "lists.hpp"
#include "persons.hpp"

using namespace std;

// course methods

// these methods do nothing in the base class, so they are empty, not pure virtual functions
void Course::updateInfoExtra() {}
void Course::displayExtra() {}

void Course::updateInfo(shared_ptr<FacultyList> facultyList) {
    cout << "Enter name: ";
    getline(cin, this->courseName);

    cout << "Enter ID: ";
    cin >> this->courseID;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter number of credits: ";
    cin >> this->credits;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // this is used to store whether a faculty was found yet or not
    bool assigned = false;
    do {
        string facID;

        cout << "Enter ID of assigned faculty: ";
        cin >> facID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        auto fac = facultyList->get(facID);
        if (fac.lock()) {
            this->assignedFaculty = fac;
            assigned = true;
        }
    } while (!assigned); // loop until found

    this->updateInfoExtra(); // virtual function invoke, to read additional input in child class
}

void Course::addStudent(shared_ptr<StudentList> studentList) {
    string prn;

    cout << "Enter student PRN: ";
    cin >> prn;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto it = this->enrolledStudents.begin(); it != this->enrolledStudents.end(); it++) {
        if (auto st = (*it).lock()) {
            if (st->getID() == prn) {
                cout << "Student is already in the course!" << endl;
                return; // returns if student with matching PRN was found in the course
            }
        }
    }

    auto st = studentList->get(prn);
    if (st.lock()) { // check if this student exists in the student list
        this->enrolledStudents.push_back(st); // if yes, then add
        cout << "Student added to course." << endl;
    } else {
        cout << "Student not found!" << endl; // otherwise print an error message
    }
}

void Course::removeStudent() {
    string prn;

    cout << "Enter student PRN: ";
    cin >> prn;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto it = this->enrolledStudents.begin(); it != this->enrolledStudents.end(); it++) {
        if (auto st = (*it).lock()) {
            if (st->getID() == prn) {
                this->enrolledStudents.erase(it);
                cout << "Student removed from course." << endl;
                return; // if student with matching PRN was found in the course, then we delete and return
            }
        }
    }

    cout << "Student not found in course!" << endl; // otherwise no prn was matching, print error
}

void Course::display() {
    cout << "Course Name: " << this->courseName << endl;
    cout << "Course ID: " << this->courseID << endl;
    cout << "No. of Credits: " << this->credits << endl;
    auto sPtr = this->assignedFaculty.lock();
    // if faculty was not locked successfully, it must mean that it was deleted, so we will display None instead
    cout << "Assigned Faculty: " << (sPtr ? sPtr->getName() : "None") << endl;
    this->displayExtra(); // virtual function invoke, to display additional input in child class
}

bool Course::isStudent(string prn) {
    for (auto it = this->enrolledStudents.begin(); it != this->enrolledStudents.end(); it++) {
        if (auto sPtr = it->lock()) {
            if (sPtr->getID() == prn) {
                return true; // if prn matched, return true
            }
        }
    }

    return false; // otherwise no prn was matching, return false
}

string Course::getID() {
    return this->courseID;
}

string Course::getName() {
    return this->courseName;
}

void Course::displayStudents() {
    for (auto it = this->enrolledStudents.begin(); it != this->enrolledStudents.end(); it++) {
        if (auto sPtr = it->lock()) {
            cout << sPtr->getID() << " " << sPtr->getName() << endl; // print prn and name of enrolled students
        }
    }
}

weak_ptr<Faculty> Course::getFaculty() {
    return this->assignedFaculty;
}

// lab course methods

// reads assigned lab
void LabCourse::updateInfoExtra() {
    cout << "Enter assigned lab: ";
    getline(cin, this->assignedLab);
}

// displays assigned lab
void LabCourse::displayExtra() {
    cout << "Assigned Lab: " << this->assignedLab << endl;
}