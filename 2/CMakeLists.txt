
add_executable(AoC_2)

set(SRC
    main.cpp)

#target_compile_definitions
#target_include_directories
#target_link_libraries
set_property(TARGET AoC_2 PROPERTY CXX_STANDARD 20)
target_sources(AoC_2 PRIVATE ${SRC})
