#include <memory>
#include <string>
#include <vector>

using namespace std;

// predefine classes so we can use in definitions
class CourseList;

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