#pragma once
#include "student.h"
#include <vector>
using namespace std;

class Display {
    public:
    void displayStudent(Student student);
    void displayAllByGPA(vector<Student> students);
    void displayAllByName(vector<Student> students);
    void displayAllByID(vector<Student> students);
    void displaySubjects(Student student);
    void printHeader();
};