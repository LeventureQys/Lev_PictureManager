# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/Leventure/WorkShop/Python/PaddleDetection/deploy/cpp/ext/yaml-cpp/src/ext-yaml-cpp"
  "E:/Leventure/WorkShop/Python/PaddleDetection/deploy/cpp/ext/yaml-cpp/src/ext-yaml-cpp-build"
  "E:/Leventure/WorkShop/Python/PaddleDetection/deploy/cpp/ext/yaml-cpp"
  "E:/Leventure/WorkShop/Python/PaddleDetection/deploy/cpp/ext/yaml-cpp/tmp"
  "E:/Leventure/WorkShop/Python/PaddleDetection/deploy/cpp/ext/yaml-cpp/src/ext-yaml-cpp-stamp"
  "E:/Leventure/WorkShop/Python/PaddleDetection/deploy/cpp/ext/yaml-cpp/src"
  "E:/Leventure/WorkShop/Python/PaddleDetection/deploy/cpp/ext/yaml-cpp/src/ext-yaml-cpp-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/Leventure/WorkShop/Python/PaddleDetection/deploy/cpp/ext/yaml-cpp/src/ext-yaml-cpp-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/Leventure/WorkShop/Python/PaddleDetection/deploy/cpp/ext/yaml-cpp/src/ext-yaml-cpp-stamp${cfgdir}") # cfgdir has leading slash
endif()
