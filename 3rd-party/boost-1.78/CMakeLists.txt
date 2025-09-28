cmake_minimum_required(VERSION 3.20)
project(BoostFetchAndBuild NONE)

include(ExternalProject)

# Target installation directory
set(BOOST_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Cache directory for downloaded Boost source
set(BOOST_CACHE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../.cache/boost-1.78-src)

# Boost version
set(BOOST_VERSION 1.78.0)
set(BOOST_VERSION_U 1_78_0)

# Build processor count
set(PROCESSOR_COUNT 4)

# Determine download URL, bootstrap command, and hash based on OS
if(WIN32)
    set(BOOST_URL https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_U}.zip)
    set(BOOST_BOOTSTRAP bootstrap.bat)
    set(BOOST_B2 b2.exe)
    set(BOOST_HASH f22143b5528e081123c3c5ed437e92f648fe69748e95fa6e2bd41484e2986cc3)
    set(BOOST_TARBALL ${CMAKE_CURRENT_SOURCE_DIR}/../.cache/boost_${BOOST_VERSION_U}.zip)
else()
    set(BOOST_URL https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_U}.tar.gz)
    set(BOOST_BOOTSTRAP ./bootstrap.sh)
    set(BOOST_B2 ./b2)
    set(BOOST_HASH 94ced8b72956591c4775ae2207a9763d3600b30d9d7446562c552f0a14a63be7)
    set(BOOST_TARBALL ${CMAKE_CURRENT_SOURCE_DIR}/../.cache/boost_${BOOST_VERSION_U}.tar.gz)
endif()

# Try to find system or existing Boost first
find_package(Boost 1.78.0 QUIET)

if(NOT Boost_FOUND)
    message(STATUS "Boost not found, will download and build it")

    # Check if tarball already exists and hash matches
    set(BOOST_TARBALL_OK FALSE)
    if(EXISTS ${BOOST_TARBALL})
        file(SHA256 ${BOOST_TARBALL} BOOST_TARBALL_HASH)
        if(BOOST_TARBALL_HASH STREQUAL ${BOOST_HASH})
            message(STATUS "Found existing Boost tarball with correct hash, will reuse it")
            set(BOOST_TARBALL_OK TRUE)
        else()
            message(WARNING "Boost tarball hash mismatch.")
            message(STATUS "Removing existing Boost cache directory.")
            file(REMOVE_RECURSE ${BOOST_CACHE_DIR})
        endif()
    else()
        message(STATUS "Boost tarball not exists: ${BOOST_TARBALL}.")
        message(STATUS "Removing existing Boost cache directory.")
        file(REMOVE_RECURSE ${BOOST_CACHE_DIR})
    endif()

    # If tarball OK, use it as URL
    if(BOOST_TARBALL_OK)
        set(BOOST_URL ${BOOST_TARBALL})
    endif()

    # Use ExternalProject to download, build, and install Boost
    ExternalProject_Add(boost_external
        URL                         ${BOOST_URL}
        URL_HASH                    SHA256=${BOOST_HASH}
        PREFIX                      ${BOOST_CACHE_DIR}
        CONFIGURE_COMMAND           ""  # No separate configure step
        BUILD_COMMAND
            ${BOOST_CACHE_DIR}/src/boost_external/${BOOST_BOOTSTRAP}
            && ${BOOST_CACHE_DIR}/src/boost_external/${BOOST_B2} -j${PROCESSOR_COUNT} --build-type=complete --with=all --prefix=${BOOST_INSTALL_DIR} install
        INSTALL_DIR                 ${BOOST_INSTALL_DIR}
        BUILD_BYPRODUCTS            ${BOOST_INSTALL_DIR}/lib
        DOWNLOAD_EXTRACT_TIMESTAMP  TRUE
        LOG_DOWNLOAD                ON
        LOG_CONFIGURE               ON
        LOG_BUILD                   ON
        LOG_INSTALL                 ON
        LOG_OUTPUT_ON_FAILURE       ON
    )

    # Remove temporary cache directory after installation
    ExternalProject_Add_Step(boost_external
        remove_cache
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${BOOST_CACHE_DIR}
        COMMENT "Cleaning Boost source cache"
        DEPENDEES install
    )

else()
    message(STATUS "Boost found, no need to download/build")
endif()

if(TARGET third_party)
    message(STATUS "Package boost_external to third_party.")
    add_dependencies(third_party boost_external)
endif()
