<div align="center">
  <h1>Terraria Wiring Computer</h1>

  <div>
    <a href="https://github.com/yfdyzjt/TerrariaWiringComputer/blob/master/LICENSE">
    <img src="https://img.shields.io/github/license/yfdyzjt/TerrariaWiringComputer" alt="license" />
    </a>
    <a href="https://github.com/yfdyzjt/TerrariaWiringComputer/issues">
    <img src="https://img.shields.io/github/issues/yfdyzjt/TerrariaWiringComputer" alt="issues" />
    </a>
    <a href="https://github.com/yfdyzjt/TerrariaWiringComputer">
    <img src="https://img.shields.io/github/stars/yfdyzjt/TerrariaWiringComputer?style=flat" alt="star" />
    </a>
    <a href="https://github.com/yfdyzjt/TerrariaWiringComputer/pulls">
    <img src="https://img.shields.io/github/forks/yfdyzjt/TerrariaWiringComputer?style=flat" alt="fork" />
    </a>
  </div>

  <div>
    <a href="https://space.bilibili.com/22871583">
    <img src="https://img.shields.io/badge/video-bilibili-00a2d8.svg" alt="bilibili" />
    </a>
    <a href="https://qm.qq.com/q/ZXDnybyQcE">
    <img src="https://img.shields.io/badge/chat-QQ-e91f1f.svg" alt="QQ" />
    </a>
    <a href="https://discord.gg/s6xbNqrUY2">
    <img src="https://img.shields.io/badge/chat-discord-5865f2.svg" alt="discord" />
    </a>
  </div>
  
  <br/>
  <b>RISC-V 32-bit computer made by wiring in vanilla Terraria</b><br/>
  <i>Modular Computer Systems: Simple, Flexible, Open High Performance and High Density Circuits</i><br/>
</div>

## Introduction ([中文](/README_zh.md))

