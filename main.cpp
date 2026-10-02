#include <core/parser/parser.hpp>
#include <filesystem>
#include <core/generate/generate.hpp>
#include <core/export/export.hpp>
#include <core/compiler/compiler.hpp>

/////////////////////////////////////////////////////////////////////////////////////////
std::vector<myregex::table<size_t>> _M_tables {};
std::vector<myregex::wtable<size_t>> _M_wtables {};
/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
    try
    {
        if (argc == 1)
            return 0;
        std::cout << "\033[1;37m" << "Iniciando..."  << "\033[0m" << std::endl;
        if (argc > 3)
            throw std::runtime_error("Solo se permite un parametro.");
        std::filesystem::path path{argv[1]};
        if (!std::filesystem::exists(path))
            throw std::runtime_error("La ruta al archivo " + path.string() + "; no existe.");
        if (!std::filesystem::is_regular_file(path))
            throw std::runtime_error("La ruta al archivo " + path.string() + "; no es un archivo.");
        if (path.extension() != ".slex")
            throw std::runtime_error("El archivo no representa un archivo de slexer, debe terminar en slexer; " + path.string() + ".");
            
        std::filesystem::path folder;
        if (argc == 3)
        {
            folder = std::filesystem::path(argv[2]);
            if (!std::filesystem::exists(folder))
                std::filesystem::create_directory(folder);
            else if (!std::filesystem::is_directory(folder))
                throw std::runtime_error("La ruta al archivo " + folder.string() + "; no es un directorio.");
        }
        else
        {
            folder = path.parent_path() / "export";
        }
        
        if (!std::filesystem::exists(folder))
            std::filesystem::create_directory(folder);
        std::filesystem::path hpp = folder / "ylexer.hpp";
        std::filesystem::path cpp = folder / "ylexer.cpp";
        
        parser::outhpp = std::basic_ofstream<char>(hpp);
        parser::outcpp = std::basic_ofstream<char>(cpp);
        parser::outhpp << "#ifndef YOURSLEXER" << std::endl
               << "#define YOURSLEXER" << std::endl;
        std::cout << "\033[1;37m" << "Cargando datos..."  << "\033[0m" << std::endl;
        std::basic_ifstream<char> in{std::filesystem::path(argv[0]).parent_path() / "clone" / "master.hpp"};
        char buffer[4096] = {};
        size_t n = 0;
        do
        {
            n = in.read(buffer, 4096).gcount();
            parser::outhpp.write(buffer, n);
        } while (!in.eof());
        in.close();
        in = std::basic_ifstream<char>(path);
        parser::outhpp << "#pragma yourcode\n//your code" << std::endl;
        parser::load(in);
        std::cout << "\033[1;32m" << "Datos procesados." << "\033[0m" << std::endl;
        parser::outhpp << "#pragma yourcode\n//your code" << std::endl << "extern slexer::basic_lexer<" << parser::chartype << "," << parser::idtype << "> ylexer;" << std::endl
               << "#endif" << std::endl;
        in.close();
        std::cout << "\033[1;37m" << "Generando tablas..." << "\033[0m" << std::endl;
        generate::build_char_expresions(parser::expresions, _M_tables);
        std::cout << "\033[1;32m" << "Tablas generadas correctamente." << "\033[0m" << std::endl;
        std::cout << "\033[1;37m" << "Exportando datos finales..." << "\033[0m" << std::endl;
        export_::export_tables_char(_M_tables, parser::expresions, parser::idtype, parser::outcpp);
        std::cout << "\033[1;32m" << "Datos finales exportados." << "\033[0m" << std::endl;
        parser::outhpp.close();
        parser::outcpp.close();

        std::cout << "\033[1;37m" << "Compilando libreria..." << "\033[0m" << std::endl;
        if (!compiler::compile(folder, hpp, cpp)) 
            throw std::runtime_error("Error: al compilar los archivos.");
        std::cout << "\033[1;32m" << "Compilacion completada." << "\033[0m" << std::endl;
    }
    catch (const myregex::basic_regex_error<char>&e) {
        std::cerr << "\033[1;31m" << e.what()
        << "\033[1;31m" << e.especification() << "\033[0m" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
    }
    return 0;
}
