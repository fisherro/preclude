# Preclude

Preclude takes a set of HTML files with server-side include directives and generates a corresponding set with the includes processed.

Note that this does not handle any of the other server-side include directives. Only `#include`.

    <!--#include file="header.htmlf"-->
    <!--#include virtual="header.htmlf"-->

Using `file` interprets the path as relative to the directory of the current file. Using `virtual` interprets the path relative to the input directory root.

Files and directories with names beginning with "." will be skipped. Only files with the extension ".html" will be processed. Files with the extension ".htmlf" (HTML Fragment) will be skipped.\* All other files will be copied to the output directory verbatim.

\*When included, the contents of an htmlf file will be processed as part of parsing the file including it.

## History

I needed a way to do static includes, and a friend convinced me that reüsing the SSI syntax was a good idea.

This was originally written in (_shudder_) Python. I rewrote it in C++ to better understand how closely C++ had come in expressiveness to a scripting language for such a task. At the time, (before C++11) I used boost and was surprised that I couldn't really say that Python or C++ with boost handled the task better than the other. Each version of the program has different areas that were rough but the bulk was quite similar.

Today, Preclude is no longer in active service, but I'm rewriting it again in C++20 to see where we are today and if I'll feel the need to enlist boost again.

## Observations

The pre-C++11 version used `BOOST_AUTO`, `BOOST_TYPEOF`, Boost regex, and Boost filesystem. With C++11, we have standard alternatives to the first three, and with C++17, we have the fourth in the standard. So, there's no longer anything I felt I needed to pull in from Boost.

I know the standard regex library is considered awful, but it gets the job done. I do wish there were some standard support for using regex with streams, whether it be a more limited form of regex or an arbitrary put-back stream adaptor.

It could really use some additional conveniences. e.g. A version of `std::regex_replace` that took a callable for generating the replacement text. But there's nothing that prevents you from writing that yourself.

C++20 gives us `std::string::starts_with`. I don't know that I would've bothered reaching for Boost just for that. It and `std::unordered_set::contains` do make the code clearer, though.

In general, for this task, C++20 provides some conveniences, but no great advantage over C++17.

A standard flat-map would have been appreciated. It didn't seem worth the trouble to use a sorted `std::vector` instead of `std::unordered_set`, and the speed and size penalties weren't a big enough concern here to do that or pull in Boost's flat-set.

It is also disappointing that we still don't have support for memory-mapped files in the standard.

There are some disappointing mismatches between different parts of the standard library. (I understand why they're there, but they are still disappointing.)

* Regex hasn't been updated for `std::string_view` or ranges.
* The `std::filesystem::recursive_directory_iterator` can't be used with ranged-for, algorithms, or ranges if you need to call `disable_recursion_pending`.

There are a couple of disappointing gaps in the filesystem library.

* No cross-platform way to identify hidden files.
* No simple way (that I found) to ask whether a given file lives under a given directory.

The useless metric of line count for the Python and C++ versions:

* Python: 149 lines
* C++: 124 lines

I'm still left with the impression that the choice of language for this project is simply which one the programmer is more familiar with.
