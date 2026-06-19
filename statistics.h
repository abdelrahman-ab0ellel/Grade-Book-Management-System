#pragma once
#include "student.h"
#include "gradeBook.h"
#include "gradeManager.h"
#include <vector>
#include <map>
using namespace std;

class Statistics{
public:
    double getClassAverage(vector<Student> students);
    Student getHigestStudent(vector<Student> students);
    Student getLowestStudent(vector<Student> students);
    map<string, int> getGradeBandCount(vector<Student> students);
    void showStats(vector<Student> students);
    vector<Student> searchByName(vector<Student> students, string name);
    vector<Student> searchByDepartment(vector<Student> students, string department);
    bool loadFromFile(const string& filename, GradeBook& gradeBook, GradeManager& gradeManager);
    void saveToFile(const string& filename, GradeBook& gradeBook);
};
