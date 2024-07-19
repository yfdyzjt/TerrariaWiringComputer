local softwarename = "test"
local hardwarename = "full"

target(softwarename)
    set_kind("binary") 

    set_filename(softwarename .. ".elf")

    add_files("src/**.c")
    add_includedirs("include")

    add_ldflags("-Wl,-T,./system/bin/" .. hardwarename .. "_link.ld")

    includes("../../hardware/src/" .. hardwarename .. ".lua")
    for _, part in ipairs(Parts) do
        if part.Type ~= "cpu" and part.Type ~= "memory_ins" and part.Type ~= "driver" then
            add_defines(
                "_" .. part.Name .. "_origin=" .. part.Origin,
                "_" .. part.Name .. "_length=" .. part.Length
            )
        end
    end

    before_link(
        function(target)
            os.exec("tmake do \"Include(self,\\\"tmake.lua\\\").MakeLinkScript(\\\"" .. hardwarename .. "\\\")\"")
        end
    )

    after_build(
        function(target)
            os.exec("riscv-none-elf-objcopy -O binary ./system/bin/" .. softwarename .. ".elf ./system/bin/" .. softwarename .. ".bin")
            os.exec("riscv-none-elf-size -Ax ./system/bin/" .. softwarename .. ".elf")
            -- os.exec("riscv-none-elf-objdump -D -b binary ./system/bin/" .. softwarename .. ".bin -mriscv > ./system/bin/" .. softwarename .. ".s")

            os.exec("tmake do \"Include(self,\\\"tmake.lua\\\").MakeWorld(\\\"" .. softwarename .. "\\\",\\\"" .. hardwarename .. "\\\")\"")
        end
    )
target_end()
