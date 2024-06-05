set_project("demo")
set_version("1.0.0")

add_rules("mode.debug", "mode.release")

toolchain("riscv-none-elf")
    set_kind("standalone")
    set_sdkdir("")
toolchain_end()

target("final")
    set_kind("binary") 
    set_toolchains("riscv-none-elf")  

    set_plat("cross")
    set_arch("riscv")
    set_filename("final.elf")
    
    add_files("start.s", "reset.c");

    add_files("src/**.c")
    add_includedirs(os.dirs(path.join(os.scriptdir(), "src/**")))

    add_cflags(
        "-Og",
        "-march=rv32imc",
        "-mabi=ilp32",
        "-nostdlib",
        "-Wall",
        "-fdata-sections",
        "-ffunction-sections",
        {force = true}
    )

    add_asflags(
        "-Og",
        "-march=rv32imc",
        "-mabi=ilp32",
        "-nostdlib",
        "-Wall",
        "-fdata-sections", 
        "-ffunction-sections",
        {force = true}
    )

    add_ldflags(
        "-Og",
        "-march=rv32imc",
        "-mabi=ilp32",
        "-nostdlib",
        "-Wl,-T,link.ld",
        "-Wl,--gc-sections",
        "--specs=nosys.specs",
        {force = true}
    )

    after_build(
        function(target)
            os.exec("riscv-none-elf-objcopy -O binary ./build/cross/riscv/release/final.elf ./build/final.bin")
            os.exec("riscv-none-elf-size -Ax ./build/cross/riscv/release/final.elf")
            -- os.run("riscv-none-elf-objdump -D -b binary ./build/final.bin -mriscv > ./final.s")
            os.run("./wirewrite computer.wld ./build/final.bin")
        end)
target_end()
