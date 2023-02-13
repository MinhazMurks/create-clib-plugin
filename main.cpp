#include <git2cpp/initializer.h>
#include "git2cpp/remote.h"
#include "git2cpp/repo.h"

#include <git2/clone.h>


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

int main(int argc, char *argv[]) {

    const char * url = R"(https://github.com/mlthelama/ExamplePlugin-SKSE64.git)";
    const char * path = R"(B:\Everything\Repositories\create-clib-plugin\testpath)";

    auto_git_initializer;
    FetchCallbacks fetch_callbacks = FetchCallbacks();
    git_checkout_options checkout_opts = {GIT_CHECKOUT_OPTIONS_VERSION, GIT_CHECKOUT_SAFE};
    try
    {
        git::Repository::clone(url, path, checkout_opts, fetch_callbacks);
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
