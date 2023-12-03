
add_executable(AoC_3)

set(SRC
    main.cpp)

#target_compile_definitions
#target_include_directories
#target_link_libraries
set_property(TARGET AoC_3 PROPERTY CXX_STANDARD 20)
target_sources(AoC_3 PRIVATE ${SRC})
