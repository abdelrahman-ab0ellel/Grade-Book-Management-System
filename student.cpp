#include "Student.h"
#include <iostream>
using namespace std;

double Student::getGPA() const {
    if (subjects.empty())
        return 0.0;
    double total = 0.0;
    for (const auto& s : subjects)
        total += s.grade;
    return (4.0/100.0) * (total / subjects.size());
}

string Student::gpaToLetter() const {
    double gpa = getGPA();
    if (gpa >= 3.3) return "A";
    if (gpa >= 2.7) return "B";
    if (gpa >= 2.3) return "C";
    if (gpa >= 2.0) return "D";
    return "F";
}

double Student::getSubjectGrade(const string& subjectName) const {
    for (const auto& subj : subjects) {
        if (subj.name == subjectName)
            return subj.grade;
    }
    cout << "Subject not registered";
    return 0.0;
}