#include "create_clib_plugin.h"
#include "git2cpp/remote.h"
#include "git2cpp/repo.h"


#include <git2cpp/initializer.h>
#include <git2/clone.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>


void replace_with_name(std::filesystem::path& file_path, std::string& project_name, std::string& replacement_text) {
    std::fstream file_stream;
    std::string line;
    file_stream.open(file_path.string().c_str(), std::fstream::in);
    if(file_stream.is_open()) {
        std::stringstream buffer;
        while (std::getline(file_stream, line)) {
            auto starting_position = line.find(replacement_text);
            if (starting_position != std::string::npos) {
                std::string removed = line.replace(starting_position, replacement_text.length(), "");
                std::string replaced = line.insert(starting_position, project_name);
                buffer << replaced;
            } else {
                buffer << line;
            }
            buffer << std::endl;
        }
        file_stream.close();

        file_stream.open(file_path.string().c_str(), std::fstream::out);
        file_stream << buffer.str();
        file_stream.close();
    }
}

struct FetchCallbacks final : git::Remote::FetchCallbacks
{
    FetchCallbacks() = default;

    void sideband_progress(char const * str, int len) override {}

    git_credential * acquire_cred(const char * url, const char * username_from_url, unsigned allowed_types) override
    {
        return nullptr;
    }
};

int main(int argc, char* argv[]) {
    std::string url = R"(https://github.com/mlthelama/ExamplePlugin-SKSE64.git)";
    std::string name = "ExamplePlugin";
    std::string cmake_lists_replacement_text = "ExamplePlugin";
    std::string vcpkg_replacement_text = "exampleplugin";

    bool replace_project_name = true;

    std::filesystem::path path = std::filesystem::current_path();

    if(argc < 2) {
        std::cout << "You must include at least 1 argument!" << std::endl;
        return 1;
    }
    if (strcmp(argv[1], "--help") == 0) {
        std::cout << "Usage:" << std::endl;
        std::cout << "-p: " << "Specify path of parent directory the project will be created in" << std::endl;
        std::cout << "-r: " << "Specify repository of template directory. Note, if this option is set, the project name may not be automatically changed." << std::endl;
        return 0;
    }
    else if (strcmp(argv[1], "--version") == 0) {
        std::cout << "Version " << create_clib_plugin_VERSION_MAJOR << "." << create_clib_plugin_VERSION_MINOR << "." << create_clib_plugin_VERSION_PATCH << std::endl;
        return 0;
    }
    else {
        name = argv[1];
    }

    for(int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            if (i + 1 >= argc) {
                std::cout << "Path not properly defined!" << std::endl;
                return 1;
            }
            path = argv[i + 1];
            if(!std::filesystem::exists(path)) {
                std::cout << "Path specified does not exist!" << std::endl;
                return 1;
            }
        }
        if (strcmp(argv[i], "-r") == 0) {
            if (i + 1 >= argc) {
                std::cout << "Repository not properly defined! " << std::endl;
                return 1;
            }
            url = argv[i + 1];
            replace_project_name = false;
        }
    }

    path /= name;
    auto_git_initializer;

    FetchCallbacks fetch_callbacks = FetchCallbacks();
    git_checkout_options checkout_opts = {GIT_CHECKOUT_OPTIONS_VERSION, GIT_CHECKOUT_SAFE};
    try
    {
        git::Repository::clone(url.c_str(), path.string().c_str(), checkout_opts, fetch_callbacks);
    }
    catch (git::repository_clone_error &err)
    {
        std::cout << std::endl;
        if (auto detailed_info = std::get_if<git::repository_clone_error::detailed_info>(&err.data))
        {
            std::cout << "ERROR " << detailed_info->klass << ": " << detailed_info->message << std::endl;
        }
        else {
            std::cout << "ERROR " << std::get<int>(err.data) << ": no detailed info" << std::endl;
        }
        return EXIT_FAILURE;
    }

    std::filesystem::path git_dir = path / ".git";
    std::filesystem::remove_all(git_dir);

    if (replace_project_name) {
        std::filesystem::path cmake_lists_file = path / "CMakeLists.txt";
        std::filesystem::path vcpkg_file = path / "vcpkg.json";

        replace_with_name(cmake_lists_file, name, cmake_lists_replacement_text);
        replace_with_name(vcpkg_file, name, vcpkg_replacement_text);
    }


    return 0;
}
