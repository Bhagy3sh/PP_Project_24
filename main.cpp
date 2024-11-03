#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Student;
class Faculty;
class Course;
class FacultyList;
class StudentList;
class CourseList;

// ============================================================
// All class definitions related to persons (student, faculty)
// ============================================================

class Person {
protected:
    string name;
    int age;

    // pure virtual functions:
    virtual void updateInfoExtra() = 0;                            // to read any additional input in child class
    virtual void displayExtra(shared_ptr<CourseList> crsList) = 0; // to display any additional variables from child class

public:
    void updateInfo(); // to read input
    void display(shared_ptr<CourseList> crsList);
    string getName();
    virtual string getID() = 0; // returns id, pure virtual function
};

class Student : public Person {
protected:
    string prn;
    int year;

    void updateInfoExtra();                            // updates additional info (prn and year of study)
    void displayExtra(shared_ptr<CourseList> crsList); // displays additional info (prn, year of study, and enrolled courses)

public:
    string getID();
};

class Faculty : public Person {
protected:
    string empID, department;

    void updateInfoExtra();                            // updates additional info (empID and department)
    void displayExtra(shared_ptr<CourseList> crsList); // displays additional info (empID, department, and courses being taught)

public:
    string getID();
};

// ============================================================
// Class definitions for course and lab course
// ============================================================

class Course {
protected:
    string courseName, courseID;
    int credits;

    vector<weak_ptr<Student>> enrolledStudents; // list of enrolled students
    weak_ptr<Faculty> assignedFaculty;          // faculty for this course

    virtual void updateInfoExtra(); // to read any additional input in child class
    virtual void displayExtra();    // to display any additional variables from child class

public:
    void updateInfo(shared_ptr<FacultyList> facultyList); // to read input
    void display();
    void displayStudents(); // to display enrolled student list
    void addStudent(shared_ptr<StudentList> studentList);
    void removeStudent();
    bool isStudent(string prn); // returns true if student is enrolled in course, otherwise returns false

    string getID();                 // returns course ID
    string getName();               // returns course name
    weak_ptr<Faculty> getFaculty(); // returns assigned faculty
};

class LabCourse : public Course {
protected:
    string assignedLab;

public:
    void updateInfoExtra(); // updates additional info (assigned lab)
    void displayExtra();    // displays additional info (assigned lab)
};

// ============================================================
// Class definitions for student, faculty, course lists
// ============================================================

class StudentList {
private:
    vector<shared_ptr<Student>> students; // list of students

public:
    void add();
    void displayStudents();
    bool remove(string prn);
    weak_ptr<Student> get(string prn); // gets student by prn if in list, otherwise returns nothing
};

class FacultyList {
private:
    vector<shared_ptr<Faculty>> faculties; // list of faculty

public:
    void add();
    void displayFaculty();
    bool isEmpty(); // returns true if empty otherwise false
    bool remove(string empID);
    weak_ptr<Faculty> get(string empID); // gets faculty by empID if in list, otherwise returns nothing
};

class CourseList {
private:
    vector<shared_ptr<Course>> courses; // list of courses

public:
    void add(shared_ptr<FacultyList> facultyList);
    void displayCourses();
    bool remove(string crsID);
    weak_ptr<Course> get(string crsID);   // gets course by course id if in list, otherwise returns nothing
    vector<shared_ptr<Course>> getList(); // returns the full list of courses
};

// ============================================================
// Person class methods
// ============================================================

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

// ============================================================
// Student class methods
// ============================================================

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

// ============================================================
// Faculty class methods
// ============================================================

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

// ============================================================
// StudentList class methods
// ============================================================

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

// ============================================================
// FacultyList class methods
// ============================================================

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

// ============================================================
// CourseList class methods
// ============================================================

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

// ============================================================
// Course methods
// ============================================================

// these two methods do nothing in the base class, so they are empty, not pure virtual functions
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
    if (st.lock()) {                          // check if this student exists in the student list
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

// ============================================================
// LabCourse methods
// ============================================================

// reads assigned lab
void LabCourse::updateInfoExtra() {
    cout << "Enter assigned lab: ";
    getline(cin, this->assignedLab);
}

// displays assigned lab
void LabCourse::displayExtra() {
    cout << "Assigned Lab: " << this->assignedLab << endl;
}

// ============================================================
// main() function with menu driven code
// ============================================================

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