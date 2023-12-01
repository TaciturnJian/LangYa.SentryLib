cmake_minimum_required(VERSION 3.16)

project(SentryLib)

# Cross Platform Support
if(CMAKE_SYSTEM_NAME MATCHES "Linux") 
    set(SC_PUBLIC_PTHREAD PUBLIC pthread)
	add_definitions(-DSC_LINUX)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows") 
    set(SC_PUBLIC_PTHREAD)
	add_definitions(-DSC_WINDOWS)
else()
    message(WARNING "The reliablity of this project on platforms except Windows|Linux are not guranteed")
endif()

# Sentry Lib Requires Following Packages, install with comments after them
find_package(spdlog REQUIRED) # apt-get install libspdlog-dev
find_package(Boost COMPONENTS system REQUIRED) # apt-get install libboost-all-dev
find_package(fmt REQUIRED) # apt-get install libfmd-dev
find_package(OpenCV 4 REQUIRED) # apt-get install libopencv-dev
find_package(Eigen3 REQUIRED) # apt-get install libEigen-dev

add_compile_options(-fconcepts)

aux_source_directory(Sources SentryLibSources)
add_library(${PROJECT_NAME} ${SentryLibSources})
target_include_directories(${PROJECT_NAME} BEFORE PUBLIC Headers)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME}
	${SC_PUBLIC_PTHREAD}
	PUBLIC Boost::system
	PUBLIC fmt::fmt
	PUBLIC spdlog::spdlog
	PUBLIC ${Eigen3_LIBS}
	PUBLIC ${OpenCV_LIBS}
)

aux_source_directory(Testers TesterSources)
foreach(TesterSource ${TesterSources})
	get_filename_component(TesterName ${TesterSource} NAME)
	message("Found Tester: ${TesterName} -> ${TesterSource}")

	add_executable(${TesterName} ${TesterSource})
	set_target_properties(${TesterName} PROPERTIES CXX_STANDARD 20)
	target_link_libraries(${TesterName} PRIVATE ${PROJECT_NAME})
endforeach()


