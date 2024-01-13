cmake_minimum_required(VERSION 3.16)

# 在这里设置项目的基本属性
project(SentryLib 
	VERSION 1.0.3
	DESCRIPTION "A C++ Library for Robot Programing"
	HOMEPAGE_URL "https://github.com/TaciturnJian/LangYa.SentryLib"
	LANGUAGES CXX
)

# DEBUG 模式
add_definitions(-DSC_DEBUG)
                                                                        
# 跨平台支持 在这里写了 pthread 和 compile options
if(CMAKE_SYSTEM_NAME MATCHES "Linux") 
    set(SC_PUBLIC_PTHREAD PUBLIC pthread)
	add_definitions(-DSC_LINUX)
	set(Boost_USE_STATIC_LIBS ON)
	add_compile_options(-fconcepts)
	message("!! You may need add new line in your cmake: 'add_compile_options(-fconcepts)' ")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows") 
    set(SC_PUBLIC_PTHREAD)
	add_definitions(-DSC_WINDOWS)
else()
    message(FATAL "!! The reliablity of this project on platforms except Windows|Linux are not guranteed")
endif()

message("!! If you met errors with 'boost' 'json', please install boost higher than 1.75.0")

# Sentry Lib 会用到这些库，后面有安装指令，其中可能名称是错误的，请使用 apt search 自行验证
find_package(spdlog REQUIRED) # apt-get install libspdlog-dev
find_package(Boost 1.83.0 REQUIRED COMPONENTS system program_options filesystem json) # apt-get install libboost-all-dev !! 这个指令安装的boost并不符合此项目的要求
find_package(fmt REQUIRED) # apt-get install libfmt-dev
find_package(OpenCV 4 REQUIRED) # apt-get install libopencv-dev
find_package(Eigen3 REQUIRED) # apt-get install libeigen3-dev

# 配置 LangYa.Common
aux_source_directory("./Common/Sources" LangYaCommonSources)
add_library(LangYaCommon ${LangYaCommonSources})
target_include_directories(LangYaCommon BEFORE PUBLIC "./Common/Headers")
set_target_properties(LangYaCommon PROPERTIES CXX_STANDARD 20)
target_link_libraries(LangYaCommon
	${SC_PUBLIC_PTHREAD}
	PUBLIC fmt::fmt
	PUBLIC spdlog::spdlog
	PUBLIC Boost::system
)

# 配置 LangYa.CodeLogic
aux_source_directory("./CodeLogic/Sources" LangYaCodeLogicSources)
add_library(CodeLogic ${LangYaCodeLogicSources})
target_include_directories(CodeLogic BEFORE PUBLIC "./CodeLogic/Headers")
set_target_properties(CodeLogic PROPERTIES CXX_STANDARD 20)
target_link_libraries(CodeLogic
	PUBLIC LangYaCommon
)

# 配置 LangYa.TinyMath
aux_source_directory("./TinyMath/Sources" TinyMathSources)
add_library(TinyMath ${TinyMathSources})
target_include_directories(TinyMath BEFORE PUBLIC "./TinyMath/Headers")
set_target_properties(TinyMath PROPERTIES CXX_STANDARD 20)
target_link_libraries(TinyMath
	PUBLIC LangYaCommon
)

# 配置 LangYa.ConsoleHelper
aux_source_directory("./ConsoleHelper/Sources" ConsoleHelperSources)
add_library(ConsoleHelper ${ConsoleHelperSources})
target_include_directories(ConsoleHelper BEFORE PUBLIC "./ConsoleHelper/Headers")
set_target_properties(ConsoleHelper PROPERTIES CXX_STANDARD 20)
target_link_libraries(ConsoleHelper
	PUBLIC TinyMath
)

# 配置 LangYa.Network
aux_source_directory("./Network/Sources" NetworkSources)
add_library(Network ${NetworkSources})
target_include_directories(Network BEFORE PUBLIC "./Network/Headers")
set_target_properties(Network PROPERTIES CXX_STANDARD 20)
target_link_libraries(Network
	PUBLIC LangYaCommon
)


# 配置 LangYa.ViMap
aux_source_directory("./ViMap/Sources" ViMapSources)
add_library(ViMap ${ViMapSources})
target_include_directories(ViMap BEFORE PUBLIC "./ViMap/Headers")
set_target_properties(ViMap PROPERTIES CXX_STANDARD 20)
target_link_libraries(ViMap
	PUBLIC TinyMath
)

# 配置头文件、库、源文件
aux_source_directory(Sources SentryLibSources)
add_library(${PROJECT_NAME} ${SentryLibSources})
target_include_directories(${PROJECT_NAME} BEFORE PUBLIC Headers)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME}
	PUBLIC LangYaCommon
	PUBLIC TinyMath
	PUBLIC ViMap
	PUBLIC Network
	PUBLIC ConsoleHelper
	${SC_PUBLIC_PTHREAD}
	PUBLIC Boost::system
	PUBLIC Boost::program_options
	PUBLIC Boost::filesystem
	PUBLIC Boost::json
	PUBLIC fmt::fmt
	PUBLIC spdlog::spdlog
	PUBLIC ${Eigen3_LIBS}
	PUBLIC ${OpenCV_LIBS}
)

# 添加测试应用程序
aux_source_directory(Testers TesterSources)
foreach(TesterSource ${TesterSources})
	get_filename_component(TesterName ${TesterSource} NAME_WLE)
	message("Found Tester: ${TesterName} -> ${TesterSource}")

	add_executable(${TesterName} ${TesterSource})
	set_target_properties(${TesterName} PROPERTIES CXX_STANDARD 20)
	target_link_libraries(${TesterName} PRIVATE ${PROJECT_NAME})
endforeach()

# 添加 SentryFramework
#add_executable(SentryDevice "./SentryFramework/SentryDevice.cpp")
#add_executable(SentryMind "./SentryFramework/SentryMind.cpp")
#add_executable(SentryMonitor "./SentryFramework/SentryMonitor.cpp")
#add_executable(SentryReferee "./SentryFramework/SentryReferee.cpp")
#target_link_libraries(SentryDevice PRIVATE ${PROJECT_NAME})
#target_link_libraries(SentryMind PRIVATE ${PROJECT_NAME})
#target_link_libraries(SentryMonitor PRIVATE ${PROJECT_NAME})
#target_link_libraries(SentryReferee PRIVATE ${PROJECT_NAME})
#set_target_properties(SentryDevice SentryMind SentryMonitor SentryReferee
#	PROPERTIES CXX_STANDARD 20
#)
