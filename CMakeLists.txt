cmake_minimum_required(VERSION 3.11.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

project(Surv)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

file(GLOB project_SRCS src/*.cpp src/*.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package( OpenCV 4.7.0 REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

#target_link_libraries( DisplayImage ${OpenCV_LIBS} )


add_executable(surv ${project_SRCS})
target_link_libraries(surv ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})
target_include_directories(surv PRIVATE ${wxWidgets_INCLUDE_DIRS})