Terraria Wiring Computer is a RISC-V 32-bit computer based on [Terraria](https://store.steampowered.com/app/105600 "Terraria") vanilla wiring.

- Implements the standard rv32imc instruction set and can use the standard RISC-V toolchain;

- Fully modular hardware, allowing free choice of suitable hardware, easy development, and flexible use;

- Uses [tmake](https://github.com/yfdyzjt/TMake "tmake") + [xmake](https://github.com/xmake-io/xmake "xmake") scripts to build hardware and software, powerful and convenient;

- Optimized for circuit area and performance, aiming to explore the limits of Terraria wiring.

## Installation

### Windows

#### Install Terraria Wiring Computer

Install git using winget

```bash
winget install Git.Git
```

Install Terraria Wiring Computer using git

```bash
git clone https://github.com/yfdyzjt/TerrariaWiringComputer
cd TerrariaWiringComputer
```

*If you cannot install Terraria Wiring Computer using the above method, please download [TerrariaWiringComputer-master.zip](https://github.com/yfdyzjt/TerrariaWiringComputer/archive/refs/heads/master.zip "TerrariaWiringComputer-master.zip") and extract it manually.*

#### Install tmake

Install curl using winget

```bash
winget install cURL.cURL
```

Install tmake using curl

```bash
curl -o tmake.exe -L https://github.com/yfdyzjt/TMake/releases/latest/download/tmake-win-x64.exe
```

*If you cannot install tmake using the above method, please download [tmake-win-x64.exe](https://github.com/yfdyzjt/TMake/releases/latest/download/tmake-win-x64.exe "tmake-win-x64.exe") and rename it to tmake.exe manually.*

#### Install xmake

Install xmake using winget

```bash
winget install xmake
```
*If you cannot install winget, please refer to [other xmake installation methods](https://xmake.io/#/zh-cn/guide/installation "xmake install") .*

#### Install riscv toolchain

Install npm using winget

```bash
winget install OpenJS.NodeJS
```

Install xpm using npm

```bash
npm install --location=global xpm@latest
```

Install riscv-none-elf-gcc-xpark using xpm

```bash
xpm init
xpm install @xpack-dev-tools/riscv-none-elf-gcc@latest --verbose
```

*If you cannot install the riscv toolchain using the above method, please refer to [other riscv-none-elf-gcc-xpark installation methods](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack "riscv-none-elf-gcc-xpark install").*

#### Install WireShark circuit acceleration module (optional)

If the circuit runs slowly, you can use the [WireShark](https://github.com/cc004/wireshark "WireShark") circuit acceleration module, which can preload circuits to improve the execution efficiency of some circuit codes without changing the circuit logic.

### Linux

#### Install Terraria Wiring Computer

Install git using apt

```bash
sudo apt-get install git
```

Install Terraria Wiring Computer using git

```bash
git clone https://github.com/yfdyzjt/TerrariaWiringComputer
cd TerrariaWiringComputer
```

*If you cannot install Terraria Wiring Computer using the above method, please download [TerrariaWiringComputer-master.zip](https://github.com/yfdyzjt/TerrariaWiringComputer/archive/refs/heads/master.zip "TerrariaWiringComputer-master.zip") and extract it manually.*

#### Install tmake

Install curl using apt

```bash
sudo apt-get install curl
```

Install tmake using curl

```bash
curl -o tmake -L https://github.com/yfdyzjt/TMake/releases/latest/download/tmake-linux-x64
chmod +x tmake
```

*If you cannot install tmake using the above method, please download [tmake-linux-x64.exe](https://github.com/yfdyzjt/TMake/releases/latest/download/tmake-linux-x64.exe "tmake-linux-x64.exe") and rename it to tmake.exe manually.*

#### Install xmake

Install xmake using apt

```bash
sudo apt-get install xmake
```

*If you cannot install winget, please refer to [other xmake installation methods](https://xmake.io/#/zh-cn/guide/installation "xmake install") .*

#### Install riscv toolchain

Install npm using apt

```bash
sudo apt-get install npm
```

Install xpm using npm

```bash
npm install --location=global xpm@latest
```

Install riscv-none-elf-gcc-xpark using xpm

```bash
xpm init
xpm install @xpack-dev-tools/riscv-none-elf-gcc@latest --verbose
```

*If you cannot install the riscv toolchain using the above method, please refer to [other riscv-none-elf-gcc-xpark installation methods](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack "riscv-none-elf-gcc-xpark install").*

#### Install WireShark circuit acceleration module (optional)

If the circuit runs slowly, you can use the [WireShark](https://github.com/cc004/wireshark "WireShark") circuit acceleration module, which can preload circuits to improve the execution efficiency of some circuit codes without changing the circuit logic.

## Build

Execute the following command in the project root directory to complete the build from hardware to software

```bash
xmake
```

The generated files will be placed in the `./system` folder, and the generated map files will be copied to the Terraria and tModLoader map save folders.

#### Configure software build

Use the following command to configure the software project to be built

```bash
xmake f --soft=[software_name]
```

*Where software_name corresponds to the software project in the `./software` folder, the default value is test.*

#### Configure hardware build

Use the following command to configure the hardware project to be built

```bash
xmake f --hard=[hardware_name]
```

*Where hardware_name corresponds to the hardware project in the `./hardware/module` folder, the default value is mini.*

## Support

There are still many unfinished and imperfect parts in this project. Participating in the creation and improvement of this project is the greatest support you can provide.

### Hardware

#### Build Scripts

Hardware build scripts refer to `./hardware/module/[hardware_name].lua`. These scripts include the following three properties:

| Name | Type | Description |
| --- | --- | --- |
| World | string | The world used, located in `/hardware/world/[World].wld` |
| Link | string | The component link script used, located in `/hardware/link/[Link].lua` |
| Parts | table | The component descriptions used, see the table below |

The build script will place components in the world according to the algorithm described in the component link script.

The members of `Parts` represent components and include the following four properties:

| Name | Type | Description |
| --- | --- | --- |
| Name | string | The name of the component, located in the `/hardware/wiring` folder |
| Type | string | The type of the component, see the table below |
| Origin | int | The address of the component, in bytes |
| Length | int | The size of the component, in bytes |

There are six types of components:

| Name | Description |
| --- | --- |
| cpu | Central Processing Unit |
| memory_ins | Instruction memory |
| memory_data | Data memory |
| input | Input, e.g., keyboard |
| output | Output, e.g., screen |
| driver | Driver, provides clock signal |

*Hardware build scripts can execute Lua code to assign values to properties.*

#### Components

##### Circuits

Components should be rectangular and surrounded by a ring of four-color wires. They should not include junction boxes. These four-color wires are used for communication between components, and all data between components is transmitted through these wires. The functions of the four-color wires are defined as follows:

| Wire Color | Function |
| --- | --- |
| Red | Clock signal generated by the driver |
| Blue | Data memory address |
| Green | Instruction memory address and data |
| Yellow | Data memory data |

*Blue, green, and yellow wires use serial transmission for data.*

The format of memory addresses transmitted by the blue wire is as follows:

| Bit | Description |
| --- | --- |
| 0 - 11 | High 12 bits of the address (inter-component address) |
| 12 | Flag: half-word |
| 13 | Flag: word |
| 14 | Flag: sign extension |
| 15 | Flag: write |
| 16 - 35 | Low 20 bits of the address (intra-component address) |

*The flag bits are enabled when set to 1.*

*Addresses start from the high 12 bits, and typically, a status serial sequence is used to compare and determine whether the high 12 bits match the component address.*

##### Addresses

Components use Memory-Mapped I/O (MMIO), which assigns an address to each component. When the CPU reads or writes to a specific address, it accesses the corresponding component. The starting address of a component is determined by `Origin`, and the address range is determined by `Length`. Therefore, the accessible addresses for each component range from `Origin` to `Origin + Length`.

The high 12 bits of the address are used to distinguish different components. Each component can use up to the low 20 bits of the address (i.e., the maximum addressable range for each component is 1MB). Address `0x0000_0000` is allocated for instruction memory, and address `0xFFFF_FFFF` is allocated for drivers, with all drivers using the same address.

##### Files

Components are placed in the `/hardware/wiring` folder, categorized by component type. Each component is described by two files: a schematic `[Name].TEditSch` and a script `[Name].lua`.

*.TEditSch files are clipboard schematic files for Terraria Map Editor (TEdit), which can be read or written using TEdit.*

The script file includes the following three members:

| Name | Description |
| --- | --- |
| AddrPos | Address read/write position |
| Write | Function to write data to the component |
| Read | Function to read data from the component (not yet implemented) |

*`AddrPos` is the position of the sequence logic lamp used for address read/write.*

*Usually, only memory-type components need to read/write data, and components of the same size can use the same read/write functions to simplify the code.*

### Software

Software source code is located in the `./software/[software_name]` folder and is written in C. The `src` folder contains source files, and the `include` folder contains header files.

The software uses the root `./xmake.lua` as
