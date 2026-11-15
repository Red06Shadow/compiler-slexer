#ifndef __COMPILER__LEXER__EXPRESION
#define __COMPILER__LEXER__EXPRESION

#include <stdint.h>
#include <string>
#include <vector>
#include <filesystem>

namespace expresions
{
    struct rule
    {
        std::basic_string<char> rege;
        std::basic_string<char> id;
        std::basic_string<char> function;
        friend std::ostream& operator<<(std::ostream& os, const rule& r)
        {
            os << "id: " << r.id << ", rege: " << r.rege << ", function: " << r.function;
            return os;
        }
    };
    struct options
    {
        uint64_t id;
        std::vector<rule> rules;
        friend std::ostream& operator<<(std::ostream& os, const options& o)
        {
            os << "id: " << o.id << ", rules: [";
            for (const auto& r : o.rules)
                os << r << ", ";
            os << "]";
            return os;
        }
    };
    struct file
    {
        std::filesystem::path path;
        std::basic_string<char> content;
        friend std::ostream& operator<<(std::ostream& os, const file& f)
        {
            os << "path: " << f.path << ", content: " << f.content;
            return os;
        }
    };
    struct metadata
    {
        std::basic_string<char> idtype;
        std::basic_string<char> chartype;
        friend std::ostream& operator<<(std::ostream& os, const metadata& m)
        {
            os << "idtype: " << m.idtype << ", chartype: " << m.chartype;
            return os;
        }
    };
    
} // namespace expresions


using listfiles = std::vector<expresions::file>;
using listoptions = std::vector<expresions::options>;

#endif