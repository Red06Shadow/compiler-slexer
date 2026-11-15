#ifndef __COMPILER__LEXER__COMPILER
#define __COMPILER__LEXER__COMPILER

#include <filesystem>
#include <iostream>

class compiler
{

public:
    static bool compile(const std::filesystem::path&);
};

bool compiler::compile(const std::filesystem::path& sources) {
    std::string gcmd = "cd \"" + sources.string() + "\" && g++ -fdiagnostics-color=always -c -Iinclude/slexer -Iinclude \"yourslexer.cpp\" -o yourslexer.o";
    std::cout << "\033[1;37m" << gcmd << "\033[0" << std::endl;
    if (std::system(gcmd.c_str()))
        throw std::runtime_error("Error fatal: fallos de compilacion del lexer.");
    //////////////////////////////////////////////////////////////////////////////////////
    std::string acmd = "cd \"" + sources.string() + "\" && ar rcs libyourslexer.a -o yourslexer.o";
    std::cout << "\033[1;37m" << acmd << "\033[0" << std::endl;
    if (std::system(acmd.c_str()))
        throw std::runtime_error("Error fatal: fallos generacion de libreria.");
    return 0;
}


#endif