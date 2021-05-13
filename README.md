#Preclude

Preclude takes a set of HTML files with server-side include directives and generates a corresponding set with the includes processed.

Note that this does not handle any of the other server-side include directives. Only `#include`.

    <!--#include file="header.htmlf"-->
    <!--#include virtual="header.htmlf"-->

Using `file` interprets the path as relative to the directory of the current file. Using `virtual` interprets the path relative to the input directory root.

Files and directories with names beginning with "." will be skipped. Only files with the extension ".html" will be parsed. Files with the extension ".htmlf" (HTML Fragment) will be skipped.\* All other files will be copied to the output directory verbatim.

\*When included, the contents of an htmlf file will be parsed as part of parsing the file including it.

## History

I needed a way to do static includes, and I was convinced that reüsing the SSI syntax was a good idea.

This was originally written in _shudder_ Python. I rewrote it in C++ to better understand how closely C++ had come in expressiveness to a scripting language for such a task. At the time, (before C++11) I used boost and was surprised that I couldn't really say that Python or C++ with boost handled the task better than the other. Each version of the program has different areas that were rough but the bulk was quite similar.

Today, Preclude is no longer in active service, but I'm rewriting it again in C++20 to see where we are today and if I'll feel the need to enlist boost again.
