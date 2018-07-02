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
find_package(WEBSOCKETPP 0.6.0 REQUIRED)
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
)

target_include_directories(CEFBaseAppUtils PUBLIC
    $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
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
    libBaseApp/CefBaseLifeSpanHandler.cpp
    libBaseApp/CefBaseLoadHandler.cpp
    libBaseApp/CefBaseMainLoop.cpp
    libBaseApp/CefBaseRendererProcess.cpp
    libBaseApp/CefBaseRenderHandler.cpp
    libBaseApp/CefBaseRequestHandler.cpp
    libBaseApp/CefBaseRequestReplies.cpp
)
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
target_include_directories(CEFBaseAppCore PUBLIC
    $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_property(TARGET CEFBaseAppCore PROPERTY PUBLIC_HEADER
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseApp.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseBrowserProcess.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseClient.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseDisplayHandler.h
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
target_include_directories(CEFBaseAppDefaults PUBLIC
    $<BUILD_INTERFACE:${CEFBaseApp_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_property(TARGET CEFBaseAppDefaults PROPERTY PUBLIC_HEADER
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseDefaults.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseDisplayGTKDefaultHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseLifeSpanDefaultHandler.h
    ${CEFBaseApp_SOURCE_DIR}/include/CefBaseLoadDefaultHandler.h
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
