cmake_minimum_required(VERSION 3.15)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Set your project name to Trace
project(sget CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory for your headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find the libcurl library installed via vcpkg
find_package(CURL REQUIRED)

# Explicitly list your source files
set(SOURCES 
    src/main.cpp 
    src/network.cpp
    src/parser.cpp
)

# Create the executable target named 'trace'
add_executable(sget ${SOURCES})

# Link libcurl to your 'trace' executable
target_link_libraries(sget PRIVATE CURL::libcurl)

set(CPACK_PACKAGE_NAME "sget")
set(CPACK_PACKAGE_VENDOR "CoderSilicon")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Advanced high-performance web data extractor")
set(CPACK_PACKAGE_CONTACT "cs@codersilicon.com")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/CoderSilicon/sget")

# --- LINUX SPECIFIC CONFIGURATION ---
if(UNIX AND NOT APPLE)
    # Only generate Debian and RPM files when on a Linux build machine
    install(TARGETS sget DESTINATION bin)
    set(CPACK_GENERATOR "DEB;RPM")

    # Debian Package Controls
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "CoderSilicon")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libcurl4") # Ensures target system has native curl

    # RPM Package Controls
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
    set(CPACK_RPM_PACKAGE_REQUIRES "libcurl")

# --- WINDOWS SPECIFIC CONFIGURATION ---
elseif(WIN32)
    # When building on Windows, generate a clean ZIP archive of the binary instead
    set(CPACK_GENERATOR "ZIP")
    
    # If you ever decide to use a Windows installer framework like NSIS or WIX later,
    # you can swap "ZIP" out for "NSIS" right here without breaking Linux.
endif()

# Include the CPack engine module (Must be the absolute last line of your file)
include(CPack)

if(WIN32)
    set(WINDOWS_DIST_DIR "${CMAKE_SOURCE_DIR}/../dist/windows")

    add_custom_command(TARGET sget POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${WINDOWS_DIST_DIR}"
        
        COMMAND ${CMAKE_COMMAND} -E copy 
            "$<TARGET_FILE:sget>" 
            "${WINDOWS_DIST_DIR}/"

        COMMAND ${CMAKE_COMMAND} -E copy 
            "${CMAKE_SOURCE_DIR}/../scripts/install.bat" 
            "${WINDOWS_DIST_DIR}/"
        
        COMMAND ${CMAKE_COMMAND} -E copy 
            "$<TARGET_RUNTIME_DLLS:sget>" 
            "${WINDOWS_DIST_DIR}/"
            
        COMMENT "Staging production binary distribution pack inside dist/windows..."
    )
endif()