#ifndef __COMPILER__LEXER__CLEAN
#define __COMPILER__LEXER__CLEAN

#include <filesystem>
#include <fstream>
#include <core/expresions/expresions.hpp>
#include <map>

class clean
{
private:
    inline static std::vector<std::filesystem::path> _S_lecture {};
    static void _S_load_list_by_order(const std::filesystem::path& repository);
    static void _S_copy_files(std::basic_ifstream<char> &in, std::basic_ofstream<char> &out);
    static void _S_copy(const std::filesystem::path& source, std::basic_ofstream<char>& destine);
public:
    static void clear(const std::filesystem::path& source, const std::filesystem::path& repository, const expresions::file& hpp, const expresions::metadata& meta);
};

#endif