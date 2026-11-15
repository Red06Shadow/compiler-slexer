#include "clone.hpp"

void clone::_S_pull_repo(const std::filesystem::path &path)
{
    git_repository *repo = nullptr;
    int error = git_repository_open(&repo, path.string().c_str());
    if (error != 0)
    {
        const git_error *e = git_error_last();
        throw std::runtime_error("Critical error: " + std::string(e && e->message ? e->message : "Error desconocido"));
    }
    git_remote *remote = nullptr;
    error = git_remote_lookup(&remote, repo, "origin");
    if (error != 0)
    {
        git_repository_free(repo);
        throw std::runtime_error("Error al verificar el repositorio: " + std::string(git_error_last()->message));
    }
    error = git_remote_fetch(remote, nullptr, nullptr, nullptr);
    if (error != 0)
    {
        const git_error *e = git_error_last();
        git_remote_free(remote);
        git_repository_free(repo);
        throw std::runtime_error("No ha podido competare el fecth del repositorio: " + std::string(e && e->message ? e->message : "Error desconocido"));
    }

    git_reference *localref = nullptr;
    git_reference *remoteref = nullptr;

    error = git_repository_head(&localref, repo);
    if (error != 0)
    {
        git_remote_free(remote);
        git_repository_free(repo);
        throw std::runtime_error("No existe el HEAD en este repositorio.");
    }

    error = git_reference_lookup(&remoteref, repo, "refs/remotes/origin/main");
    if (error != 0)
    {
        error = git_reference_lookup(&remoteref, repo, "refs/remotes/origin/master");
        if (error != 0)
        {
            git_reference_free(localref);
            git_remote_free(remote);
            git_repository_free(repo);
            throw std::runtime_error("No pudo encontrarse la rama de lectura.");
        }
    }

    git_oid local_oid = *git_reference_target(localref), remote_oid = *git_reference_target(localref);
    if (git_oid_equal(&local_oid, &remote_oid))
    {
        git_reference_free(localref);
        git_reference_free(remoteref);
        git_remote_free(remote);
        git_repository_free(repo);
        return;
    }
    git_annotated_commit *merge_commit = nullptr;
    error = git_annotated_commit_lookup(&merge_commit, repo, &remote_oid);
    if (error != 0)
    {
        std::cerr << "No se pudo preparar el merge.\n";
        git_reference_free(remoteref);
        git_reference_free(localref);
        git_remote_free(remote);
        git_repository_free(repo);
        throw std::runtime_error("No pudo prepararse el merge.");
    }

    git_merge_analysis_t analysis;
    git_merge_preference_t preference;
    error = git_merge_analysis(&analysis, &preference, repo, const_cast<const git_annotated_commit**>(&merge_commit), 1);

    if (error == 0 && (analysis & GIT_MERGE_ANALYSIS_UP_TO_DATE) == 0)
    {
        std::cout << "Clonando repositorio..." << std::endl;
    }

    git_annotated_commit_free(merge_commit);
    git_reference_free(remoteref);
    git_reference_free(localref);
    git_remote_free(remote);
    git_repository_free(repo);
}

void clone::_S_clone_repo(const std::filesystem::path &path)
{
    const std::basic_string<char> url = "C:\\Proyectos(Red06Shadow)\\c++\\slexer";
    git_repository *repo = nullptr;
    git_clone_options options = GIT_CLONE_OPTIONS_INIT;
    options.checkout_branch = "main";
    std::cout << "Clonando repositorio..." << std::endl;
    int error = git_clone(&repo, url.c_str(), path.string().c_str(), &options);
    if (error != 0)
    {
        const git_error *e = git_error_last();
        throw std::runtime_error("Error al clonar el repositorio: " + std::string(e && e->message ? e->message : "Error desconocido"));
    }
    std::cout << "Repositorio clonado correctamente." << std::endl;
    git_repository_free(repo);
}

void clone::_S_copy_repo(const std::filesystem::path &destine, const std::filesystem::path &source)
{
    for (auto &&dir : std::filesystem::directory_iterator(source))
    {
        if (dir.is_directory() && dir.path().filename() != ".." && dir.path().filename() != "builder" && dir.path().filename() != ".git")
        {
            std::filesystem::path _destine = destine / dir.path().filename();
            std::filesystem::path _source = source / dir.path().filename();
            if (!std::filesystem::exists(_destine))
                std::filesystem::create_directories(_destine);
            clone::_S_copy_repo(_destine, _source);
        }
        else if (dir.is_regular_file() && dir.path().filename() != "CMakeLists.txt" && dir.path().filename() != "slexer.hpp" && dir.path().filename() != "myregex.hpp")
        {
            std::filesystem::copy_file(dir.path(), destine / dir.path().filename(), std::filesystem::copy_options::overwrite_existing);
        }
    }
    ////////////////////////////////////////////////////////////////
}

void clone::clone_(
    const std::filesystem::path &repository,
    const std::filesystem::path &destine)
{
    ///////////////////////////////////////////////////////////////////////////////////////
    if (!std::filesystem::exists(repository))
        std::filesystem::create_directories(repository);
    else if (!std::filesystem::is_directory(repository))
        throw std::runtime_error("La direccion del path del repositorio no es un directorio valido.");
    ///////////////////////////////////////////////////////////////////////////////////////
    if (std::filesystem::is_empty(repository))
        clone::_S_clone_repo(repository);
    else
        clone::_S_pull_repo(repository);
    ///////////////////////////////////////////////////////////////////////////////////////
    clone::_S_copy_repo(destine, repository);
}