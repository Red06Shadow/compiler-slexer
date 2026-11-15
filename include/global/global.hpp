#ifndef __GLOBAL
#define __GLOBAL

#define MODE_DEBUG true

#include <filesystem>

class global
{
public:
    inline static std::filesystem::path installing {};
    inline static std::filesystem::path repository {};
    inline static std::filesystem::path source {};
    inline static std::filesystem::path destine {};
};

#endif