cmake_minimum_required(VERSION 3.16)

project(SentryLib)

# Sentry Lib Requires Following Packages, install with comments after them
find_package(spdlog REQUIRED) # apt-get install libspdlog-dev
find_package(Boost COMPONENTS system REQUIRED) # apt-get install libboost-all-dev
find_package(fmt REQUIRED) # apt-get install libfmd-dev
find_package(OpenCV 4 REQUIRED)
find_package(Eigen3 REQUIRED)

aux_source_directory(Sources SentryLibSources)
add_library(${PROJECT_NAME} ${SentryLibSources})
target_include_directories(${PROJECT_NAME} BEFORE PUBLIC Headers)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME}
	PUBLIC pthread
	PUBLIC Boost::system
	PUBLIC fmt::fmt
	PUBLIC spdlog::spdlog
	PUBLIC ${Eigen3_LIBS}
	PUBLIC ${OpenCV_LIBS}
)
