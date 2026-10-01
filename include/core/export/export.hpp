#ifndef EXPORT
#define EXPORT
#include <myregex/myregex.hpp>
#include <fstream>
#include <core/global/global.hpp>

class export_
{
public:
    static void export_tables_char(const std::vector<myregex::table<size_t>> &, const std::vector<expresion> &, const std::string &, std::basic_ofstream<char> &);
    static void export_table_char(const myregex::table<size_t> &, const std::vector<expresion> &, const std::string &, std::basic_ofstream<char> &);
};
void export_::export_tables_char(const std::vector<myregex::table<size_t>> &tablas, const std::vector<expresion> &exps, const std::string &idT, std::basic_ofstream<char> &out)
{
    out << "slexer::basic_lexer<char," << idT << "> ylexer = slexer::basic_lexer<char," << idT << ">({" << std::endl;
    for (auto &&table : tablas)
    {
        export_table_char(table, exps, idT, out);
        out << ',';
    }
    out << "});" << std::endl;
}

void export_::export_table_char(const myregex::table<size_t> &other, const std::vector<expresion> &exps, const std::string &idT, std::basic_ofstream<char> &out)
{
    out << "myregex::basic_table<char, " << "slexer::basic_lexer<char," << idT << ">::_I_idT" << ">({";
    for (size_t state = 0; state < other.status().size(); state++)
    {
        if (other.status()[state].valid())
            out << "slexer::basic_lexer<char," << idT << ">::_I_idT" <<'(' << exps[other.status()[state].get()].id << ',' << exps[other.status()[state].get()].funtion << ')';
        else
            out << "{}";
        out << ((state >= other.status().size() - 1ULL) ? '}' : ',');
    }
    out << ',' << std::endl
        << '{';
    for (size_t state = 0; state < other.status().size(); state++)
    {
        for (size_t letter = 0; letter < myregex::basic_table<char, size_t>::dictionary; letter++)
                out << (long long)(other.transitions()[(state * myregex::basic_table<char, size_t>::dictionary) + letter]) << "ULL" << ',';
        out << std::endl;
    }
    out << "})";
}
#endif