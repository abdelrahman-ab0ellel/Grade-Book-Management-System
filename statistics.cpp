#include "statistics.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

double Statistics::getClassAverage(vector<Student> students){
    if(students.empty()){
        return 0.0;
    }

    double total=0;
    for(auto& student : students){
        total+=student.getGPA();
    }

    return total/students.size();
}

Student Statistics::getHigestStudent(vector<Student> students){
    if(students.empty()){
        return Student();
    }
    Student H_Student = students[0];
    for(auto& student: students){
        if(student.getGPA() > H_Student.getGPA()){
            H_Student = student;
        }
    }

    return H_Student;
}

Student Statistics::getLowestStudent(vector<Student> students){
    if(students.empty()){
        return Student();
    }
    Student L_Student = students[0];
    for(auto& student: students){
        if(student.getGPA() < L_Student.getGPA()){
            L_Student = student;
        }
    }

    return L_Student;
}

map<string, int> Statistics::getGradeBandCount(vector<Student> students){
    map<string, int> bands;
    bands["A"]=0;
    bands["B"]=0;
    bands["C"]=0;
    bands["D"]=0;
    bands["F"]=0;

    for(auto& student: students){
        string gpa_letter= student.gpaToLetter(); 
        bands[gpa_letter]++;
    }

    return bands;
}

void Statistics::showStats(vector<Student> students){
    if(students.empty()){
        cout<<"There are no students in the system";
        return;
    }


    double avg = getClassAverage(students);
    Student H_student = getHigestStudent(students);
    Student L_student = getLowestStudent(students);
    map<string, int> bands = getGradeBandCount(students);

     cout<<"Total students: "<<students.size()<<endl;
     cout<<"Average is: "<<avg<<endl;
     cout<<"Highest student is: "<<H_student.name<<endl; 
     cout<<"Lowest student is: "<<L_student.name<<endl;  

     for(auto& [letter,count]: bands){
         cout<<letter<<" : "<<count<<endl;
    }

}

vector<Student> Statistics::searchByName(vector<Student> students, string name){
    vector<Student> result;

    if(students.empty()){
        return result;
    }

    for(auto& student : students){
        if(student.name.find(name) != string::npos){
            result.push_back(student);
        }
    }

    if(result.empty()){
        cerr<<"There is no student with this name\n";
        return result;
    }

    return result;
}

vector<Student> Statistics::searchByDepartment(vector<Student> students, string department){
    vector<Student> result;

    if(students.empty()){
        return result;
    }

    for(auto& student : students){
        if(student.department.find(department) != string::npos){
            result.push_back(student);
        }
    }

    if(result.empty()){
        cerr<<"There is no student with this department\n";
        return result;
    }

    return result;
}

bool Statistics::loadFromFile(const string& filename, GradeBook& gradeBook, GradeManager& gradeManager) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << "\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 3) continue;

        string id   = tokens[0];
        string name = tokens[1];
        string dept = tokens[2];

        if (gradeBook.isExist(id)) continue;

        gradeBook.addStudent(id, name, dept);
        Student& student = gradeBook.getStudent(id);

        for (int i = 3; i + 1 < tokens.size(); i += 2) {
            string subjectName = tokens[i];
            double grade = stod(tokens[i + 1]);
            gradeManager.addGrade(student, subjectName, grade);
        }
    }

    file.close();
    return true;
}

void Statistics::saveToFile(const string& filename, GradeBook& gradeBook) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file for saving: " << filename << "\n";
        return;
    }

    vector<Student> allStudents = gradeBook.getAllStudents();
    sort(allStudents.begin(), allStudents.end(), [](Student& a, Student& b) {
        return a.id < b.id;
    });
    for (auto& student : allStudents) {
        file << student.id << ","
             << student.name << ","
             << student.department;

        for (auto& subject : student.subjects) {
            file << "," << subject.name << "," << subject.grade;
        }

        file << "\n";
    }

    file.close();
}