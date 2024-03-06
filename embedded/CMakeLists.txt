############################################
############## 	Cmake Build	############
############################################

cmake_minimum_required(VERSION 3.12)
project(test)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
file(GLOB SOURCES "src/*.c" "src/*.cpp")
add_executable(btb_proj ${SOURCES})
target_link_libraries(btb_proj pigpio atomic)
set (CMAKE_SHARED_LINKER_FLAGS "-lpigpio -lrt -lpthread -lm -ldl -latomic")
target_compile_options(btb_proj PUBLIC -Wall)
#target_compile_options(btb_proj PUBLIC -Wall -lpigpio -lrt -lpthread -lm -ldl -latomic)

#target_link_options(btb_proj PUBLIC "LINKER: -lpigpio -lrt -lpthread -lm -ldl -latomic")


############################################
######### Notes/Other Functions	############
############################################

#SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wfloat-conversion")
#SET(GCC_COVERAGE_LINK_FLAGS    "-lpigpio -lrt -lpthread")

#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
#SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

#set (CMAKE_SHARED_LINKER_FLAGS "-lpigpio -lrt -lpthread")

#cmake_minimum_required(VERSION 3.12)

#project (Foo)

#file (GLOB_RECURSE Foo_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "include/*.cpp")
#file (GLOB_RECURSE Foo_HEADERS CONFIGURE_DEPENDS "include/*.h" "src/*.h" )

#set (Foo_INCLUDE_DIRS "")
#foreach (_headerFile ${Foo_HEADERS})
#    get_filename_component(_dir ${_headerFile} PATH)
#    list (APPEND Foo_INCLUDE_DIRS ${_dir})
#endforeach()
#list (REMOVE_DUPLICATES Foo_INCLUDE_DIRS)

#add_executable(FooExe ${Foo_SOURCES})
#target_compile_options(FooExe PUBLIC -std=c++1y -Wall -Wfloat-conversion -lpigpio -lrt -lpthread)
#target_include_directories(FooExe PRIVATE ${Foo_INCLUDE_DIRS})
#find_package(pigpio)



#cmake_minimum_required(VERSION 3.1.0)
#project (btb_proj_cmake)
#add_executable(project src/btb_main.cpp)


#file(GLOB_RECURSE sources src/*.cpp	include/*.h)



#target_compile_options(project PUBLIC -std=c++1y -Wall -Wfloat-conversion -lpigpio -lrt -lpthread)
#target_include_directories(project PUBLIC include/)
#target_include_directories(project PUBLIC src/)
#file(COPY ${data})
#find_package(Boost 1.36.0 stdlib string stdio unistd pigpio math time)
#find_package(stdlib)
#find_package(string)
#find_package(stdio)
#find_package(unistd)
#find_package(pigpio)
#find_package(math)
#find_package(time)
#target_link_libraries(project PUBLIC include)
#target_link_libraries(project PUBLIC src)
#target_link_libraries(project PUBLIC ${Boost_LIBRARIES})



#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_executable(demo DemoThread.cpp demo.cpp)
#find_package (Threads)
#target_link_libraries (demo ${CMAKE_THREAD_LIBS_INIT})
