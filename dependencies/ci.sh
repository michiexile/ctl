#!/bin/bash

if [ "$TRAVIS_OS_NAME" == "linux" ]; then 
     sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y 
     sudo apt-get update -qq  
     if [ "$CXX" == "g++" ]; then sudo apt-get install -qq g++-4.8; fi
     if [ "$CXX" == "g++" ]; then export CXX="g++-4.8" CC="gcc-4.8"; fi
     sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 40 --slave /usr/bin/g++ g++ /usr/bin/g++-4.8 
     sudo update-alternatives --config gcc 
     sudo apt-get install doxygen libann-dev libboost-all-dev libtbb-dev libmetis-dev 
else
     brew install tbb metis doxygen openmpi ann 
     brew install boost ---build-from-source --with-c++11 --with-mpi --with-program_options --with-clang --without-single
fi  
