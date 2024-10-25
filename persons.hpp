#include <memory>
#include <string>
#include <vector>

using namespace std;

class CourseList;

class Person {
protected:
    string name;
    int age;

    virtual void displayExtra(weak_ptr<CourseList> cList) = 0;
    virtual void updateInfoExtra() = 0;

public:
    void updateInfo();
    void display(weak_ptr<CourseList> cList);
    string getName();
    virtual string getID() = 0;
};

class Student : public Person {
protected:
    string prn;
    int year;

    void updateInfoExtra();
    void displayExtra(weak_ptr<CourseList> cList);

public:
    string getID();
};

class Faculty : public Person {
protected:
    string empID, department;

    void updateInfoExtra();
    void displayExtra(weak_ptr<CourseList> cList);

public:
    string getID();
};