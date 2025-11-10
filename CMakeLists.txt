cmake_minimum_required(VERSION 4.1) # minimum version of CMake required to use this file
set(CMAKE_CXX_COMPILER "g++") # or clang++

# project() is a macro that sets up the basic project information. It takes several arguments, including VERSION, DESCRIPTION, and LANGUAGES.
# create project with the name specified above, version and description. LANGUAGES CXX is used to specify that the project is a C++ project
# CMAKE could be uesed to build C, C++, CUDA(VERSION 3.8+), and other languages. 
project(cmake_intro VERSION 2.1.3 DESCRIPTION "Intro to C++" LANGUAGES C CXX)

# set the C++ standard to C++XX, if using modern C++ features, u have to set the standard to 11 or higher.
set(CMAKE_CXX_STANDARD 23) # options: 98, 11, 14, 17, 20, 23, 26
# set the C++ standard required and C++ extensions to OFF, if using older C++ features, u can set these to ON.
# if CMAKE_CXX_STANDARD_REQUIRED is ON, the compiler will issue an error if the standard is not met.
# if CMAKE_CXX_EXTENSIONS is ON, the compiler will enable all C++ extensions, even if they are not part of the standard. eg. typeof(<var>) new_variable;
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

# !!!!!! IMPORTANT !!!!!! 如果接受C++标准的降级,可以CMAKE_CXX_STANDARD_REQUIRED设置为OFF,以增强向下兼容性.
# !如果团队开发要求严格,可以CMAKE_CXX_STANDARD_REQUIRED设置为ON,以确保代码符合标准.
# !CMAKE_CXX_EXTENSIONS设置为ON,可以开启一些C++扩展,但不符合标准.特别是在跨平台/多人协作中出现问题.(不同编译器之间的插件不兼容)

# 将src目录下的源文件编译成库my_lib,并设置头文件搜索路径为include目录. 一般情况下,普通变量使用小写的驼峰命名法,而缓存变量名大写的下划线命名法.
aux_source_directory(${CMAKE_SOURCE_DIR}/src srcs) # *这样收集src目录下的源文件,但是不会递归收集子目录下的源文件
# file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp) # *递归收集src目录下的源文件,不推荐file()这种方式
# file(GLOB SRCS ${CMAKE_SOURCE_DIR})
add_library(my_lib SHARED ${srcs})
target_include_directories(my_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# add_executable() is a macro that creates an executable from a source file. 
# It takes two arguments, the name of the executable and the source file including main() function.
add_executable(Intro main.cpp)
# link the executable with the library created above.
target_link_libraries(Intro my_lib)

# option(MY_SWAP "custom swap function?" 1) # 这里的1(ON或者TRUE)表示默认开启,0(OFF或者FALSE)表示默认关闭
# configure_file(${CMAKE_SOURCE_DIR}/config/config.h.in ${CMAKE_SOURCE_DIR}/include/config.h) # configure_file() is a macro that copies/configures a file from the source directory to the build directory.

# * 一种更为现代的CMAKE项目配置方式,使用更加简洁的语法,设置构建目标的属性,如下. 
# * 这样,在project()中只需要设置项目名称即可,最后补充其他配置
# set_target_properties(CmakeIntro PROPERTIES CXX_STANDARD 23 CMAKE_CXX_STANDARD_REQUIRED ON CMAKE_CXX_EXTENSIONS OFF)

# 输出一些提示信息
# message(STATUS "Building ${CMAKE_PROJECT_NAME} version ${CMAKE_PROJECT_VERSION} project path ${CMAKE_CURRENT_SOURCE_DIR} -- ${CMAKE_HOME_DIRECTORY}")
# message(STATUS "${cmake_intro_VERSION_MAJOR}") # 输出版本号的主版本号
# message(STATUS "${cmake_intro_VERSION_MAJOR}.${cmake_intro_VERSION_MINOR}") # 输出版本号的主版本号和次版本号
# message(STATUS "${cmake_intro_VERSION_MAJOR}.${cmake_intro_VERSION_MINOR}.${cmake_intro_VERSION_PATCH}") # 输出版本号的主版本号、次版本号和补丁版本号

# 常见的几种路径
# CMAKE_SOURCE_DIR:你第一次调用 cmake 命令时所在的源代码根目录,通常是顶层CMakeLists.txt所在的目录.
# CMAKE_BINARY_DIR:构建过程产生的中间文件和可执行文件所在的目录,通常是 build 目录.
# CMAKE_CURRENT_SOURCE_DIR:当前处理的CMakeLists.txt文件所在的目录.
# CMAKE_CURRENT_BINARY_DIR:当前处理的CMakeLists.txt文件产生的中间文件和可执行文件所在的目录,通常是相应 build 目录.
# CMAKE_HOME_DIRECTORY:现在C++项目中不推荐使用,这是CMAKE的遗留问题,等价于 CMAKE_SOURCE_DIR.
# PROJECT_SOURCE_DIR: 如果在CMAKE的子目录中,调用了project()命令,则PROJECT_SOURCE_DIR等价于当前CMakeLists.txt文件所在的目录.
#                    如果在顶层CMakeLists.txt中,则PROJECT_SOURCE_DIR等价于CMAKE_SOURCE_DIR.在子目录中调用project()命令,则PROJECT_SOURCE_DIR等价于CMAKE_CURRENT_SOURCE_DIR.
#                    换言之,PROJECT_SOURCE_DIR取决于你在哪里调用了project()命令.