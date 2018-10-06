#
# Project Definition
#
cmake_minimum_required(VERSION 3.8.2)
project(CEFBaseApp)

#
# Find dependencies
#
find_package(ThreadComms 0.1 REQUIRED)
find_package(FixedJSON 0.1 REQUIRED)
find_package(DevToolsLog 0.1 REQUIRED)
find_package(OSDevTools 0.1 REQUIRED)
find_package(UtilTime 0.1 REQUIRED)
find_package(GTKUtils 0.1 REQUIRED)
find_package(Boost 0.1 REQUIRED COMPONENTS system thread)
find_package(WEBSOCKETPP 0.8.0 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CEF REQUIRED)


add_library(CEFBaseAppUtils STATIC
    include/CEFDOMSnapshot.h
    include/CEFJSLogging.h
    include/CEFJSObject.h
    include/CEFJSUtils.h
    libUtils/CEFDOMSnapshot.cpp
    libUtils/CEFJSLogging.cpp
    libUtils/CEFJSObject.cpp
    libUtils/CEFJSUtils.cpp
)

target_link_libraries(CEFBaseAppUtils PUBLIC
    FixedJSON::FixedJSON
    ThreadComms::ThreadComms
    DevToolsLog::Log
    OSDevTools::OSDevTools
    Boost::system
    Boost::thread
    Boost::boost
    OpenSSL::SSL
    OpenSSL::Crypto
    ${CEF_BINARY_DIR_RELEASE}/libcef_dll_wrapper.a
)

target_include_directories(CEFBaseAppUtils
    PUBLIC
        $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CEF_INCLUDE_PATH}
)

set_property(TARGET CEFBaseAppUtils PROPERTY PUBLIC_HEADER
    ${CEFBaseApp_SOURCE_DIR}/include/CEFDOMSnapshot.h
    ${CEFBaseApp_SOURCE_DIR}/include/CEFJSLogging.h
    ${CEFBaseApp_SOURCE_DIR}/include/CEFJSObject.h
    ${CEFBaseApp_SOURCE_DIR}/include/CEFJSUtils.h
)

add_library(CEFBaseAppCore STATIC
    include/CefBaseApp.h
    include/CefBaseBrowserProcess.h
    include/CefBaseClient.h
    include/CefBaseDisplayHandler.h
    include/CefBaseIPCExec.h
    include/CefBaseIPCVia.h
    include/CefBaseJSHandler.h
    include/CefBaseJSMessageRouter.h
    include/CefBaseLifeSpanHandler.h
    include/CefBaseLoadHandler.h
    include/CefBaseMainLoop.h
    include/CefBaseMultiHandler.h
    include/CefBaseRendererProcess.h
    include/CefBaseRenderHandler.h
    include/CefBaseRequestHandler.h
    include/CefBaseRequestReplies.h
    include/CefBaseThread.h
    libBaseApp/CefBaseApp.cpp
    libBaseApp/CefBaseBrowserProcess.cpp
    libBaseApp/CefBaseClient.cpp
    libBaseApp/CefBaseDisplayHandler.cpp
    libBaseApp/CefBaseJSHandler.cpp
    libBaseApp/CefBaseJSMessageRouter.cpp
    libBaseApp/CefBaseIPCExec.cpp
    libBaseApp/CefBaseIPCVia.cpp
    libBaseApp/CefBaseLifeSpanHandler.cpp
    libBaseApp/CefBaseLoadHandler.cpp
    libBaseApp/CefBaseMainLoop.cpp
    libBaseApp/CefBaseRendererProcess.cpp
    libBaseApp/CefBaseRenderHandler.cpp
    libBaseApp/CefBaseRequestHandler.cpp
    libBaseApp/CefBaseRequestReplies.cpp)

target_link_libraries(CEFBaseAppCore PUBLIC
    FixedJSON::FixedJSON
    ThreadComms::ThreadComms
    DevToolsLog::Log
    OSDevTools::OSDevTools
    Boost::system
    Boost::thread
    Boost::boost
    OpenSSL::SSL
    OpenSSL::Crypto
    CEFBaseAppUtils
)
target_include_directories(CEFBaseAppCore
    PUBLIC
        $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CEF_INCLUDE_PATH}
)

set_property(TARGET CEFBaseAppCore PROPERTY PUBLIC_HEADER
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseApp.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseBrowserProcess.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseClient.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseDisplayHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseIPCExec.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseIPCVia.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseJSHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseJSMessageRouter.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseLifeSpanHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseLoadHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseMainLoop.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseMultiHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseRendererProcess.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseRenderHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseRequestHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseRequestReplies.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseThread.h
)

