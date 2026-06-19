#pragma once
#include <string>
using namespace std;

struct Subject{
    string name;
    double grade;
    string gradeToLetter() const;
} ;
