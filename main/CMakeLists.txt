idf_component_register(SRCS 
                        "src/main.cpp"
                        "src/wifi/wifi.cpp"
                        "src/udp/udp.cpp"
                        INCLUDE_DIRS "include")

add_library(opus STATIC IMPORTED)
set_property(TARGET opus PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopus.a)
target_link_libraries(${COMPONENT_LIB} PUBLIC opus)