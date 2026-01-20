vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO FelixHommel/Glad2-CMake
    REF v1.1.5
    SHA512 1b211a56e1ed27328862cf785057d5efc2bcd1f1f37d13d1064d290583e836578907c898765a9c8aca5044b546cad320465eafe066ba85086c5e5ecd18dded36
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME glad2cmake
    CONFIG_PATH lib/cmake/glad2cmake
)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
