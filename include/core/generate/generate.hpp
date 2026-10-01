#ifndef GENERATE
#define GENERATE

#include <myregex/myregex.hpp>
#include <core/global/global.hpp>

class generate
{
public:
    inline static constexpr bool is_char = true;
    inline static constexpr bool is_wchar_t = false;
    static bool is_char_or_wchar_t;
    //////////////////////////////////////////////////////////////////////////
    static void build_char_expresions(const std::vector<expresion> &, std::vector<myregex::table<size_t>> &);
};

void generate::build_char_expresions(const std::vector<expresion> &expresions, std::vector<myregex::table<size_t>> &allocation)
{
    std::vector<std::pair<size_t, std::string>> regexs{};
    size_t index = 0, count = 0;
    for (auto &&exp : expresions)
    {
        if (index != exp.idreal)
        {
            auto v1 = myregex::basic_builder<char, size_t>::build_nfa(regexs);
            auto v2 = myregex::basic_builder<char, size_t>::build_dfa(v1);
            allocation.push_back(std::move(myregex::basic_builder<char, size_t>::build_table(v2)));
            regexs.clear();
            index = exp.idreal;
        }
        regexs.push_back({count++, exp.regex});
    }
    if (!regexs.empty())
    {
        auto v1 = myregex::basic_builder<char, size_t>::build_nfa(regexs);
        auto v2 = myregex::basic_builder<char, size_t>::build_dfa(v1);
        allocation.push_back(std::move(myregex::basic_builder<char, size_t>::build_table(v2)));
    }
}

#endif