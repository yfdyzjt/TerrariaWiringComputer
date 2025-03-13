set_project("terraria_wiring_computer")
set_version("1.0.0", {build = "%Y%m%d"})

add_rules("mode.debug", "mode.release")

toolchain("riscv-none-elf")
    set_kind("standalone")
    set_sdkdir("/xpacks/@xpack-dev-tools/riscv-none-elf-gcc/.content")
    set_bindir("/xpacks/@xpack-dev-tools/riscv-none-elf-gcc/.content/bin")
toolchain_end()

set_toolchains("riscv-none-elf")
set_defaultplat("cross")
set_defaultarchs("riscv")
set_plat("cross")
set_arch("riscv")

option("s")
    set_default("helloworld")
    set_description("Name of the software build")
option_end()

target("system")
    add_options("s")

    local cur_config = {}
    cur_config.software = get_config("s")
    cur_config.world = "terraria_computer_large"
    cur_config.cpu = "cpu_rv32imc"
    cur_config.driver = "6.96K"
    cur_config.ram = "4K"
    cur_config.printf_float = false
    cur_config.scanf_float = false

    if cur_config.software then
        local sub_path = "software/src/" .. cur_config.software .. "/xmake.lua"
        if os.isfile(sub_path) then
            includes(sub_path)
            if config then
                if config.world ~= nil then cur_config.world = config.world end
                if config.cpu ~= nil then cur_config.cpu = config.cpu end
                if config.driver ~= nil then cur_config.driver = config.driver end
                if config.ram ~= nil then cur_config.ram = config.ram end
                if config.printf_float ~= nil then cur_config.printf_float = config.printf_float end
                if config.scanf_float ~= nil then cur_config.scanf_float = config.scanf_float end
            end
        end

        local parts = {}

        set_kind("binary") 
        set_targetdir("system")
        set_filename(cur_config.software .. ".elf")
        set_strip("none")

        add_includedirs("hardware/include")
        add_includedirs("software/include")
        add_files("hardware/lib/**.c")
        add_files("software/lib/**.c")

        add_files("hardware/entry/start.s")
        add_files("hardware/entry/reset.c")

        add_includedirs("software/src/" .. cur_config.software)  
        add_files("software/src/" .. cur_config.software .. "/**.c")

        add_ldflags("-Wl,-T,./system/" .. cur_config.software .. "_link.ld")
        if cur_config.printf_float then add_ldflags("-Wl,-u,_printf_float") end
        if cur_config.scanf_float then add_ldflags("-Wl,-u,_scanf_float") end

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
                text_parts, rodata_parts, data_parts, io_parts, driver_parts = part(target, target_size, cur_config)
                parts, mem_parts = combiepart(text_parts, rodata_parts, data_parts, io_parts, driver_parts, cur_config)
                link_script = linkscript(mem_parts)

                io.writefile("./system/" .. cur_config.software .. "_link.ld", link_script)
                io.save("./system/" .. cur_config.software .. "_part.lua", parts)
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
                
                os.exec("tmake do \"Include(self,\\\"./hardware/module/make.lua\\\").MakeWorld(\\\"" .. cur_config.world .. "\\\",\\\"" .. cur_config.software .. "\\\")\"")
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
