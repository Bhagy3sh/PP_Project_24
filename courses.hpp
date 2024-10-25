#include <memory>
#include <string>
#include <vector>

using namespace std;

class Student;
class Faculty;
class FacultyList;
class StudentList;

class Course {
protected:
    string courseName, courseID;
    int credits;

    vector<weak_ptr<Student>> enrolledStudents;
    weak_ptr<Faculty> assignedFaculty;

    virtual void updateInfoExtra();
    virtual void displayExtra();

public:
    void updateInfo(weak_ptr<FacultyList> fList);
    void display();
    void displayStudents();
    void addStudent(weak_ptr<StudentList> sList);
    void removeStudent();
    bool isStudent(string prn);

    string getID();
    string getName();
    weak_ptr<Faculty> getFaculty();
};

class LabCourse : public Course {
protected:
    string assignedLab;

public:
    void updateInfoExtra();
    void displayExtra();
};