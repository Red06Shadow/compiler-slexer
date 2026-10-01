#ifndef COMPILER
#define COMPILER

#include <filesystem>
#include <iostream>

class compiler
{

public:
    static bool compile(const std::filesystem::path&, const std::filesystem::path&, const std::filesystem::path&);
};

bool compiler::compile(const std::filesystem::path& parent, const std::filesystem::path& hpp, const std::filesystem::path& cpp) {
    std::string compiler_comannd = "cd \"" + parent.string() + "\" && g++ -fdiagnostics-color=always -c ylexer.cpp -o ylexer.o";
    std::cout << "\033[1;37m" << compiler_comannd << "\033[0" << std::endl;
    if (std::system(compiler_comannd.c_str())) return false;
    std::string arc_comannd = "cd \"" + parent.string() + "\" && ar rcs liblexer.a -o ylexer.o";
    std::cout << "\033[1;37m" << arc_comannd << "\033[0" << std::endl;
    if (std::system(arc_comannd.c_str())) return false;
    return true;
}


#endif