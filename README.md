# The Static Kernel Analyzer (SKA)

# Requirements

* CMake >= 2.8
* C++ compiler with c++11 support
* LLVM >= 3.6.1
* Qt4
* Qwt >= 6.0
* Graphviz

To create LLVM IR input files for SKA you will need clang/clang++.

# Build & Installation

SKA uses the Cinch build system.  From a user-perspective, this is
equivalent to CMake.  To configure a build, do the following from
the top-level source directory:

```
% mkdir build
% cd build
```
**NOTE: ska prohibits in-source builds**

Then call your favorite version of CMake:

**Normal**
```
cmake -DENABLE_FEATURE -DCMAKE_INSTALL_PREFIX=/path/to/install ..
```

**Curses**
```
% ccmake ..
```

After configuration, simply type:

```
% make
% make install
```

# Running ska and skaview

The *ska* executable takes two filenames as arguements: an architecture
specification in .xml format, and an LLVM IR file of the source code
to be analyzed.  Optionally, you can specify an output file with the
'-o' flag, e.g.:

```
% ska -o saxby.ska pentium-m.xml saxby.ll
```

In this case, 'saxby.ska' is the output file, 'pentium-m.xml' is the
architecture file, and 'saxby.ll' is the LLVM IR file.

If you generate an output file, you can use the optional viewer program
*skaview* to look at the results.  *skaview* can be called with an
output file name to open:

```
% skaview saxby.ska
```

The viewer recognizes the file suffix, *.ska*.
