#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// predefine classes so we can use in definitions
class Student;
class Faculty;
class Course;

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