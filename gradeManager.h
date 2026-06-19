#pragma once
#include "student.h"
#include <map>
#include <vector>
using namespace std;

class GradeManager {
private:
    map<double, vector<string>> gpaIndex;

public:
    void addGrade(Student& student, string subjectName, double grade);
    bool updateGrade(Student& student, string subjectName, double newGrade);
    bool removeSubject(Student& student, string subjectName);
    vector<string> getSortedByGPA() const;
    const map<double, vector<string>>& getGPAIndex() const;

private:
    void updateGPAIndex(Student& student, double oldGPA);
};
