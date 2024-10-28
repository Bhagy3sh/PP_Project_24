#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "courses.hpp"
#include "lists.hpp"
#include "persons.hpp"

using namespace std;

void StudentList::add() {
    auto temp = make_shared<Student>(); // create a student
    temp->updateInfo();                 // read input

    // check if student with that prn already exists
    for (auto it = this->students.begin(); it != this->students.end(); it++) {
        if ((*it)->getID() == temp->getID()) {
            cout << "Student with that ID already exists!" << endl;
            return; // if exists, then print error and return
        }
    }

    this->students.push_back(temp); // otherwise add to list
}

void StudentList::displayStudents() {
    for (auto it = this->students.begin(); it != this->students.end(); it++) {
        cout << (*it)->getID() << " " << (*it)->getName() << endl;
    }
}

bool StudentList::remove(string prn) {
    for (auto it = this->students.begin(); it != this->students.end(); it++) {
        if ((*it)->getID() == prn) {
            this->students.erase(it);
            return true; // returns true if successfully deleted
        }
    }

    return false; // otherwise false (not found)
}

weak_ptr<Student> StudentList::get(string prn) {
    for (auto it = this->students.begin(); it != this->students.end(); it++) {
        if ((*it)->getID() == prn) {
            return *it; // return found student
        }
    }

    return weak_ptr<Student>(); // otherwise return nothing
}

void FacultyList::add() {
    auto temp = make_shared<Faculty>(); // create a faculty
    temp->updateInfo();                 // read input

    // check if faculty with that empID already exists
    for (auto it = this->faculties.begin(); it != this->faculties.end(); it++) {
        if ((*it)->getID() == temp->getID()) {
            cout << "Faculty with that ID already exists!" << endl;
            return; // if exists, then print error and return
        }
    }

    this->faculties.push_back(temp); // otherwise add to list
}

void FacultyList::displayFaculty() {
    for (auto it = this->faculties.begin(); it != this->faculties.end(); it++) {
        cout << (*it)->getID() << " " << (*it)->getName() << endl;
    }
}

bool FacultyList::isEmpty() {
    return this->faculties.size() == 0;
}

bool FacultyList::remove(string empID) {
    for (auto it = this->faculties.begin(); it != this->faculties.end(); it++) {
        if ((*it)->getID() == empID) {
            this->faculties.erase(it);
            return true; // returns true if successfully deleted
        }
    }

    return false; // otherwise false (not found)
}

weak_ptr<Faculty> FacultyList::get(string empID) {
    for (auto it = this->faculties.begin(); it != this->faculties.end(); it++) {
        if ((*it)->getID() == empID) {
            return *it; // return found faculty
        }
    }

    return weak_ptr<Faculty>(); // otherwise return nothing
}

void CourseList::add(shared_ptr<FacultyList> facultyList) {
    // first check if no faculty exists, then exit with error
    if (facultyList->isEmpty()) {
        cout << "No faculty exists yet!" << endl;
        return;
    }

    char ch;
    cout << "Enter L/l for lab course, anything else for theory course: ";
    cin >> ch;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    shared_ptr<Course> temp;
    if (ch == 'L' || ch == 'l') { // create lab or normal course depending on input
        temp = make_shared<LabCourse>();
    } else {
        temp = make_shared<Course>();
    }
    temp->updateInfo(facultyList); // read input

    // check if course with that id already exists
    for (auto it = this->courses.begin(); it != this->courses.end(); it++) {
        if ((*it)->getID() == temp->getID()) {
            cout << "Course with that ID already exists!" << endl;
            return; // if exists, then print error and return
        }
    }

    this->courses.push_back(temp); // otherwise add to list
}

void CourseList::displayCourses() {
    for (auto it = this->courses.begin(); it != this->courses.end(); it++) {
        cout << (*it)->getID() << " " << (*it)->getName() << endl;
    }
}

bool CourseList::remove(string crsID) {
    for (auto it = this->courses.begin(); it != this->courses.end(); it++) {
        if ((*it)->getID() == crsID) {
            this->courses.erase(it);
            return true; // returns true if successfully deleted
        }
    }

    return false; // otherwise false (not found)
}

weak_ptr<Course> CourseList::get(string crsID) {
    for (auto it = this->courses.begin(); it != this->courses.end(); it++) {
        if ((*it)->getID() == crsID) {
            return *it; // return found course
        }
    }

    return weak_ptr<Course>(); // otherwise return nothing
}

vector<shared_ptr<Course>> CourseList::getList() {
    return this->courses;
}