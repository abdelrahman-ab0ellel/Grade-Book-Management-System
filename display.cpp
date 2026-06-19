#include "display.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

void Display::printHeader() {
    cout << left
         << setw(10) << "ID"
         << setw(20) << "Name"
         << setw(20) << "Department"
         << setw(10) << "GPA"
         << setw(10) << "Grade"
         << "\n";
}

void Display::displaySubjects(Student student) {
    if (student.subjects.empty()) {
        cout << "  No subjects added yet.\n";
        return;
    }

    cout << left
         << setw(20) << "Subject"
         << setw(10) << "Grade"
         << setw(10) << "Letter"
         << "\n";

    for (auto& subject : student.subjects) {
        cout << left
             << setw(20) << "  " + subject.name
             << setw(10) << subject.grade
             << setw(10) << subject.gradeToLetter()
             << "\n";
    }
}

void Display::displayStudent(Student student) {
    cout << left
         << setw(10) << "ID:"         << student.id<< "\n"
         << setw(10) << "Name:"       << student.name << "\n"
         << setw(10) << "Dept:"       << student.department << "\n"
         << setw(10) << "GPA:"        << student.getGPA() << "\n"
         << setw(10) << "Grade:"      << student.gpaToLetter() << "\n";
    displaySubjects(student);
}

void Display::displayAllByGPA(vector<Student> students) {
    if (students.empty()) {
        cout << "No students in the system.\n";
        return;
    }

    sort(students.begin(), students.end(), [](Student a, Student b) {
        return a.getGPA() > b.getGPA();
    });

    printHeader();

    for (auto& student : students) {
        cout << left
             << setw(10) << student.id
             << setw(20) << student.name
             << setw(20) << student.department
             << setw(10) << student.getGPA()
             << setw(10) << student.gpaToLetter()
             << "\n";
    }
}

void Display::displayAllByName(vector<Student> students) {
    if (students.empty()) {
        cout << "No students in the system.\n";
        return;
    }

    sort(students.begin(), students.end(), [](Student a, Student b) {
        return a.name < b.name;
    });

    printHeader();

    for (auto& student : students) {
        cout << left
             << setw(10) << student.id
             << setw(20) << student.name
             << setw(20) << student.department
             << setw(10) << student.getGPA()
             << setw(10) << student.gpaToLetter()
             << "\n";
    }
}

void Display::displayAllByID(vector<Student> students) {
    if (students.empty()) {
        cout << "No students in the system.\n";
        return;
    }

    sort(students.begin(), students.end(), [](Student a, Student b) {
        return a.id < b.id;
    });

    printHeader();

    for (auto& student : students) {
        cout << left
             << setw(10) << student.id
             << setw(20) << student.name
             << setw(20) << student.department
             << setw(10) << student.getGPA()
             << setw(10) << student.gpaToLetter()
             << "\n";
    }
}