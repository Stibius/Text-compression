cmake_minimum_required (VERSION 3.8)

# solution
project(bwted)
# project
set(APP_NAME "bwted")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES
   src/BlockCoder.h
   src/BWT_MTF_RLE_Huffman_Coder.h
   src/BWTCoder.h
   src/Coder.h
   src/HuffmanCoder.h
   src/HuffmanTree.h
   src/MTFCoder.h
   src/RLE0Coder.h
   src/StreamCoder.h
)

set(SOURCE_FILES
   src/BWT_MTF_RLE_Huffman_Coder.cpp
   src/BWTCoder.cpp
   src/Coder.cpp
   src/HuffmanCoder.cpp
   src/HuffmanTree.cpp
   src/main.cpp
   src/MTFCoder.cpp
   src/RLE0Coder.cpp
   src/StreamCoder.cpp
)

# Define a grouping for source files in IDE project generation
source_group("Source Files" FILES ${SOURCE_FILES})

# Define a grouping for source files in IDE project generation
source_group("Header Files" FILES ${HEADER_FILES})

add_executable(${APP_NAME} ${SOURCE_FILES} ${HEADER_FILES})
