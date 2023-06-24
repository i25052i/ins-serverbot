add_definitions(-D_WIN32_WINNT=0x0A00)

add_library(fsm fsm.cpp features.cpp parser.cpp)

target_include_directories(fsm PUBLIC
                           "include/"
                           "../rcon/"
                           )

target_link_libraries(fsm PUBLIC rcon)