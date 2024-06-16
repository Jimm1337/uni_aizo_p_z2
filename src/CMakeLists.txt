add_executable(uni_aizo_p_z2 main.cpp
        aizo_ds_graphmatrix.cpp
        aizo_ds_graphlist.cpp
        aizo_tool_gengraph.cpp
        aizo_tool_readergraph.cpp
        aizo_tool_menu.cpp
        aizo_algo_mst.cpp
        aizo_algo_sp.cpp
        aizo_tool_measure.cpp)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Compile options
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(uni_aizo_p_z2 INTERFACE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
    if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
        target_compile_options(uni_aizo_p_z2 INTERFACE -g -O0)
    elseif (CMAKE_BUILD_TYPE EQUAL "RELEASE")
        target_compile_options(uni_aizo_p_z2 INTERFACE -O3)
    endif ()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(uni_aizo_p_z2 PUBLIC /W4 /w14640 /permissive- /Zc:preprocessor)
    if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
        target_compile_options(uni_aizo_p_z2 INTERFACE /Zi)
    elseif (CMAKE_BUILD_TYPE EQUAL "RELEASE")
        target_compile_options(uni_aizo_p_z2 INTERFACE /O2)
    endif ()
endif ()

# Libraries
target_link_libraries(uni_aizo_p_z2 PUBLIC fmt::fmt)
target_include_directories(uni_aizo_p_z2 PUBLIC ${fmt_INCLUDE_DIRS} ${gsl_SOURCE_DIR}/include ${scn_INCLUDE_DIRS})
