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

option("soft")
    set_default("test")
option_end()

option("hard")
    set_default("mini")
option_end()

target("system")
    add_options("soft", "hard")
    local software = get_config("soft")
    local hardware = get_config("hard")

    if software and hardware then
        set_kind("binary") 

        set_targetdir("system")
        set_filename(software .. ".elf")

        add_files("hardware/entry/start.s")
        add_files("hardware/entry/reset.c")

        add_files("software/" .. software .. "/src/**.c")
        add_includedirs("software/" .. software .. "/include")

        add_ldflags("-Wl,-T,./system/" .. hardware .. "_link.ld")

        includes("hardware/module/" .. hardware .. ".lua")
        for _, part in ipairs(Parts) do
            if part.Type ~= "cpu" and part.Type ~= "memory_ins" and part.Type ~= "driver" then
                add_defines(
                    "_" .. part.Name .. "_origin=" .. part.Origin,
                    "_" .. part.Name .. "_length=" .. part.Length
                )
            end
        end

        before_link(
            function (target)
                os.exec("tmake do \"Include(self,\\\"tmake.lua\\\").MakeLinkScript(\\\"" .. hardware .. "\\\")\"")
            end
        )
    
        after_build(
            function (target)
                local sdkdir = "xpacks/@xpack-dev-tools/riscv-none-elf-gcc/.content/bin/"
                os.exec(sdkdir .. "riscv-none-elf-objcopy -O binary ./system/" .. software .. ".elf ./system/" .. software .. ".bin")
                os.exec(sdkdir .. "riscv-none-elf-size -Ax ./system/" .. software .. ".elf")
                os.exec("tmake do \"Include(self,\\\"tmake.lua\\\").MakeWorld(\\\"" .. software .. "\\\",\\\"" .. hardware .. "\\\")\"")
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
target_end()
