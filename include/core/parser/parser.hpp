#ifndef READERHPP
#define READERHPP

#include <myregex/myregex.hpp>
#include <fstream>
#include <core/global/global.hpp>

class parser
{
public:
    inline static std::string chartype{};
    inline static std::string idtype{};
    inline static std::basic_ofstream<char> outhpp{};
    inline static std::basic_ofstream<char> outcpp{};
    inline static std::vector<expresion> expresions{};

private:
    typedef void (*Handle)(std::basic_ifstream<char> &);

    inline static myregex::regex<size_t> head = myregex::builder<size_t>({
                                                                             {1, "#.hpp#"},
                                                                             {2, "#.cpp#"},
                                                                             {3, "#.expresion#"},
                                                                             {4, "#.metadata#"},
                                                                             {5, "#.typeid#"},
                                                                             {6, "#.typechar#"},
                                                                             {7, "#.main#"},
                                                                             {8, "#.option#"},
                                                                             {9, "#.exp#"},
                                                                             {10, "#.end#"},
                                                                         })
                                                    .convert_to_dfa()
                                                    .convert_to_table()
                                                    .build();

public:
    static void load(std::basic_ifstream<char> &);

private:
    static void hpphead(std::basic_ifstream<char> &in);
    static void cpphead(std::basic_ifstream<char> &in);
    static void metadatahead(std::basic_ifstream<char> &in);
    static void expresionmainhead(std::basic_ifstream<char> &in);
    static void regularexpresion(std::basic_ifstream<char> &in, size_t idaction);

    inline static Handle handle[10] = {
        nullptr,
        hpphead,
        cpphead,
        expresionmainhead,
        metadatahead,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };
};

void parser::hpphead(std::basic_ifstream<char> &in)
{
    while (!in.eof())
    {
        std::string line{};
        std::getline(in, line);
        basic_string_range<char> range{line};
        size_t id = head.match<myregex::constants::match_maximun>(range).id();
        if (id == 10)
        {
            if (range.peak() != range.end())
                throw std::runtime_error("La linea solo debe pertenecer a la cabecera");
            break;
        }
        outhpp << line << std::endl;
    }
}
void parser::cpphead(std::basic_ifstream<char> &in)
{
    while (!in.eof())
    {
        std::string line{};
        std::getline(in, line);
        basic_string_range<char> range{line};
        size_t id = head.match<myregex::constants::match_maximun>(range).id();
        if (id == 10)
        {
            if (range.peak() != range.end())
                throw std::runtime_error("La linea solo debe pertenecer a la cabecera");
            break;
        }
        outcpp << line << std::endl;
    }
}
void parser::metadatahead(std::basic_ifstream<char> &in)
{
    std::string line{};
    size_t id = 0;
    //////////////////////////////////////////////////////////////////////////////////
    if (in.eof())
        throw std::runtime_error("Se esperaba la cabecera de typeid(#.typeid#)");
    std::getline(in, line);
    basic_string_range<char> range{line};
    id = head.match<myregex::constants::match_maximun>(range).id();
    if (id != 5)
        throw std::runtime_error("Se esperaba la cabecera de typeid(#.typeid#)");
    //////////////////////////////////////////////////////////////////////////////////
    if (in.eof())
        throw std::runtime_error("Se esperaba un texto plano con el tipo de id");
    std::getline(in, line);
    range = basic_string_range<char>(line);
    id = head.match<myregex::constants::match_maximun>(range).id();
    if (id != 0)
        throw std::runtime_error("Se esperaba un texto plano con el tipo de id");
    idtype = line;
    line.clear();
    //////////////////////////////////////////////////////////////////////////////////
    if (in.eof())
        throw std::runtime_error("Se esperaba la cabecera de typechar(#.typechar#)");
    std::getline(in, line);
    range = basic_string_range<char>(line);
    id = head.match<myregex::constants::match_maximun>(range).id();
    if (id != 6)
        throw std::runtime_error("Se esperaba la cabecera de typechar(#.typechar#)");
    //////////////////////////////////////////////////////////////////////////////////
    if (in.eof())
        throw std::runtime_error("Se esperaba un texto plano con el tipo de caracter");
    std::getline(in, line);
    range = basic_string_range<char>(line);
    id = head.match<myregex::constants::match_maximun>(range).id();
    if (id != 0)
        throw std::runtime_error("Se esperaba un texto plano con el tipo de caracter");
    chartype = line;
    line.clear();
}
void parser::expresionmainhead(std::basic_ifstream<char> &in)
{
    std::string line{};
    size_t id = 0;
    size_t count = 0;
    //////////////////////////////////////////////////////////////////////////////////
    if (in.eof())
        throw std::runtime_error("Se esperaba la cabecera de main(#.main#)");
    std::getline(in, line);
    basic_string_range<char> range{line};
    id = head.match<myregex::constants::match_maximun>(range).id();
    if (id != 7)
        throw std::runtime_error("Se esperaba la cabecera de main(#.main#)");
    regularexpresion(in, count++);
    line.clear();
    do
    {
        if (in.eof())
            throw std::runtime_error("Se esperaba la cabecera de cierre u otra de definicion de reglas(#.end# o #.option#)");
        std::getline(in, line);
        basic_string_range<char> range{line};
        id = head.match<myregex::constants::match_maximun>(range).id();
        if (id == 10)
            break;
        if (id != 8)
            throw std::runtime_error("Se esperaba la cabecera de option(#.option#)");
        regularexpresion(in, count++);
        line.clear();
    } while (true);
}

void parser::regularexpresion(std::basic_ifstream<char> &in, size_t idaction)
{
    std::string line{};
    size_t id = 0;
    //////////////////////////////////////////////////////////////////////////////////
    do
    {
        if (in.eof())
            throw std::runtime_error("Se esperaba la cabecera de cierre u otra de definicion de reglas(#.end# o #.option#)");
        std::getline(in, line);
        basic_string_range<char> range{line};
        id = head.match<myregex::constants::match_maximun>(range).id();
        if (id == 10)
            break;
        if (id != 9)
            throw std::runtime_error("Se esperaba la cabecera de exp(#.exp#)");
        line.clear();
        expresion _exp {};
        if (in.eof())
            throw std::runtime_error("Se esperaba el texto plano del id");
        std::getline(in, _exp.id);
        if (in.eof())
            throw std::runtime_error("Se esperaba el texto plano de la expresion regular");
        std::getline(in, _exp.regex);
        if (in.eof())
            throw std::runtime_error("Se esperaba el texto plano de la funcion");
        std::getline(in, _exp.funtion);
        _exp.idreal = idaction;
        expresions.push_back(_exp);
    } while (true);
}

void parser::load(std::basic_ifstream<char> &in)
{
    while (!in.eof())
    {
        std::string line{};
        std::getline(in, line);

        if (line.empty())
            continue;

        basic_string_range<char> range{line};

        size_t id = head.match<myregex::constants::match_maximun>(range).id();
        if (id == 0)
            throw std::runtime_error("Todo archivo debe iniciar con una cabecera(o #.hpp# o #.exp# o #.cpp#)");
        if (range.peak() != range.end())
            throw std::runtime_error("La linea solo debe pertenecer a la cabecera");
        handle[id](in);
    }
}
#endif