set_project("terraria_wiring_computer")
set_version("1.0.0")

add_rules("mode.debug", "mode.release")

local sdk_dir = "../xpacks/@xpack-dev-tools/riscv-none-elf-gcc/.content"
local bin_dir = sdk_dir .. "/bin/"
local sdk_program = bin_dir .. "riscv-none-elf-"

toolchain("riscv-none-elf")
    set_kind("standalone")
    set_sdkdir(sdk_dir)
    set_bindir(bin_dir)
toolchain_end()

set_toolchains("riscv-none-elf")
set_plat("cross")
set_arch("riscv")

option("world")
    set_default("terraria_computer_large")
option_end()

option("cpu")
    set_default("cpu_rv32imc")
option_end()

option("driver")
    set_default("driver_portal_gun_station_x20_6.96kHz")
option_end()

option("ram")
    set_default("4K")
option_end()

option("software")
    set_default("test")
option_end()

target("system")
    add_options("world")
    add_options("cpu")
    add_options("driver")
    add_options("ram")
    add_options("software")

    local config = {}

    config.world = get_config("world")
    config.cpu = get_config("cpu")
    config.driver = get_config("driver")
    config.ram = get_config("ram")
    config.software = get_config("software")

    if config.world and config.software then
        local parts = {}

        set_kind("binary") 

        set_targetdir("system")
        set_filename(config.software .. ".elf")

        add_includedirs("hardware/include")
        add_includedirs("software/include")
        add_files("hardware/lib/**.c")
        -- add_files("software/lib/**.c")

        add_files("hardware/entry/start.s")
        add_files("hardware/entry/reset.c")

        add_includedirs("software/src/" .. config.software)  
        add_files("software/src/" .. config.software .. "/**.c")

        add_ldflags("-Wl,-T,./system/" .. config.software .. "_link.ld")

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

                target_size = targetsize(target, sdk_program)
                text_parts, rodata_parts, data_parts, io_parts = part(target, target_size, config, sdk_program)
                parts, mem_parts = combiepart(text_parts, rodata_parts, data_parts, io_parts, config)
                link_script = linkscript(mem_parts)

                io.writefile("./system/" .. config.software .. "_link.ld", link_script)
                io.save("./system/" .. config.software .. "_part.lua", parts)
            end
        )
    
        after_build(
            function (target)
                local target_file = target:targetfile()
                local final_file = target_file:gsub("%.elf$", ".bin")
                
                os.execv(sdk_program .. "objcopy", {"-O", "binary", target_file, final_file})
                os.execv(sdk_program .. "size", {"-Ax", target_file})
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
        --"-nostdlib",
        "-Wl,--gc-sections",
        "-Wl,--strip-all",
        "--specs=nano.specs",       -- newlib-nano
        "--specs=nosys.specs",      -- system call
        --"-flto",
        {force = true}
    )
target_end()
