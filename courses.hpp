#include <memory>
#include <string>
#include <vector>

using namespace std;

// predefine classes so we can use in definitions
class Student;
class Faculty;
class FacultyList;
class StudentList;

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