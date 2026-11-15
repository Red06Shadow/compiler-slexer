#include <iostream>
#include <filesystem>
#include <global/global.hpp>
#include <core/parser/parser.hpp>
#include <core/clone/clone.hpp>
#include <core/build/build.hpp>
#include <core/compiler/compiler.hpp>
#include <core/clean/clean.hpp>
#include <myregex/exceptions/error.hpp>
/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
    global::installing = std::move(std::filesystem::path(argv[0]).parent_path().parent_path());
    global::repository = std::move(global::installing / "repository");
    try
    {
        git_libgit2_init();
        std::cout << "\033[1;37m" << "Iniciando..."  << "\033[0m" << std::endl;
        std::cout << "\033[1;37m" << "Verificando parametros..."  << "\033[0m" << std::endl;
        if (argc == 1)
        {
            global::source = "C:\\Proyectos(Red06Shadow)\\c++\\slexer-compiler\\test\\test0001.yaml";
            global::destine = "C:\\Proyectos(Red06Shadow)\\c++\\slexer-compiler\\test\\export";
        }
        else
        {

            if (argc > 3)
                throw std::runtime_error("Solo se permite un parametro.");
            global::source = std::filesystem::path(argv[1]);
            if (!std::filesystem::exists(global::source))
                throw std::runtime_error("La ruta al archivo " + global::source.string() + "; no existe.");
            if (!std::filesystem::is_regular_file(global::source))
                throw std::runtime_error("La ruta al archivo " + global::source.string() + "; no es un archivo.");    
            if (argc == 3)
            {
                global::destine = std::filesystem::path(argv[2]);
                if (!std::filesystem::exists(global::destine))
                    std::filesystem::create_directory(global::destine);
                else if (!std::filesystem::is_directory(global::destine))
                    throw std::runtime_error("La ruta al archivo " + global::destine.string() + "; no es un directorio.");
            }
            else
            {
                global::destine = global::source.parent_path() / "export";
            }
        }
        std::filesystem::create_directory(global::destine);
        std::cout << "\033[1;32m" << "Verificacion completada." << "\033[0m" << std::endl;
        std::cout << "\033[1;37m" << "Cargando datos..."  << "\033[0m" << std::endl;
        if (global::source.extension() == ".yaml")
            parser::parser_with_yaml(global::source);
        else
            throw std::runtime_error("El archivo no representa un archivo de slexer, debe terminar en slexer; " + global::source.string() + ".");
        clone::clone_(global::repository, global::destine);
        std::cout << "\033[1;32m" << "Datos cargados." << "\033[0m" << std::endl;
        std::cout << "\033[1;37m" << "Generando estructura..."  << "\033[0m" << std::endl;
        build::build_files(parser::includes(), global::destine);
        build::build_hpp(parser::hpp(), parser::metadata(), global::destine);
        std::cout << "\033[1;32m" << "Estructura generada." << "\033[0m" << std::endl;
        std::cout << "\033[1;37m" << "Construyendo reglas..."  << "\033[0m" << std::endl;
        build::build_cpp(parser::cpp(), parser::metadata(), parser::options(), global::destine);
        std::cout << "\033[1;32m" << "Reglas generada." << "\033[0m" << std::endl;
        std::cout << "\033[1;37m" << "Compilado..."  << "\033[0m" << std::endl;
        compiler::compile(global::destine);
        std::cout << "\033[1;32m" << "Compilacion terminda." << "\033[0m" << std::endl;
        std::cout << "\033[1;37m" << "Limpiando y terminando..."  << "\033[0m" << std::endl;
        clean::clear(global::destine, global::repository, parser::hpp(), parser::metadata());
        std::cout << "\033[1;32m" << "Operacion terminada." << "\033[0m" << std::endl;
    }
    catch (const myregex::basic_lexicar_error<char>&e) {
        std::cerr << "\033[1;31m";
        e.view(std::cerr);
        std::cerr << "\033[0m" << std::endl;
        if(!global::destine.empty())
            std::filesystem::remove_all(global::destine);
    }
    // catch (const myregex::basic_lexicar_error<wchar_t>&e) {
    //     std::cerr << "\033[1;31m";
    //     e.view(std::wcerr);
    //     std::cerr << "\033[0m" << std::endl;
    // }
    catch (const std::exception &e)
    {
        std::cerr << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
        if(!global::destine.empty()) 
            std::filesystem::remove_all(global::destine);
    }
    git_libgit2_shutdown();
    return 0;
}
