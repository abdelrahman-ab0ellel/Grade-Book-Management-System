#include "gradeManager.h"
#include <algorithm>
#include <iostream>
using namespace std;

void GradeManager::updateGPAIndex(Student& student, double oldGPA) {
    if (gpaIndex.count(oldGPA)) {
        auto& bucket = gpaIndex[oldGPA];
        bucket.erase(remove(bucket.begin(), bucket.end(), student.id), bucket.end());
        if (bucket.empty())
            gpaIndex.erase(oldGPA);
    }
    double newGPA = student.getGPA();
    gpaIndex[newGPA].push_back(student.id);
}

void GradeManager::addGrade(Student& student, string subjectName, double grade) {
    double oldGPA = student.getGPA();
    Subject newSubject;
    newSubject.name = subjectName;
    newSubject.grade = grade;
    student.subjects.push_back(newSubject);
    updateGPAIndex(student, oldGPA);
}

bool GradeManager::updateGrade(Student& student, string subjectName, double newGrade) {
    double oldGPA = student.getGPA();

    for (auto& subject : student.subjects) {
        if (subject.name == subjectName) {
            subject.grade = newGrade;
            updateGPAIndex(student, oldGPA);
            return true;
        }
    }

    cerr << "Subject \"" << subjectName << "\" not found!\n";
    return false;  
}

bool GradeManager::removeSubject(Student& student, string subjectName) {
    double oldGPA = student.getGPA();

    // 2. find and remove the subject
    auto& subs = student.subjects;
    for (auto it = subs.begin(); it != subs.end(); ++it) {
        if (it->name == subjectName) {
            subs.erase(it);
            updateGPAIndex(student, oldGPA);
            return true;
        }
    }

    cerr << "Subject \"" << subjectName << "\" not found!\n";
    return false;
}

vector<string> GradeManager::getSortedByGPA() const {
    vector<string> sorted;
    for (auto it = gpaIndex.rbegin(); it != gpaIndex.rend(); ++it)
        for (const string& id : it->second)
            sorted.push_back(id);
    return sorted;
}

const map<double, vector<string>>& GradeManager::getGPAIndex() const {
    return gpaIndex;
}
