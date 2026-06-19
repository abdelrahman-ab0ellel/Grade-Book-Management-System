#include "gradeBook.h"
#include <iostream>
using namespace std;

void GradeBook:: addStudent(string id, string name, string department){
    if(id.empty() || name.empty() || department.empty()){
    cerr<<"fields are empty, fill them please";
    return;
    }
    Student newStudent;
    newStudent.id =id;
    newStudent.name=name;
    newStudent.department=department;
    students[id]=newStudent;
}

Student* GradeBook:: searchById(string id){
    if(students.count(id)){
        return &students[id];
    }

    return nullptr;
}

Student& GradeBook::getStudent(string id){
    return students[id];
}

void GradeBook:: deleteStudent(string id){
    if(!students.count(id)){
        cerr<<"This student not found!";
        return;
    }

    students.erase(id);
}

void GradeBook::updateStudentInfo(string id, string newName, string newDepartment){
    if(!students.count(id)){
        cerr<<"This student not found!";
        return;
    }
    students[id].name =newName;
    students[id].department=newDepartment;
}

bool GradeBook::isExist(string id){
    return students.count(id);
}

int GradeBook::size(){
    return students.size();
}

vector<Student> GradeBook::getAllStudents(){
    vector<Student> allStudents;
    for(auto&[id,Student]: students){
        allStudents.push_back(Student);
    }
    return allStudents;
}
