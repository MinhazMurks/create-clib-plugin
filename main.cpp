#include <git2cpp/initializer.h>
#include "git2cpp/remote.h"
#include "git2cpp/repo.h"

#include <git2/clone.h>
#include <iostream>
#include <filesystem>

#define CREATE_CLIB_PLUGIN_VERSION @CREATE_CLIB_PLUGIN_VERSION@

struct FetchCallbacks final : git::Remote::FetchCallbacks
{
    FetchCallbacks()
    {
    }

    void sideband_progress(char const * str, int len) override
    {
    }

    void transfer_progress(git_indexer_progress const & progress) override
    {
    }

    git_credential * acquire_cred(const char * url, const char * username_from_url, unsigned allowed_types) override
    {
        return nullptr;
    }
};

int main(int argc, char* argv[]) {

    std::cout << std::filesystem::current_path();

    char * url = R"(https://github.com/mlthelama/ExamplePlugin-SKSE64.git)";
    char * name = "ExamplePlugin";
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
        std::cout << "Version " << CREATE_CLIB_PLUGIN_VERSION << std::endl;
        return 0;
    }
    else {
        name = argv[1];
    }
    for(int i = 2; i < argc; i++) {
        if (strcmp(argv[1], "-p") == 0) {
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
        if (strcmp(argv[1], "-r") == 0) {
            if (i + 1 >= argc) {
                std::cout << "Repository not properly defined! " << std::endl;
                return 1;
            }
            url = argv[i + 1];
        }
    }

    auto_git_initializer;
    FetchCallbacks fetch_callbacks = FetchCallbacks();
    git_checkout_options checkout_opts = {GIT_CHECKOUT_OPTIONS_VERSION, GIT_CHECKOUT_SAFE};
    try
    {
        //git::Repository::clone(url, path, checkout_opts, fetch_callbacks);
    }
    catch (git::repository_clone_error &err)
    {
        printf("\n");
        if (auto detailed_info = std::get_if<git::repository_clone_error::detailed_info>(&err.data))
            printf("ERROR %d: %s\n", detailed_info->klass, detailed_info->message);
        else
            printf("ERROR %d: no detailed info\n", std::get<int>(err.data));
        return EXIT_FAILURE;
    }

    return 0;
}
