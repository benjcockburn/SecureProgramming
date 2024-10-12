cmake_minimum_required(VERSION 3.16)

project(SecureChat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Find and link the Threads library
find_package(Threads REQUIRED)

# Find and link OpenSSL
find_package(OpenSSL REQUIRED)

# Check if OpenSSL was found and include directories


set(PROJECT_SOURCES
        main.cpp
        securechatmainwindow.cpp
        securechatmainwindow.h
        securechatmainwindow.ui
        client.h
        client.cpp
        messagehandler.cpp
        messagehandler.h
        decrypt_RSA.h
        decrypt_RSA.cpp
        encrypt_AES.h
        encrypt_AES.cpp
        JsonHandler.cpp
        JsonHandler.h
        ecodeBase_64.h
        ecodeBase_64.cpp
        decrypt_AES.h
        decrypt_AES.cpp
        generate_Keys.h
        generate_Keys.cpp
        publicKeyStringConversion.h
        publicKeyStringConversion.cpp
        json.hpp
        controller.cpp
        controller.h
        recipient.h
        recipient.cpp
        RSA_generate_Keys.h
        RSA_generate_Keys.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SecureChat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        messagehandler.h messagehandler.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SecureChat APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SecureChat SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SecureChat
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link the Threads and OpenSSL libraries along with the Qt Widgets
target_link_libraries(SecureChat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Threads::Threads OpenSSL::SSL OpenSSL::Crypto)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SecureChat)
endif()
set_target_properties(SecureChat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SecureChat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SecureChat)
endif()
