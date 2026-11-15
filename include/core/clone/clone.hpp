#ifndef __COMPILER__LEXER__CLONE
#define __COMPILER__LEXER__CLONE

#include <iostream>
#include <core/expresions/expresions.hpp>
#include <fstream>
#include <git2.h>

class clone
{
private:
    static void _S_generate_project(const std::filesystem::path& destine, const listfiles& files);
    static void _S_copy_repo(const std::filesystem::path& destine, const std::filesystem::path& source);
    static void _S_clone_repo(const std::filesystem::path& path);
    static void _S_pull_repo(const std::filesystem::path& path);
public:
    static void clone_(
        const std::filesystem::path& repository,
        const std::filesystem::path& destine
    );
};

#endif