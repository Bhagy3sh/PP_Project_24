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

void Course::updateInfoExtra() {}
void Course::displayExtra() {}

void Course::updateInfo(weak_ptr<FacultyList> fList) {
    cout << "Enter name: ";
    getline(cin, this->courseName);

    cout << "Enter ID: ";
    cin >> this->courseID;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter number of credits: ";
    cin >> this->credits;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool assigned = false;
    do {
        string facID;

        cout << "Enter ID of assigned faculty: ";
        cin >> facID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (auto fL = fList.lock()) {
            auto fac = fL->get(facID);
            if (fac.lock()) {
                this->assignedFaculty = fac;
                assigned = true;
            }
        }
    } while (!assigned);

    this->updateInfoExtra();
}

void Course::addStudent(weak_ptr<StudentList> sList) {
    string prn;

    cout << "Enter student PRN: ";
    cin >> prn;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto it = this->enrolledStudents.begin(); it != this->enrolledStudents.end(); it++) {
        if (auto st = (*it).lock()) {
            if (st->getID() == prn) {
                cout << "Student is already in the course!" << endl;
                return;
            }
        }
    }

    if (auto sPtr = sList.lock()) {
        auto st = sPtr->get(prn);
        if (st.lock()) {
            this->enrolledStudents.push_back(st);
            cout << "Student added to course." << endl;
        } else {
            cout << "Student not found!" << endl;
        }
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
                return;
            }
        }
    }

    cout << "Student not found in course!" << endl;
}

void Course::display() {
    cout << "Course Name: " << this->courseName << endl;
    cout << "Course ID: " << this->courseID << endl;
    cout << "No. of Credits: " << this->credits << endl;
    cout << "Assigned Faculty: " << this->assignedFaculty.lock()->getName() << endl;
    this->displayExtra();
}

bool Course::isStudent(string prn) {
    for (auto it = this->enrolledStudents.begin(); it != this->enrolledStudents.end(); it++) {
        if (auto sPtr = it->lock()) {
            if (sPtr->getID() == prn) {
                return true;
            }
        }
    }

    return false;
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
            cout << sPtr->getID() << " " << sPtr->getName() << endl;
        }
    }
}

weak_ptr<Faculty> Course::getFaculty() {
    return this->assignedFaculty;
}

// lab course methods

void LabCourse::updateInfoExtra() {
    cout << "Enter assigned lab: ";
    getline(cin, this->assignedLab);
}

void LabCourse::displayExtra() {
    cout << "Assigned Lab: " << this->assignedLab << endl;
}