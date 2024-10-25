#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Student;
class Faculty;
class Course;

class StudentList {
private:
    vector<shared_ptr<Student>> students;

public:
    void add();
    void displayStudents();
    bool remove(string prn);
    weak_ptr<Student> get(string prn);
};

class FacultyList {
private:
    vector<shared_ptr<Faculty>> faculties;

public:
    void add();
    void displayFaculty();
    bool isEmpty();
    bool remove(string empID);
    weak_ptr<Faculty> get(string empID);
};

class CourseList {
private:
    vector<shared_ptr<Course>> courses;

public:
    void add(weak_ptr<FacultyList> fList);
    void displayCourses();
    bool remove(string crsID);
    weak_ptr<Course> get(string crsID);
    vector<shared_ptr<Course>> getList();
};