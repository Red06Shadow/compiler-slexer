#include <iostream>
#include <filesystem>
#include <global/global.hpp>
#include <core/parser/parser.hpp>
#include <core/clone/clone.hpp>
#include <core/build/build.hpp>
#include <core/compiler/compiler.hpp>
#include <core/clean/clean.hpp>
#include <myregex/exceptions/error.hpp>
#include <core/terminal/terminal.hpp>
#include <core/main/main.hpp>
/////////////////////////////////////////////////////////////////////////////////////////

std::map<std::basic_string<char>, gesture::handle> gesture::_S_operations{
    {"-c", gesture::default_},
    {"--help", gesture::help},
    {"--version", gesture::version},
};

uint32_t gesture::help(global::args &_args)
{
    if (_args.iterator() != _args.end())
        throw std::runtime_error("Arguments are not allowed for --help.");
    return 0;
}

uint32_t gesture::version(global::args &_args)
{
    gesture::version();
    if (_args.iterator() != _args.end())
        throw std::runtime_error("Arguments are not allowed for --version.");
    return 0;
}

void gesture::excecute(int __args, const char **argv) {
    global::args _args{argv, uint64_t(__args)};
    _args.next();
    if (_args.iterator() == _args.end())
        gesture::version(_args);
    else
    {
        auto arg = _args.next();
        gesture::handle _handle = gesture::_S_operations[arg];
        if (!_handle)
            throw std::runtime_error("The command " + arg + " is not valid in this program.");
        _handle(_args);
    }
}

bool check_directory(const std::filesystem::path& path) {
    if (std::filesystem::is_empty(path))
        return true;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_YELLOW)
              << "[warning] The path: " << path.string() << " contains these directories: " << std::endl;
    std::cout << "[warning] Continuing will delete all files and directories inside: " << path.string() << ";" << std::endl << path.filename().string() << ':' << std::endl;
    for (auto &&p : std::filesystem::directory_iterator(path))
        std::cout << "\t- " << p.path().filename().string() << std::endl;
    std::cout << "[warning] Do you want to continue? [Y: Yes, any other key: No and stop the program]"
              << format() << std::endl;
    std::cout << ':';
    return std::toupper(std::getchar()) == 'Y';
}

uint32_t gesture::default_(global::args &_args)
{
    if (_args.iterator() == _args.end())
        throw std::runtime_error("No source file was provided.");
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Starting..."  << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Checking parameters..."  << format() << std::endl;
    global::source = std::filesystem::path(_args.next());
    if (!std::filesystem::exists(global::source))
        throw std::runtime_error("The file path " + global::source.string() + " does not exist.");
    if (!std::filesystem::is_regular_file(global::source))
        throw std::runtime_error("The path " + global::source.string() + " is not a file.");
    if (_args.iterator() != _args.end())
    {
        global::destine = std::filesystem::path(_args.next());
        if (!std::filesystem::exists(global::destine))
            std::filesystem::create_directory(global::destine);
        else if (!std::filesystem::is_directory(global::destine))
            throw std::runtime_error("The path " + global::destine.string() + " is not a directory.");
    }
    else
    {
        global::destine = terminal::current_path();
    }
    if (!check_directory(global::destine))
    {
        std::cout << format(format::style::NEGRITA, format::color::BRIGHT_YELLOW)
                << "[message] Operation cancelled." << format() << std::endl;
        return 0;
    }
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Verification completed." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Loading data..."  << format() << std::endl;
    if (global::source.extension() == ".yaml")
        parser::parser_with_yaml(global::source);
    else
        throw std::runtime_error("The file is not a slexer file; it must end in .yaml: " + global::source.string() + ".");
    clone::clone_(global::repository, global::destine);
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Data loaded." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Generating structure..."  << format() << std::endl;
    build::build_files(parser::includes(), global::destine);
    build::build_hpp(parser::hpp(), parser::metadata(), global::destine);
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Structure generated." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Building rules..."  << format() << std::endl;
    build::build_cpp(parser::cpp(), parser::metadata(), parser::options(), global::destine);
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Rules generated." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Compiling..."  << format() << std::endl;
    compiler::compile(global::destine);
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Compilation finished." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Cleaning and finishing..."  << format() << std::endl;
    clean::clear(global::destine, global::repository, parser::hpp(), parser::metadata());
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Operation finished." << format() << std::endl;
    return 0;
}

int main(int argc, const char *argv[])
{
    global::installing = std::move(std::filesystem::path(argv[0]).parent_path().parent_path());
    global::repository = std::move(global::installing / "repository");
    try
    {
        git_libgit2_init();
        gesture::excecute(argc, argv);
    }
    catch (const myregex::basic_lexicar_error<char>&e) {
        std::cerr << format(format::style::NEGRITA, format::color::BRIGHT_RED) << "[error] ";
        e.view(std::cerr);
        std::cerr << format() << std::endl;
        if(!global::destine.empty())
            std::filesystem::remove_all(global::destine);
        git_libgit2_shutdown();
        return EXIT_FAILURE;
    }
    catch (const std::exception &e)
    {
        std::cerr << format(format::style::NEGRITA, format::color::BRIGHT_RED) << "[error] " << e.what() << format() << std::endl;
        if(!global::destine.empty())
            std::filesystem::remove_all(global::destine);
        git_libgit2_shutdown();
        return EXIT_FAILURE;
    }
    git_libgit2_shutdown();
    return EXIT_SUCCESS;
}
