include(FetchContent)
include(ExternalProject)

ExternalProject_Add(catch_download
    PREFIX              ${FETCHCONTENT_BASE_DIR}/catch
    URL                 https://github.com/philsquared/Catch/releases/download/v1.9.6/catch.hpp
    DOWNLOAD_NO_EXTRACT 1
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       ""
    INSTALL_COMMAND     ""
)

add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE ${FETCHCONTENT_BASE_DIR}/catch/src)
add_dependencies(Catch2 catch_download)
