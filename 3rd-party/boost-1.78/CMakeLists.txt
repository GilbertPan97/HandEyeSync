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

# Determine download URL and bootstrap command based on OS
if(WIN32)
    set(BOOST_URL https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_U}.zip)
    set(BOOST_BOOTSTRAP bootstrap.bat)
    set(BOOST_B2 b2.exe)
else()
    set(BOOST_URL https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_U}.tar.gz)
    set(BOOST_BOOTSTRAP ./bootstrap.sh)
    set(BOOST_B2 ./b2)
endif()

# Try to find system or existing Boost first
find_package(Boost 1.78.0 QUIET)

if(NOT Boost_FOUND)
    message(STATUS "Boost not found, will download and build it")

    # Use ExternalProject to download, build, and install Boost
    ExternalProject_Add(boost_external
        URL ${BOOST_URL}
        PREFIX ${BOOST_CACHE_DIR}
        SOURCE_DIR ${BOOST_CACHE_DIR}/src
        BINARY_DIR ${BOOST_CACHE_DIR}/build
        CONFIGURE_COMMAND ""  # No separate configure step
        BUILD_COMMAND
            ${BOOST_CACHE_DIR}/src/${BOOST_BOOTSTRAP}
            && ${BOOST_CACHE_DIR}/src/${BOOST_B2} -j${PROCESSOR_COUNT} --build-type=complete --with=all --prefix=${BOOST_INSTALL_DIR} install
        INSTALL_DIR ${BOOST_INSTALL_DIR}
        BUILD_BYPRODUCTS ${BOOST_INSTALL_DIR}/lib
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
        LOG_INSTALL ON
        LOG_OUTPUT_ON_FAILURE ON
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
