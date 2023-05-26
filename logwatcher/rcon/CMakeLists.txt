add_definitions(-D_WIN32_WINNT=0x0A00)

add_library(rcon rcon.cpp rcon_connection.cpp)

#path to boost root directory 
target_include_directories(rcon PUBLIC
                           "../../../boost_1_82_0" 
                           )