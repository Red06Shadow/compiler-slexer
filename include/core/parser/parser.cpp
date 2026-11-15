#include "parser.hpp"
#include <iostream>


expresions::file parser::_S_parser_include(const YAML::Node& incld) {
    // if (!incld["content"] && !incld["reference"])
    //     throw YAML::RepresentationException(root.Mark(), "Se necesita que se defina un conjunto de reglas a procesar");
    return {.path = incld["path"].as<std::basic_string<char>>(), .content = incld["content"].as<std::basic_string<char>>("")};
}

expresions::rule parser::_S_parser_rule(const YAML::Node& root) {
    if(!root.IsMap() || root.size() != 3)
        throw YAML::RepresentationException(root.Mark(), "Se necesita que se defina un conjunto de reglas a procesar");
    return {
        .rege = root["expresion"].as<std::basic_string<char>>(),
        .id = root["id"].as<std::basic_string<char>>(),
        .function = root["function"].as<std::basic_string<char>>()
    };
}

expresions::options parser::_S_parser_regex(const YAML::Node& root, bool isStart) {
    std::vector<expresions::rule> _M_rule {};
    if (!root.IsSequence())
        throw YAML::RepresentationException(root.Mark(), "Se necesita que se defina un conjunto de reglas a procesar");
    for (const auto& reg : root)
        _M_rule.push_back(std::move(parser::_S_parser_rule(reg)));
    return {.id = isStart ? 0U : _S_id++, .rules = std::move(_M_rule)};
}
void parser::_S_parser_slexer(const YAML::Node& root) {
    _S_metadata.chartype = root["typechar"].as<std::basic_string<char>>();
    if (_S_metadata.chartype != "ASCII" && _S_metadata.chartype != "UNICODE")
        throw YAML::RepresentationException(root.Mark(), "Se necesita que se defina un conjunto de reglas a procesar");
    _S_metadata.idtype = root["typeid"].as<std::basic_string<char>>();
    parser::_S_main_hpp = 
        { .path = "yourslexer.hpp",
          .content = root["hpp_content"].as<std::basic_string<char>>()
        };
    parser::_S_main_cpp = 
        { .path = "yourslexer.cpp",
          .content = root["cpp_content"].as<std::basic_string<char>>()
        };
    if (!root["regex"].IsSequence())
        return;

    for (const auto& group : root["regex"])
    {
        if(!group.IsMap() || group.size() != 1)
            throw YAML::RepresentationException(root.Mark(), "Se necesita que se defina un conjunto de reglas a procesar");
        bool isStart;
        if ((isStart = group.begin()->first.as<std::basic_string<char>>() == "start"))
            _S_define_start = true;
        if (isStart)
            _S_regexs.insert(_S_regexs.begin(), std::move(parser::_S_parser_regex(group.begin()->second, isStart)));
        else
            _S_regexs.push_back(std::move(parser::_S_parser_regex(group.begin()->second, isStart)));
    }
}

void parser::_S_parser_root(const YAML::Node& root) {
    _S_includes.reserve(32);
    if (root["includes"])
        for (const auto& includ : root["includes"])
            _S_includes.push_back(std::move(parser::_S_parser_include(includ)));
    parser::_S_parser_slexer(root["slexer"]);
}

void parser::parser_with_yaml(const std::filesystem::path& path) {
    try
    {
        YAML::Node root = YAML::LoadFile(path.string());
        parser::_S_parser_root(root);
        if (!_S_define_start)
            throw std::runtime_error("Se necesita que se defina un conjunto de reglas a procesar con el nombre de start");
    }
    catch(const YAML::Exception& e) {
        std::cerr << "Error: en yaml: " << e.what() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << e.what() << std::endl;
    }
}