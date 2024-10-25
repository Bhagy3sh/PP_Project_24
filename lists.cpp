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
    auto temp = make_shared<Student>();
    temp->updateInfo();

    for (auto it = this->students.begin(); it != this->students.end(); it++) {
        if ((*it)->getID() == temp->getID()) {
            cout << "Student with that ID already exists!" << endl;
            return;
        }
    }

    this->students.push_back(temp);
}

void StudentList::displayStudents() {
    for (auto it = this->students.begin(); it != this->students.end(); it++) {
        cout << (*it)->getID() << " " << (*it)->getName() << endl;
    }
}

bool StudentList::remove(string prn) {
    for (auto it = this->students.begin(); it != this->students.end(); it++) {
        if (it->get()->getID() == prn) {
            this->students.erase(it);
            return true;
        }
    }

    return false;
}

weak_ptr<Student> StudentList::get(string prn) {
    for (auto it = this->students.begin(); it != this->students.end(); it++) {
        if (it->get()->getID() == prn) {
            return *it;
        }
    }

    return weak_ptr<Student>();
}

void FacultyList::add() {
    auto temp = make_shared<Faculty>();
    temp->updateInfo();

    for (auto it = this->faculties.begin(); it != this->faculties.end(); it++) {
        if ((*it)->getID() == temp->getID()) {
            cout << "Faculty with that ID already exists!" << endl;
            return;
        }
    }

    this->faculties.push_back(temp);
}

void FacultyList::displayFaculty() {
    for (auto it = this->faculties.begin(); it != this->faculties.end(); it++) {
        cout << (*it)->getID() << " " << (*it)->getName() << endl;
    }
}

bool FacultyList::isEmpty() {
    return (this->faculties.size()) == 0;
}

bool FacultyList::remove(string empID) {
    for (auto it = this->faculties.begin(); it != this->faculties.end(); it++) {
        if (it->get()->getID() == empID) {
            this->faculties.erase(it);
            return true;
        }
    }

    return false;
}

weak_ptr<Faculty> FacultyList::get(string empID) {
    for (auto it = this->faculties.begin(); it != this->faculties.end(); it++) {
        if (it->get()->getID() == empID) {
            return *it;
        }
    }

    return weak_ptr<Faculty>();
}

void CourseList::add(weak_ptr<FacultyList> fList) {
    if (auto temp = fList.lock()) {
        if (temp->isEmpty()) {
            cout << "No faculty exists yet!" << endl;
            return;
        }
    }

    char ch;
    cout << "Enter L/l for lab course, anything else for theory course: ";
    cin >> ch;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    shared_ptr<Course> temp;
    if (ch == 'L' || ch == 'l') {
        temp = make_shared<LabCourse>();
    } else {
        temp = make_shared<Course>();
    }
    temp->updateInfo(fList);

    for (auto it = this->courses.begin(); it != this->courses.end(); it++) {
        if ((*it)->getID() == temp->getID()) {
            cout << "Course with that ID already exists!" << endl;
            return;
        }
    }

    this->courses.push_back(temp);
}

void CourseList::displayCourses() {
    for (auto it = this->courses.begin(); it != this->courses.end(); it++) {
        cout << (*it)->getID() << " " << (*it)->getName() << endl;
    }
}

bool CourseList::remove(string crsID) {
    for (auto it = this->courses.begin(); it != this->courses.end(); it++) {
        if (it->get()->getID() == crsID) {
            this->courses.erase(it);
            return true;
        }
    }

    return false;
}

weak_ptr<Course> CourseList::get(string crsID) {
    for (auto it = this->courses.begin(); it != this->courses.end(); it++) {
        if (it->get()->getID() == crsID) {
            return *it;
        }
    }

    return weak_ptr<Course>();
}

vector<shared_ptr<Course>> CourseList::getList() {
    return this->courses;
}