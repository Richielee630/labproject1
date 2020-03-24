set(HEADER_FILES
        inc/inventory_node.h inc/inventory.h inc/queue_node.h inc/queue.h)

set(SOURCE_FILES
        src/inventory.cpp src/inventory_node.cpp src/queue.cpp)
include_directories(inc)
add_library(lib_lab_exam STATIC ${SOURCE_FILES} ${HEADER_FILES})