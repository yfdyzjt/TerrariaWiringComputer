set_project("terraria_computer")
set_version("0.0.1")

add_rules("mode.debug", "mode.release")

toolchain("riscv-none-elf")
    set_kind("standalone")
    set_sdkdir("./xpacks/@xpack-dev-tools/riscv-none-elf-gcc/.content")
toolchain_end()

set_toolchains("riscv-none-elf")
set_plat("cross")
set_arch("riscv")

set_targetdir("system/bin")

add_files("system/src/start.s", "system/src/reset.c");

add_cflags(
    "-Os",
    "-march=rv32imc",
    "-mabi=ilp32",
    "-nostdlib",
    "-Wall",
    "-fdata-sections",
    "-ffunction-sections",
    "-fomit-frame-pointer",
    "-fno-stack-protector",
    "-finline-functions",
    "-flto",
    "-fno-exceptions",
    {force = true}
)

add_asflags(
    "-Os",
    "-march=rv32imc",
    "-mabi=ilp32",
    "-nostdlib",
    "-Wall",
    "-fdata-sections", 
    "-ffunction-sections",
    "-fomit-frame-pointer",
    "-fno-stack-protector",
    "-finline-functions",
    "-flto",
    {force = true}
)

add_ldflags(
    "-Os",
    "-static",
    "-march=rv32imc",
    "-mabi=ilp32",
    "-nostdlib",
    "-Wl,--gc-sections",
    "-Wl,--strip-all",
    "--specs=nosys.specs",
    "-flto",
    {force = true}
)

includes(
    "software/test"
)

after_clean(
    function(target)
        os.rm("build")
        os.rm("system/bin")
    end
)
