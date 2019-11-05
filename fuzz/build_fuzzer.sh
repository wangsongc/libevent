#!/bin/bash -eux
# This script is meant to be run by
#https://github.com/google/oss-fuzz/tree/master/projects/libevent/build.sh

# build libevent
mkdir -p build-cmake
cd build-cmake
cmake ..
make

# build fuzzer target
fuzzerFiles=$(find $SRC/libevent/fuzz/ -name "*.cc")
find . -name "*.a"

for F in $fuzzerFiles; do
  fuzzerName=$(basename $F .cc)
  echo "Building fuzzer $fuzzerName"
  $CXX $CXXFLAGS -std=c++11 \
      -o $OUT/${fuzzerName} $LIB_FUZZING_ENGINE $F \
      -I $SRC/libevent/include/event2 \
      -I $SRC/libevent/include \
      -I $SRC/libevent/build-cmake/include/event2 \
      -I $SRC/libevent/build-cmake/include \
      $SRC/libevent/build-cmake/lib/libevent.a \
      $SRC/libevent/build-cmake/lib/libevent_extra.a \
      $SRC/libevent/build-cmake/lib/libevent_pthreads.a \
      $SRC/libevent/build-cmake/lib/libevent_core.a \
      $SRC/libevent/build-cmake/lib/libevent_openssl.a 
  if [ -d "$SRC/libevent/fuzz/${fuzzerName}_corpus" ]; then
    zip -j $OUT/${fuzzerName}_seed_corpus.zip $SRC/libevent/fuzz/${fuzzerName}_corpus/*
  fi
done