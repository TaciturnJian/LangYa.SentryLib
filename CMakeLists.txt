cmake_minimum_required(VERSION 3.16)

project(SentryLib 
	VERSION 1.0.1
	DESCRIPTION "A C++ Library for Robot Programing"
	HOMEPAGE_URL "https://github.com/TaciturnJian/LangYa.SentryLib"
	LANGUAGES CXX
)
                                                                        
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
    message(WARNING "!! The reliablity of this project on platforms except Windows|Linux are not guranteed")
endif()

message("!! If you met errors with 'boost' 'json', please install boost higher than 1.75.0")
###### 无法使用 boost json 报错的解决方式
## 它的报错类似于下面这样：
# CMake Error at /usr/share/cmake-3.22/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
# Could NOT find Boost: Found unsuitable version "1.74.0", but required is at
#	least "1.75.0" (found /usr/include, found components: system
#	program_options filesystem)
# Call Stack (most recent call first):
#	/usr/share/cmake-3.22/Modules/FindPackageHandleStandardArgs.cmake:592 (_FPHSA_FAILURE_MESSAGE)
#	/usr/share/cmake-3.22/Modules/FindBoost.cmake:2360 (find_package_handle_standard_args)
#	CMakeLists.txt:32 (find_package)
## 解决方式是安装最新版本的 boost 库，安装方式如下：（别想着我会给你写一次性就能成功执行的脚本，自己检查错误去;）
#> 安装步骤1：（结果还是写了个脚本，在这个项目的Scripts下）
# 执行里面的IntsallBoost.sh脚本，它会自动下载boost，解压，编译，安装，然后把库文件放到/usr/local/lib下，头文件放到/usr/local/include下
##
#> 安装步骤2：使用 vcpkg 的
# 第一步，下载基本工具
# $ sudo apt-get update && sudo apt-get install -y zip tar wget curl git cmake build-essential automake libtool m4 autoconf autoconf-archive
# 第二步，下载 vcpkg，接下来，别sudo
# $ git clone https://github.com/microsoft/vcpkg ~/.vcpkg
# 第三步，执行安装脚本
# $ cd ~/.vcpkg
# $ ./bootstrap-vcpkg.sh
# 第四步，安装 boost
# $ ./vcpkg install boost:x64-<SYSTEM> # 这里的<SYSTEM>替换为 linux 或 windows
# 第五步，去听几首歌或刷视频，在正常的电脑上一般很快(10min)就安装好了。
# 最后一步，集成到cmake
# $ ./vcpkg integrate install
## 接下来调用 cmake 时需要手动指定工具链，上一行指令有提到，类似于下面的内容：
# CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=<vcpkg文件夹路径>/.vcpkg/scripts/buildsystems/vcpkg.cmake"
# 即调用 cmake 时，加上后缀参数：
# $cmake .. -DCMAKE_TOOLCHAIN_FILE=<vcpkg文件夹路径>/.vcpkg/scripts/buildsystems/vcpkg.cmake
### 

# Sentry Lib 会用到这些库，后面有安装指令，其中可能名称是错误的，请使用 apt search 自行验证
find_package(spdlog REQUIRED) # apt-get install libspdlog-dev
find_package(Boost 1.83.0 REQUIRED COMPONENTS system program_options filesystem json) # apt-get install libboost-all-dev !! 这个指令安装的boost并不符合此项目的要求
find_package(fmt REQUIRED) # apt-get install libfmt-dev
find_package(OpenCV 4 REQUIRED) # apt-get install libopencv-dev
find_package(Eigen3 REQUIRED) # apt-get install libeigen3-dev

# 配置头文件、库、源文件
aux_source_directory(Sources SentryLibSources)
add_library(${PROJECT_NAME} ${SentryLibSources})
target_include_directories(${PROJECT_NAME} BEFORE PUBLIC Headers)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME}
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
