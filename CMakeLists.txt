cmake_minimum_required(VERSION 3.15)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Path to vcpkg toolchain")
endif()

project(sget VERSION 2.0.1a LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(nlohmann_json REQUIRED)

set(SOURCES
    src/main.cpp
    src/network.cpp
    src/parser.cpp
    src/download.cpp
    src/progress.cpp
)

add_executable(sget ${SOURCES})

target_link_libraries(sget PRIVATE CURL::libcurl LibXml2::LibXml2 nlohmann_json::nlohmann_json)

# Installed to /usr/bin/sget on Linux, staged by the ZIP generator on Windows
install(TARGETS sget DESTINATION bin)

# =============================================================================
# CPack packaging (DEB / RPM / ZIP)
# =============================================================================
set(CPACK_PACKAGE_NAME "sget")
set(CPACK_PACKAGE_VENDOR "CoderSilicon")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Advanced high-performance web data extractor")
set(CPACK_PACKAGE_CONTACT "cs@codersilicon.com")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/CoderSilicon/sget")
set(CPACK_PACKAGE_EXECUTABLES "sget" "sget")

# Deterministic package file names:
#   sget_2.0.0_amd64.deb   /   sget-2.0.0-1.x86_64.rpm   /   sget.zip
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_RPM_FILE_NAME RPM-DEFAULT)

# Ship a stripped binary inside the packages
set(CPACK_STRIP_FILES TRUE)

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;RPM")

    # --- Debian / Ubuntu (.deb) ---
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "CoderSilicon <cs@codersilicon.com>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${CPACK_PACKAGE_HOMEPAGE_URL}")
    # Auto-detect runtime deps (libcurl4, libxml2, libstdc++6, ...) from the binary.
    # Requires dpkg-dev (dpkg-shlibdeps) on the build host.
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

    # --- Red Hat / Fedora (.rpm) ---
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
    # Auto-detect Runtime requirements (libcurl.so.4, libxml2.so.2, ...) via ldd.
    # Requires rpmbuild on the build host.
    set(CPACK_RPM_PACKAGE_AUTOREQ ON)

elseif(WIN32)
    # Windows: fresh ZIP archive of the binary instead of an installer
    set(CPACK_GENERATOR "ZIP")
endif()

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