set_project("terraria_wiring_computer")
set_version("1.0.0")

add_rules("mode.debug", "mode.release")

local sdk_dir = "../xpacks/@xpack-dev-tools/riscv-none-elf-gcc/.content"
local bin_dir = sdk_dir .. "/bin/"

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

    local world = get_config("world")
    local cpu = get_config("cpu")
    local driver = get_config("driver")
    local ram = get_config("ram")
    local software = get_config("software")

    if world and cpu and software then
        local parts = {}

        set_kind("binary") 

        set_targetdir("system")
        set_filename(software .. ".elf")

        add_includedirs("hardware/include")
        add_includedirs("software/include")
        add_files("hardware/entry/start.s")
        add_files("hardware/entry/reset.c")
        add_files("hardware/lib/*.c")
        -- add_files("software/lib/*.c")

        add_includedirs("software/src/" .. software)  
        add_files("software/src/" .. software .. "/**.c")

        add_ldflags("-Wl,-T,./system/" .. software .. "_link.ld")

        before_link(
            function (target)
                import("hardware.module.part")
                import("hardware.module.linkscript")

                local link_parts = {}

                parts, link_parts = part(target:sourcefiles(), target:objectfiles(), target:get("includedirs"), bin_dir, cpu, driver, ram)
                
                io.writefile("./system/" .. software .. "_link.ld", linkscript(link_parts))
                io.save("./system/" .. software .. "_part.lua", parts)
            end
        )
    
        after_build(
            function (target)
                local target_file = target:targetfile()
                local final_file = target_file:gsub("%.elf$", ".bin")
                os.execv(bin_dir .. "riscv-none-elf-objcopy", {"-O", "binary", target_file, final_file})
                os.execv(bin_dir .. "riscv-none-elf-size", {"-Ax", target_file})
                os.exec("tmake do \"Include(self,\\\"./hardware/module/make.lua\\\").MakeWorld(\\\"" .. world .. "\\\",\\\"" .. software .. "\\\")\"")
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
