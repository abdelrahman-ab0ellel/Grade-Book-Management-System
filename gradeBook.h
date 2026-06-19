#pragma once
#include "student.h"
#include <unordered_map>
using namespace std;

class GradeBook{
private:
    unordered_map<string,Student> students;

public:
    void addStudent(string id, string name, string department);
    Student* searchById(string id);
    Student& getStudent(string id);
    void deleteStudent(string id);
    void updateStudentInfo(string id, string newName , string newDepartment);
    bool isExist(string id);
    vector<Student> getAllStudents();
    int size();
    
};

