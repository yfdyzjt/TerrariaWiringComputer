set_project("terraria_wiring_computer")
set_version("1.0.0")

add_rules("mode.debug", "mode.release")

toolchain("riscv-none-elf")
    set_kind("standalone")
    set_sdkdir("/xpacks/@xpack-dev-tools/riscv-none-elf-gcc/.content")
    set_bindir("/xpacks/@xpack-dev-tools/riscv-none-elf-gcc/.content/bin")
toolchain_end()

set_toolchains("riscv-none-elf")
set_plat("cross")
set_arch("riscv")

option("world")
    set_default("terraria_computer_large")
    set_description("Name of the world")
option_end()

option("cpu")
    set_default("cpu_rv32imc")
    set_description("Name of the CPU")
option_end()

option("driver")
    set_default("6.96K")
    set_description("Value of the drive frequency")
option_end()

option("ram")
    set_default("4K")
    set_description("Size of the data RAM")
option_end()

option("software")
    set_default("helloworld")
    set_description("Name of the software")
option_end()

option("printf_float")
    set_default(false)
    set_description("Enable floating point printf support")
option_end()

option("scanf_float")
    set_default(false)
    set_description("Enable floating point scanf support")
option_end()

target("system")
    add_options("world")
    add_options("cpu")
    add_options("driver")
    add_options("ram")
    add_options("software")
    add_options("printf_float")
    add_options("scanf_float")

    local config = {}

    config.world = get_config("world")
    config.cpu = get_config("cpu")
    config.driver = get_config("driver")
    config.ram = get_config("ram")
    config.software = get_config("software")
    config.printf_float = get_config("printf_float")
    config.scanf_float = get_config("scanf_float")

    if config.world and config.software then
        local parts = {}

        set_kind("binary") 
        set_targetdir("system")
        set_filename(config.software .. ".elf")
        set_strip("none")

        add_includedirs("hardware/include")
        add_includedirs("software/include")
        add_files("hardware/lib/**.c")
        add_files("software/lib/**.c")

        add_files("hardware/entry/start.s")
        add_files("hardware/entry/reset.c")

        add_includedirs("software/src/" .. config.software)  
        add_files("software/src/" .. config.software .. "/**.c")

        add_ldflags("-Wl,-T,./system/" .. config.software .. "_link.ld")
        if config.printf_float then add_ldflags("-Wl,-u,_printf_float") end
        if config.scanf_float then add_ldflags("-Wl,-u,_scanf_float") end

        before_build(
            function (target)
                if not os.isdir("./system") then
                    os.mkdir("./system")
                end
            end
        )

        before_link(
            function (target)
                import("hardware.module.targetsize")
                import("hardware.module.part")
                import("hardware.module.linkscript")
                import("hardware.module.combiepart")

                target_size = targetsize(target)
                text_parts, rodata_parts, data_parts, io_parts, driver_parts = part(target, target_size, config)
                parts, mem_parts = combiepart(text_parts, rodata_parts, data_parts, io_parts, driver_parts, config)
                link_script = linkscript(mem_parts)

                io.writefile("./system/" .. config.software .. "_link.ld", link_script)
                io.save("./system/" .. config.software .. "_part.lua", parts)
            end
        )
    
        after_build(
            function (target)
                local target_file = target:targetfile()
                local final_file = target_file:gsub("%.elf$", ".bin")
                
                os.execv("riscv-none-elf-objcopy", {"-O", "binary", target_file, final_file})
                -- os.execv("riscv-none-elf-size", {"-Ad", target_file})

                local result = os.iorunv("riscv-none-elf-size", {"-Ad", target_file})
                if result then
                    result = result:gsub("(%s+)(%d+)(%s+)(%d+)", function(space1, size, space2, addr)
                        return space1 .. size .. string.format("%9s", string.format("0x%x", tonumber(addr)))
                    end)
                    print(result)
                end
                
                os.exec("tmake do \"Include(self,\\\"./hardware/module/make.lua\\\").MakeWorld(\\\"" .. config.world .. "\\\",\\\"" .. config.software .. "\\\")\"")
            end
        )
        
        after_clean(
            function(target)
                os.rm("build")
                os.rm("system")
            end
        )
    end

    add_cflags(
        "-Os",
        "-march=rv32imc",
        "-mabi=ilp32",
        --"-nostdlib",
        "-Wall",
        "-fdata-sections",
        "-ffunction-sections",
        "-fomit-frame-pointer",
        "-fno-stack-protector",
        "-finline-functions",
        --"-flto",
        "-fno-exceptions",
        {force = true}
    )

    add_asflags(
        "-Os",
        "-march=rv32imc",
        "-mabi=ilp32",
        --"-nostdlib",
        "-Wall",
        "-fdata-sections", 
        "-ffunction-sections",
        "-fomit-frame-pointer",
        "-fno-stack-protector",
        "-finline-functions",
        --"-flto",
        {force = true}
    )

    add_ldflags(
        "-Os",
        "-static",
        "-march=rv32imc",
        "-mabi=ilp32",
        --"-nostdlib",              -- no stdlib
        "-Wl,--gc-sections",
        --"-Wl,--strip-all",        
        "--specs=nano.specs",       -- newlib-nano
        "--specs=nosys.specs",      -- no system call
        --"-flto",
        {force = true}
    )
target_end()
