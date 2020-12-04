find_package(Threads REQUIRED)

add_library(sqlite3 sqlite3.c sqlite3.h)

target_link_libraries(sqlite3 Threads::Threads)
target_link_libraries(sqlite3 ${CMAKE_DL_LIBS})
