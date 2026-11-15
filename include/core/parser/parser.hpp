#ifndef __COMPILER__LEXER__PARSER
#define __COMPILER__LEXER__PARSER

#include <core/expresions/expresions.hpp>
#include <yaml-cpp/yaml.h>


class parser
{
private:
    inline static bool _S_define_start = false;
    inline static uint64_t _S_id = 1U;
    inline static listfiles _S_includes {};
    inline static listoptions _S_regexs {};
    inline static expresions::metadata _S_metadata {};

    inline static expresions::file _S_main_hpp {};
    inline static expresions::file _S_main_cpp {};

    static expresions::rule _S_parser_rule(const YAML::Node& root);
    static expresions::options _S_parser_regex(const YAML::Node& root, bool isStart);
    static void _S_parser_slexer(const YAML::Node& root);
    static void _S_parser_root(const YAML::Node& root);
    static expresions::file _S_parser_include(const YAML::Node& incld);
public:
    static void parser_with_yaml(const std::filesystem::path& in);
    static expresions::metadata& metadata() { return _S_metadata; }
    static listfiles& includes() { return _S_includes; }
    static expresions::file& hpp() { return _S_main_hpp; }
    static expresions::file& cpp() { return _S_main_cpp; }
    static listoptions& options() { return _S_regexs; }
};

#endif