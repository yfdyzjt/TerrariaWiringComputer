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

### Navigate to the Project Folder

```bash
cd /TerrariaWiringComputer
```

*Replace with the actual directory*

### Pull the Docker Image

```bash
docker pull yfdyzjt/terraria-wiring-computer
```

### Run the Docker Container

```bash
# Linux
docker run -it --rm -v ${PWD}:/TerrariaWiringComputer yfdyzjt/terraria-wiring-computer

# Windows
docker run -it --rm -v %CD%:/TerrariaWiringComputer yfdyzjt/terraria-wiring-computer
```

#### Install WireShark circuit acceleration mod (optional)

If the circuit runs slowly, you can use the [WireShark](https://github.com/cc004/wireshark "WireShark") circuit acceleration mod, which can preload circuits to improve the execution efficiency of some circuit codes without changing the circuit logic.

## Build

Execute the following command in the Docker container to complete the build from hardware to software:

```bash
xmake
```

*The generated files will be placed in the `./system` folder*

Use the following command to copy the generated world files to the Terraria and tModLoader world save folders:

```bash
copy_world
```

#### Configure Software Build

Use the following command to configure the software project you want to build:

```bash
xmake f --soft=[software_name]
```

*Where software_name corresponds to a software project in the `./software folder`, with a default value of test.*

#### Configure Hardware Build

Use the following command to configure the hardware project you want to build:

```bash
xmake f --hard=[hardware_name]
```

*Where hardware_name corresponds to a hardware project in the `./hardware/module folder`, with a default value of mini.*

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
