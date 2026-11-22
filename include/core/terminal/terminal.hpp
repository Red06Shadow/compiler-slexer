#ifndef __TERMINAL_SGPROJECT
#define __TERMINAL_SGPROJECT

#include <iostream>
#include <stdint.h>
#include <processenv.h>
#include <filesystem>

class format
{
public:
    enum class style : uint8_t {
        NORMAL,
        NEGRITA,
        TENUE,
        CURSIVA,
        SUBRAYADO,
        PARPADEO,
        INVERTIDO,
        OCULTO
    };
    enum class color : uint8_t {
        RESET = 0,
        BLACK = 30,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        BRIGHT_BLACK = 90,
        BRIGHT_RED,
        BRIGHT_GREEN,
        BRIGHT_YELLOW,
        BRIGHT_BLUE,
        BRIGHT_MAGENTA,
        BRIGHT_CYAN,
        BRIGHT_WHITE
    };
    enum class background : uint8_t {
        RESET = 0,
        BLACK = 40,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        BRIGHT_BLACK = 100,
        BRIGHT_RED,
        BRIGHT_GREEN,
        BRIGHT_YELLOW,
        BRIGHT_BLUE,
        BRIGHT_MAGENTA,
        BRIGHT_CYAN,
        BRIGHT_WHITE,
    };
private:
    uint32_t _M_code_color_format;
    uint32_t _M_code_sytle;
    uint32_t _M_code_color_background;
public:
    format() : 
        _M_code_color_format(static_cast<uint32_t>(format::color::RESET)), 
        _M_code_sytle(static_cast<uint32_t>(format::style::NORMAL)),
        _M_code_color_background(static_cast<uint32_t>(format::background::RESET))
    {}
    format(format::color _c) : 
        _M_code_color_format(static_cast<uint32_t>(_c)), 
        _M_code_sytle(static_cast<uint32_t>(format::style::NORMAL)),
        _M_code_color_background(static_cast<uint32_t>(format::background::RESET))
    {}
    format(format::color _c, format::background _b) : 
        _M_code_color_format(static_cast<uint32_t>(_c)), 
        _M_code_sytle(static_cast<uint32_t>(format::style::NORMAL)),
        _M_code_color_background(static_cast<uint32_t>(_b))
    {}
    format(format::style _s, format::color _c) : 
        _M_code_color_format(static_cast<uint32_t>(_c)), 
        _M_code_sytle(static_cast<uint32_t>(_s)),
        _M_code_color_background(static_cast<uint32_t>(format::background::RESET))
    {}
    format(format::style _s, format::color _c, format::background _b) : 
        _M_code_color_format(static_cast<uint32_t>(_c)), 
        _M_code_sytle(static_cast<uint32_t>(_s)),
        _M_code_color_background(static_cast<uint32_t>(_b))
    {}
    friend std::basic_ostream<char>& operator<<(std::basic_ostream<char>& out, const format& _t) {
        out << "\033[" << _t._M_code_sytle << ";" << _t._M_code_color_format << "m";
        return out;
    }
    ~format() {}
};

class terminal
{
public:
    static std::string anidation() { return ""; }
    template <typename T, typename... Args>
    static std::string anidation(const T &arg, const Args &...args) { return ""; }
    template <typename... Args>
    static std::string anidation(const char *arg, const Args &...args)
    {
        std::string str = anidation(args...);
#if defined(__linux__) || defined(__APPLE__)
        return std::string(arg) + (str.empty() ? str : (" ; " + str));
#elif defined(_WIN32) || defined(_WIN64)
        return std::string(arg) + (str.empty() ? str : (" && " + str));
#endif
    }
    template <typename... Args>
    static std::string anidation(const std::string &arg, const Args &...args)
    {
        std::string str = anidation(args...);
#if defined(__linux__) || defined(__APPLE__)
        return std::string(arg) + (str.empty() ? str : (" ; " + str));
#elif defined(_WIN32) || defined(_WIN64)
        return std::string(arg) + (str.empty() ? str : (" && " + str));
#endif
    }
    static std::filesystem::path current_path() {
        wchar_t cwd[260];
        GetCurrentDirectoryW(260, cwd) != 0;
        return std::filesystem::path(cwd);
    };
};

#endif