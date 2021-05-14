//TODO: Need a list of file extensions to parse.
//TODO: Need a list of files/extensions to not copy.
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ranges>
#include <regex>
#include <unordered_set>

namespace fs = std::filesystem;

//No cross-platform support for hidden files in <filesystem>. Sigh.
bool is_hidden(const fs::path& p)
{
    std::string fn{p.filename()};
    return fn.starts_with(".");
}

//Is there a better way?
bool is_file_in_directory(const fs::path& file, const fs::path& dir)
{
    std::string f{fs::absolute(file)};
    std::string d{fs::absolute(dir)};
    return f.starts_with(d);
}

std::ostreambuf_iterator<char> process(const fs::path& root, const fs::path& file, std::ostreambuf_iterator<char> out)
{
    static const std::regex rx{
        R"RX(<!--\s*#include\s+(virtual|file)\s*=\s*"([^"]+)"\s*-->)RX"
    };

    std::cout << "Processing " << file << '\n';

    if (not is_file_in_directory(file, root)) {
        std::cerr << std::quoted(file.string()) << " is not in " << std::quoted(root.string()) << '\n';
        return out;
    }

    //The regex library requires bidirectional iterators,
    //so we need the whole file in memory.
    //(Or write an arbitrary length push-back stream adaptor.)
    std::ifstream fs{file};
    const std::string text(std::istreambuf_iterator<char>{fs}, {});

    std::sregex_iterator rx_iter{text.begin(), text.end(), rx};
    std::sregex_iterator rx_end;

    auto end_of_match{text.cbegin()};
    for (const auto& match: std::ranges::subrange{rx_iter, rx_end}) {
        end_of_match = match.suffix().first;
        out = std::copy(match.prefix().first, match.prefix().second, out);
        const std::string how{match.str(1)};
        fs::path to_include{file.parent_path() / match.str(2)};
        if ("virtual" == how) {
            to_include = root / match.str(2);
        }
        out = process(root, to_include, out);
    }
    return std::copy(end_of_match, text.cend(), out);
}

void process_file(const fs::path& root, const fs::path& file, const fs::path& outpath)
{
    std::cout << "root: " << root << "; file: " << file << "; out: " << outpath << '\n';
#if 0
    fs::create_directories(outpath.parent_path());
    std::ofstream outstream{outpath};
    process(root, file, std::ostreambuf_iterator{outstream});
#endif
    process(root, file, std::ostreambuf_iterator{std::cout});
    std::cout << '\n';
}

void copy_file(const fs::path& p)
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

        std::cout << "In:\t" << indir << '\n';
        std::cout << "Out:\t" << outdir << '\n';

        //To prevent recursion into hidden directories,
        //we need access to the iterator.
        //So, we can't use a ranged-for or other higher-level iteration.
        for (fs::recursive_directory_iterator i{indir};
                i != fs::recursive_directory_iterator{};
                ++i)
        {
            const fs::path inpath{i->path()};
            if (is_hidden(inpath)) {
                if (i->is_directory()) {
                    i.disable_recursion_pending();
                }
                continue;
            }

            const fs::path outpath{outdir / fs::relative(inpath, indir)};
            if (to_parse.contains(inpath.extension())) {
                process_file(indir, inpath, outpath);
            } else if (not dont_copy.contains(inpath.extension())) {
                copy_file(inpath, outpath);
                //fs::copy_file(inpath, outpath);
            } else {
                std::cout << "Skipping " << inpath << '\n';
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
