#include "clean.hpp"
#include <iostream>

void clean::_S_load_list_by_order(const std::filesystem::path& repository) {
    std::basic_ifstream<char> in {repository / "list.txt"};
    std::basic_string<char> line {};
    while (!in.eof())
    {
        std::getline(in, line);
        clean::_S_lecture.push_back(repository / "include" / "slexer" / line);
        line.clear();
    }
}
void clean::_S_copy_files(std::basic_ifstream<char> &in, std::basic_ofstream<char> &out)
{
    std::basic_string<char> line{};
    bool write = false;
    while (!in.eof())
    {
        std::getline(in, line);
        if (line == "//<--...")
            write = !write;
        else if (write)
            out << line << std::endl;
        line.clear();
    }
}
void clean::_S_copy(const std::filesystem::path &source, std::basic_ofstream<char> &destine)
{
    for (auto &&dir : std::filesystem::directory_iterator(source))
    {
        if (dir.is_directory() && dir.path().filename() != "..")
            clean::_S_copy(dir.path(), destine);
        else if (dir.is_regular_file())
        {
            if (dir.path().extension() == ".hpp")
            {
                std::basic_ifstream<char> in{dir.path()};
                clean::_S_copy_files(in, destine);
                in.close();
            }
        }
    }
}

void clean::clear(const std::filesystem::path &source, const std::filesystem::path& repository, const expresions::file &hpp, const expresions::metadata &meta)
{
    std::basic_ofstream<char> out{source / "yourslexer.hpp"};
    out << "#ifndef YOURSLEXER" << std::endl
        << "#define YOURSLEXER" << std::endl;
    out << hpp.content << std::endl;
    clean::_S_load_list_by_order(repository);
    for (auto &&path : clean::_S_lecture)
        _S_copy(path, out);
    std::filesystem::remove_all(source / "include");
    std::filesystem::remove(source / ".gitignore");
    std::filesystem::remove(source / "README.md");
    std::filesystem::remove(source / "list.txt");
    std::filesystem::remove(source / "yourslexer.cpp");
    std::filesystem::remove(source / "yourslexer.o");
    out << "extern slexer::basic_lexer<" << (meta.chartype == "ASCII" ? "char" : "wchar_t") << ", " << meta.idtype << "> ylexer;" << std::endl;
    out << "#endif" << std::endl;
    out.close();
}