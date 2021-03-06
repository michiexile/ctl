# README #
This C++11 library provides a set of generic tools for:

 * Generating point sets (coming soon)
 * Building Neighborhood Graphs 
 * Building Cellular Complexes
 * Computing [persistent] homology over finite fields
 * Parallel algorithm(s) for homology
	
## LICENSE ##
 * CTL is licensed under the BSD. 
 * See LICENSE for more info. 

## DEPENDENCIES ##

CTL has a number of dependencies. We have listed them below.

We have made an effort to make getting an using these dependencies simple.

In particular, we strongly recommend installing them on your system either
as root or with the help of your system administrator via your operating systems
package manager e.g. apt on debian based systems, yum on RHEL systems, or 
brew/port/fink on OSX.

If you do not tell us specifically where these dependencies are, we will try to
find them for you via the CMake `find_package` system. 

If this fails or if you wish to use a local/system wide install of the library, 
simply update the corresponding entries in:
	build_dependencies.txt

If you do not have the software installed anywhere, then for most of our 
dependencies we provide make targets to download, and install them on your 
system.

For example:
	`make boost`

Will download and install boost into the dependencies/ directory.

You may also type:
	`make dependencies`

Which will download and install all of the dependencies into the dependencies/
directory.

For your convienence `build_dependencies.txt` already lists the directories
where this software should be installed to. So just uncomment!

If you encounter any issues, make sure to file it:
  https://github.com/appliedtopology/ctl/issues

This list of the dependencies is below and we have annoted 
which packages we can auto-help you install locally.

C++11 compiler (clang or gcc)
GTest (cmake .; make will download and compile automagically)
Boost 
Intel TBB 
METIS (make metis) (used for graph partitioning)
ANN (make ann) (used for nearest neighbor querying..)
Doxygen (Optional)

## BUILDING ##
0. You may specify paths to the include/link directories here:	
	`vim build_dependencies.txt`
1. CMake will use what you specify, and if you dont it will look on the system
   for dependencies (and ideally find them...)
2. Create makefiles
	`mkdir build; cd build; cmake ..`
3. Compile: (the -j option makes in parallel)
	`make -j`

### For OS/X Users: ###
If you plan on using the default compiler (clang)
then when installing boost ensure that you use:
	brew install boost ---build-from-source --with-c++11 --with-mpi --with-program_options --with-clang --without-single
and after `cmake .` ensure that the compiler chosen is clang via `ccmake .` /usr/bin/c++ 
is a good choice.

Since libstdc++ (GNU C++ STL) and libc++ (Clang C++ STL) are not ABI compatible
one needs to take care to use the correct compilers for libraries linked against.
if boost is compiled with gcc, you cannot use clang to compile CTL and vice versa.


## INSTALL ##
OS/X:
 `brew tap appliedtopology/software`
 `brew install ctl`

Fedora/CentOS/RHEL:(coming soon)
`yum install ctl` 

Ubuntu:  (coming soon)
 `apt-get install ctl`

## INSTALL (from source) ##
 `make install`

This sticks the headers into the default location on your system and all the tools into the appropriate path/bin directory

## USING (coming soon) ##
See tools/ or tests/ for a number of examples of using the library. 
We hope to write a tutorial soon.

## SUBMITTING PATCHES ##
Please Do! Accepting Pull Requests via github.

### Future ###
We hope to add support for:
 * [Persistent] Co-homology
 * Zig Zag Persistence
 * Multidimensional Persistence
 * Tidy Sets
 * OpenGL Visualizations
 * Bindings to other languages such as Python, MATLAB, and R
  
Feel free to fork and help development. Do ask questions!
