vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO FelixHommel/Glad2-CMake
    REF v1.1.6
    SHA512 8185f27830e715fda634ab64af3a274e1e39657159c11ed7f0bbb609f991830e8b06cbb9d61102e7007d38c2ed13b50b2f6911d584ed190a226b4ada9176d776
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
