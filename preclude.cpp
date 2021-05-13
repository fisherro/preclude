//TODO: Need a list of file extensions to parse.
//TODO: Need a list of files/extensions to not copy.
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <unordered_set>

namespace fs = std::filesystem;

//No cross-platform support for hidden files in <filesystem>. Sigh.
bool is_hidden(const fs::path& p)
{
    std::string fn{p.filename()};
    return fn.starts_with(".");
}

void parse_it(const fs::path& p)
{
    std::cout << "Parsing " << p << '\n';
}

void copy_it(const fs::path& p)
{
    std::cout << "Copying " << p << '\n';
}

int main(const int argc, const char** argv)
{
    try {
        const std::unordered_set<std::string> to_parse{".html"};
        const std::unordered_set<std::string> dont_copy{".htmlf"};

        if (argc < 3) {
            std::cerr << "Usage: " << argv[0]
                << " input_directory output_directory\n";
            return EXIT_FAILURE;
        }

        const fs::path indir{argv[1]};
        const fs::path outdir{argv[2]};

        //To prevent recursion into hidden directories,
        //we need access to the iterator.
        //So, we can't use a ranged-for or other higher-level iteration.
        for (fs::recursive_directory_iterator i{indir};
                i != fs::recursive_directory_iterator{};
                ++i)
        {
            if (is_hidden(i->path())) {
                if (i->is_directory()) {
                    i.disable_recursion_pending();
                }
                continue;
            }

            if (to_parse.contains(i->path().extension())) {
                parse_it(i->path());
            } else if (not dont_copy.contains(i->path().extension())) {
                copy_it(i->path());
            } else {
                std::cout << "Skipping " << i->path() << '\n';
            }
        }

        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unrecognized exception\n";
        return EXIT_FAILURE;
    }
}
