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
        throw std::runtime_error("No se permiten argumentos para --help.");
    return 0;
}

uint32_t gesture::version(global::args &_args)
{
    gesture::version();
    if (_args.iterator() != _args.end())
        throw std::runtime_error("No se permiten argumentos para --version.");
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
            throw std::runtime_error("El comando: " + arg + ", no es un comando valido dentro del programa.");
        _handle(_args);
    }
}

uint32_t gesture::default_(global::args &_args)
{
    if (_args.iterator() == _args.end())
        throw std::runtime_error("No se permiten argumentos para --version.");
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Iniciando..."  << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Verificando parametros..."  << format() << std::endl;
    global::source = std::filesystem::path(_args.next());
    if (!std::filesystem::exists(global::source))
        throw std::runtime_error("La ruta al archivo " + global::source.string() + "; no existe.");
    if (!std::filesystem::is_regular_file(global::source))
        throw std::runtime_error("La ruta al archivo " + global::source.string() + "; no es un archivo.");    
    if (_args.iterator() != _args.end())
    {
        global::destine = std::filesystem::path(_args.next());
        if (!std::filesystem::exists(global::destine))
            std::filesystem::create_directory(global::destine);
        else if (!std::filesystem::is_directory(global::destine))
            throw std::runtime_error("La ruta al archivo " + global::destine.string() + "; no es un directorio.");
    }
    else
    {
        global::destine = terminal::current_path();
    }
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Verificacion completada." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Cargando datos..."  << format() << std::endl;
    if (global::source.extension() == ".yaml")
        parser::parser_with_yaml(global::source);
    else
        throw std::runtime_error("El archivo no representa un archivo de slexer, debe terminar en slexer; " + global::source.string() + ".");
    clone::clone_(global::repository, global::destine);
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Datos cargados." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Generando estructura..."  << format() << std::endl;
    build::build_files(parser::includes(), global::destine);
    build::build_hpp(parser::hpp(), parser::metadata(), global::destine);
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Estructura generada." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Construyendo reglas..."  << format() << std::endl;
    build::build_cpp(parser::cpp(), parser::metadata(), parser::options(), global::destine);
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Reglas generada." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Compilado..."  << format() << std::endl;
    compiler::compile(global::destine);
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Compilacion terminda." << format() << std::endl;
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_WHITE)
                << "[message] Limpiando y terminando..."  << format() << std::endl;
    clean::clear(global::destine, global::repository, parser::hpp(), parser::metadata());
    std::cout << format(format::style::NEGRITA, format::color::BRIGHT_GREEN)
                << "[message] Operacion terminada." << format() << std::endl;
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
