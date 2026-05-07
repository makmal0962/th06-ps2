set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR mips)

set(PS2DEV $ENV{PS2DEV})
set(PS2SDK $ENV{PS2SDK})

set(CMAKE_C_COMPILER   ${PS2DEV}/ee/bin/mips64r5900el-ps2-elf-gcc)
set(CMAKE_CXX_COMPILER ${PS2DEV}/ee/bin/mips64r5900el-ps2-elf-g++)
set(CMAKE_AR           ${PS2DEV}/ee/bin/mips64r5900el-ps2-elf-ar)
set(CMAKE_RANLIB       ${PS2DEV}/ee/bin/mips64r5900el-ps2-elf-ranlib)

set(CMAKE_FIND_ROOT_PATH
    ${PS2DEV}/ee/mips64r5900el-ps2-elf
    ${PS2SDK}
    ${PS2SDK}/ports
    /usr/local/ps2dev/gsKit
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_EXE_LINKER_FLAGS_INIT "-T${PS2SDK}/ee/startup/linkfile")

add_compile_options(-G0 -mno-gpopt)
add_compile_definitions(
    PS2
    _EE
    _MIPSEL
    __PS2__
)