add_library(CEFBaseAppDefaults STATIC
        include/CefBaseDefaults.h
        include/CefBaseDisplayGTKDefaultHandler.h
        include/CefBaseLifeSpanDefaultHandler.h
        include/CefBaseLoadDefaultHandler.h
        libBaseDefaults/CefBaseDefaults.cpp
        libBaseDefaults/CefBaseDisplayGTKDefaultHandler.cpp
        libBaseDefaults/CefBaseLifeSpanDefaultHandler.cpp
        libBaseDefaults/CefBaseLoadDefaultHandler.cpp
)
target_link_libraries(CEFBaseAppDefaults PUBLIC
    CEFBaseAppCore
    GTKUtils::GTKUtil
)
target_include_directories(CEFBaseAppDefaults
    PUBLIC
        $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CEF_INCLUDE_PATH}
)

set_property(TARGET CEFBaseAppDefaults PROPERTY PUBLIC_HEADER
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseDefaults.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseDisplayGTKDefaultHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseLifeSpanDefaultHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseLoadDefaultHandler.h
)

add_library(CefTestApp STATIC
        include/CefTestApp.h
        include/CefTestAppHandlers.h
        include/CefTestClient.h
        include/CefTestJSHandler.h
        include/CefTests.h

        libTest/CefTestApp.cpp
        libTest/CefTestAppHandlers.cpp
        libTest/CefTestClient.cpp
        libTest/CefTestJSHandler.cpp
        libTest/CefTests.cpp
)

target_link_libraries(CefTestApp PUBLIC CEFBaseAppDefaults UtilTime::Time)

target_include_directories(CefTestApp
        PUBLIC
        $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CEF_INCLUDE_PATH}
        )

set_property(TARGET CEFBaseAppDefaults PROPERTY PUBLIC_HEADER
        ${CEFBaseApp_SOURCE_DIR}/include/CefBaseDefaults.h
        ${CEFBaseApp_SOURCE_DIR}/include/CefBaseDisplayGTKDefaultHandler.h
        ${CEFBaseApp_SOURCE_DIR}/include/CefBaseLifeSpanDefaultHandler.h
        ${CEFBaseApp_SOURCE_DIR}/include/CefBaseLoadDefaultHandler.h
        )

#
# Test Configuration
#
find_package(GTest REQUIRED)

add_executable(cookie test/cookie.cpp)
COPY_FILES(cookie "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR_RELEASE}" ${CEFBaseApp_BINARY_DIR})
COPY_FILES(cookie "${CEF_RESOURCE_FILES}" "${CEF_BINARY_DIR_RELEASE}" ${CEFBaseApp_BINARY_DIR})
COPY_FILES(cookie "index.html" ${CEFBaseApp_SOURCE_DIR}/test  ${CEFBaseApp_BINARY_DIR})

add_executable(threads test/threads.cpp)
add_executable(ipc test/ipc.cpp)

enable_testing()
add_test(cookie cookie)
add_test(threads threads)
add_test(ipc ipc)

target_link_libraries(cookie
    PUBLIC
        CefTestApp
        ${CEF_BINARY_DIR_RELEASE}/libcef_dll_wrapper.a
        ${CEF_LIB_RELEASE}
        ${CEF_STANDARD_LIBS}
        GTest::GTest
        stdc++fs
)

target_include_directories(cookie
        PUBLIC
        $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CEF_INCLUDE_PATH}
    )

target_link_libraries(threads
        PUBLIC
        CefTestApp
        ${CEF_BINARY_DIR_RELEASE}/libcef_dll_wrapper.a
        ${CEF_LIB_RELEASE}
        ${CEF_STANDARD_LIBS}
        GTest::GTest
)

target_include_directories(threads
        PUBLIC
        $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CEF_INCLUDE_PATH}
)

target_link_libraries(ipc
        PUBLIC
        CefTestApp
        ${CEF_BINARY_DIR_RELEASE}/libcef_dll_wrapper.a
        ${CEF_LIB_RELEASE}
        ${CEF_STANDARD_LIBS}
        GTest::GTest
        )

target_include_directories(ipc
        PUBLIC
        $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CEF_INCLUDE_PATH}
        )

#
# Installation instructions
#
install(TARGETS       CEFBaseAppUtils CEFBaseAppCore CEFBaseAppDefaults
        EXPORT        CEFBaseAppTargets
        ARCHIVE       DESTINATION lib
        INCLUDES      DESTINATION include
        PUBLIC_HEADER DESTINATION include
)

install (EXPORT CEFBaseAppTargets
         FILE         CEFBaseAppTargets.cmake
         NAMESPACE    CEFBaseApp::
         DESTINATION  lib/cmake/CEFBaseApp
)

#
# Define our package version
#
include(CMakePackageConfigHelpers)
write_basic_package_version_file("CEFBaseAppConfigVersion.cmake"
    VERSION       0.1
    COMPATIBILITY SameMajorVersion
)
install (
    FILES       ${CEFBaseApp_BINARY_DIR}/CEFBaseAppConfigVersion.cmake
    DESTINATION lib/cmake/CEFBaseApp
)

#
# Finally export the Config which CMake will use
# when looking for matching libraries to link against.
#
install (
    FILES       CEFBaseAppConfig.cmake
    DESTINATION lib/cmake/CEFBaseApp
)

#
# Configure Coverage Build
#
SET(CMAKE_CXX_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE)
