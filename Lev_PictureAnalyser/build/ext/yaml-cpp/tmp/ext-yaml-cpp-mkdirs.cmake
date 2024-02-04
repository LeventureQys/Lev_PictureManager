# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "H:/PictureAnalyser/Lev_PictureAnalyser/build/ext/yaml-cpp/src/ext-yaml-cpp"
  "H:/PictureAnalyser/Lev_PictureAnalyser/build/ext/yaml-cpp/src/ext-yaml-cpp-build"
  "H:/PictureAnalyser/Lev_PictureAnalyser/build/ext/yaml-cpp"
  "H:/PictureAnalyser/Lev_PictureAnalyser/build/ext/yaml-cpp/tmp"
  "H:/PictureAnalyser/Lev_PictureAnalyser/build/ext/yaml-cpp/src/ext-yaml-cpp-stamp"
  "H:/PictureAnalyser/Lev_PictureAnalyser/build/ext/yaml-cpp/src"
  "H:/PictureAnalyser/Lev_PictureAnalyser/build/ext/yaml-cpp/src/ext-yaml-cpp-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "H:/PictureAnalyser/Lev_PictureAnalyser/build/ext/yaml-cpp/src/ext-yaml-cpp-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "H:/PictureAnalyser/Lev_PictureAnalyser/build/ext/yaml-cpp/src/ext-yaml-cpp-stamp${cfgdir}") # cfgdir has leading slash
endif()
