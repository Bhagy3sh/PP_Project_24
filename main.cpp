#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "courses.hpp"
#include "lists.hpp"
#include "persons.hpp"

using namespace std;

const char *MAIN_MENU_STRING = "\n1) Student Management\n2) Faculty Management\n3) Course Management\nOther) Quit\nEnter your choice: ";
const char *STUDENT_MENU_STRING = "\n1) Add Student\n2) Remove Student\n3) Update Student Details\n4) View Student Details\n5) List all Students\nOther) Back\nEnter your choice: ";
const char *FACULTY_MENU_STRING = "\n1) Add Faculty\n2) Remove Faculty\n3) Update Faculty Details\n4) View Faculty Details\n5) List all Faculties\nOther) Back\nEnter your choice: ";
const char *COURSE_MENU_STRING = "\n1) Add Course\n2) Remove Course\n3) Update Course Details\n4) View Course Details\n5) List all Courses\n6) Add Student to Course\n7) Remove Student from Course\n8) View Enrolled Students\nOther) Back\nEnter your choice: ";

shared_ptr<StudentList> studentList;
shared_ptr<FacultyList> facultyList;
shared_ptr<CourseList> courseList;

void studentMenu() {
    while (true) {
        char choice;

        cout << STUDENT_MENU_STRING;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

        switch (choice) {
        case '1': {
            studentList->add();
            break;
        }
        case '2': {
            string prn;

            cout << "Enter PRN of student to remove: ";
            cin >> prn;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (studentList->remove(prn)) {
                cout << "Successfully removed!" << endl;
            } else {
                cout << "Student not found!" << endl;
            }

            break;
        }
        case '3': {
            string prn;

            cout << "Enter PRN of student to update: ";
            cin >> prn;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (auto sPtr = studentList->get(prn).lock()) {
                sPtr->updateInfo();
            } else {
                cout << "Student not found!" << endl;
            }

            break;
        }
        case '4': {
            string prn;

            cout << "Enter PRN of student to display: ";
            cin >> prn;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (auto sPtr = studentList->get(prn).lock()) {
                sPtr->display(courseList);
            } else {
                cout << "Student not found!" << endl;
            }

            break;
        }
        case '5': {
            cout << "Students: " << endl;
            studentList->displayStudents();
            break;
        }
        default: {
            return;
        }
        }
    }
}

void facultyMenu() {
    while (true) {
        char choice;

        cout << FACULTY_MENU_STRING;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

        switch (choice) {
        case '1': {
            facultyList->add();
            break;
        }
        case '2': {
            string empID;

            cout << "Enter employee ID of faculty to remove: ";
            cin >> empID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (facultyList->remove(empID)) {
                cout << "Successfully removed!" << endl;
            } else {
                cout << "Faculty not found!" << endl;
            }

            break;
        }
        case '3': {
            string empID;

            cout << "Enter employee ID of faculty to update: ";
            cin >> empID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (auto sPtr = facultyList->get(empID).lock()) {
                sPtr->updateInfo();
            } else {
                cout << "Faculty not found!" << endl;
            }

            break;
        }
        case '4': {
            string empID;

            cout << "Enter employee ID of faculty to display: ";
            cin >> empID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (auto sPtr = facultyList->get(empID).lock()) {
                sPtr->display(courseList);
            } else {
                cout << "Faculty not found!" << endl;
            }

            break;
        }
        case '5': {
            cout << "Faculties: " << endl;
            facultyList->displayFaculty();
            break;
        }
        default: {
            return;
        }
        }
    }
}

void courseMenu() {
    while (true) {
        char choice;

        cout << COURSE_MENU_STRING;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

        switch (choice) {
        case '1': {
            courseList->add(facultyList);
            break;
        }
        case '2': {
            string crsID;

            cout << "Enter course ID of course to remove: ";
            cin >> crsID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (courseList->remove(crsID)) {
                cout << "Successfully removed!" << endl;
            } else {
                cout << "Course not found!" << endl;
            }

            break;
        }
        case '3': {
            string crsID;

            cout << "Enter course ID of course to update: ";
            cin >> crsID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (auto sPtr = courseList->get(crsID).lock()) {
                sPtr->updateInfo(facultyList);
            } else {
                cout << "Course not found!" << endl;
            }

            break;
        }
        case '4': {
            string crsID;

            cout << "Enter course ID of course to display: ";
            cin >> crsID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (auto sPtr = courseList->get(crsID).lock()) {
                sPtr->display();
            } else {
                cout << "Course not found!" << endl;
            }

            break;
        }
        case '5': {
            cout << "Courses: " << endl;
            courseList->displayCourses();
            break;
        }

        case '6': {
            string crsID;

            cout << "Enter course ID to which student will be added: ";
            cin >> crsID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            auto crs = courseList->get(crsID);
            if (auto sPtr = crs.lock()) {
                sPtr->addStudent(studentList);
            } else {
                cout << "Course not found!" << endl;
            }
            break;
        }

        case '7': {
            string crsID;

            cout << "Enter course ID from which student will be removed: ";
            cin >> crsID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            auto crs = courseList->get(crsID);
            if (auto sPtr = crs.lock()) {
                sPtr->removeStudent();
            } else {
                cout << "Course not found!" << endl;
            }
            break;
        }

        case '8': {
            string crsID;

            cout << "Enter course ID for which students will be displayed: ";
            cin >> crsID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            auto crs = courseList->get(crsID);
            if (auto sPtr = crs.lock()) {
                sPtr->displayStudents();
            } else {
                cout << "Course not found!" << endl;
            }
            break;
        }

        default: {
            return;
        }
        }
    }
}

int main() {
    studentList = make_shared<StudentList>();
    facultyList = make_shared<FacultyList>();
    courseList = make_shared<CourseList>();

    while (true) {
        char choice;
        cout << MAIN_MENU_STRING;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

        switch (choice) {
        case '1': {
            studentMenu();
            break;
        }
        case '2': {
            facultyMenu();
            break;
        }
        case '3': {
            courseMenu();
            break;
        }
        default: {
            return 0;
        }
        }
    }

    return 0;
}