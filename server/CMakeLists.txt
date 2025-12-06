add_executable(server
    main.cpp
    server.cpp
)

target_link_libraries(server PRIVATE core)