#ifndef __GLOBAL
#define __GLOBAL

#define MODE_DEBUG true

#include <filesystem>

class global
{
public:
    class args
    {
    private:
        const char **_M_begin;
        const char **_M_iterator;
        const char **_M_end;

    public:
        args() : _M_begin(nullptr), _M_iterator(nullptr), _M_end(nullptr) {}
        args(const char **argv, uint64_t args) : _M_begin(argv), _M_iterator(argv), _M_end(argv + args) {}
        const char **iterator() const { return _M_iterator; }
        std::basic_string<char> peak() const { return std::basic_string<char>(*_M_iterator); }
        std::basic_string<char> next()
        {
            const char **pv = _M_iterator++;
            return std::basic_string<char>(*pv);
        }
        const char **begin() const { return _M_begin; }
        const char **end() const { return _M_end; }
        ~args() {}
    };
    inline static std::filesystem::path installing{};
    inline static std::filesystem::path repository{};
    inline static std::filesystem::path source{};
    inline static std::filesystem::path destine{};
};

#endif