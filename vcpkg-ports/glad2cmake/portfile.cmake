vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO FelixHommel/Glad2-CMake
    REF v1.1.7
    SHA512 197482d85da5a5b93cd5e3ea27600fefad86a5eb63359c6fbe12ea67d7fb9b39c3a44c79543478718e6505cf3a16f41dae530cb971470c674cfa4e0c35daeb15
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
