# 最低支持的CMake版本
cmake_minimum_required(VERSION 3.18.1)


project("msvoicechange")

# 导入头文件
include_directories("inc")

file(GLOB allCPP *.c *.h *.cpp)

add_library(
        #库名字
        msvoicechange

        # 库类型 动态还是静态
        SHARED

        # 添加的源文件
#        native-lib.cpp

        ${allCPP}
)


#设置库文件路径
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")



#定义一个变量相当于是一个缓存,会寻找一次，也可以直接引用，如果需要引用多次，就需要定义一个常量
#不用每次都查找了，提高侠侣
find_library(
        #变量名称
        log-lib
        #库名   liblog.so   找的是这个库 D:\tools\Android\Sdk\ndk\21.4.7075529\platforms\android-24\arch-arm64\usr\lib\liblog.so
        log)


target_link_libraries( # Specifies the target library.
        msvoicechange
        fmod  #需要将so 链接到msvoicechange 里边  注意这里的名称前面的lib默认会加这里不能写 so也是一样
        fmodL
        ${log-lib})