#Preclude

Preclude takes a set of HTML files with server-side include directives and generates a corresponding set with the includes processed.

Note that this does not handle any of the other server-side include directives. Only `#include` with a `file` parameter.

    <!--#include file="header.html"-->

(I needed a way to do static includes, and I was convinced that reüsing the SSI syntax was a good idea.)

This was originally written in _shudder_ Python. I rewrote it in C++ to better understand how closely C++ had come in expressiveness to a scripting language for such a task. At the time, (before C++11) I used boost and was surprised that I couldn't really say that Python or C++ with boost handled the task better than the other. Each version of the program has different areas that were rough but the bulk was quite similar.

Today, Preclude is no longer in active service, but I'm rewriting it again in C++20 to see where we are today and if I'll feel the need to enlist boost again.
