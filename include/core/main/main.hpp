#ifndef MAINSLEXERCOMPILER
#define MAINSLEXERCOMPILER

#include <string>
#include <stdint.h>
#include <iostream>
#include <map>
#include <global/global.hpp>

class gesture
{
public:
    typedef uint32_t (*handle)(global::args& _args);
private:
    static std::map<std::basic_string<char>, handle> _S_operations; 
    static uint32_t help(global::args& _args);
    static uint32_t version(global::args& _args);
    static uint32_t default_(global::args& _args);
public:
    inline static void version() { std::cout << "compiler-slexer v0.1.0 @ Red06Shadow." << std::endl; }
    static void excecute(int args, const char **argv);
};

#endif