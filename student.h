#pragma once
#include "subject.h"
#include <vector>
using namespace std;

struct Student{
    string id;
    string name;
    string department;
    vector<Subject> subjects;
    double getGPA()const;
    string gpaToLetter() const;
    double getSubjectGrade(const string& subjectName) const;
};