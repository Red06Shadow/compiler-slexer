#ifndef GLOBAL
#define GLOBAL

#include <string>
#include <iostream>

struct expresion
{
    size_t idreal;
    std::string regex;
    std::string id;
    std::string funtion;
    friend std::ostream& operator<<(std::ostream& out, expresion exp) {
        out << "group: " << exp.idreal << std::endl;
        out << "typeid: " << exp.id << std::endl;
        out << "regularexpresion: " << exp.regex << std::endl;
        out << "funtion: " << exp.funtion << std::endl;
        return out;
    }
};
#endif