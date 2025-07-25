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
startup
```

*Run `startup.bat` or `startup.sh` .*

#### Install WireShark circuit acceleration mod (optional)

If the circuit runs slowly, you can use the [WireShark](https://github.com/cc004/wireshark "WireShark") circuit acceleration mod, which can preload circuits to improve the execution efficiency of some circuit codes without changing the circuit logic.

## Usage

### Build

Run the following command inside the Docker container to complete the hardware and software build:

```bash
xmake
```

*The generated files will be placed in the ./system folder*

Execute the following command in the project directory to copy the generated world file to the Terraria and tModLoader world save folders:

```bash
copy_world
```

*Run `copy_world.bat` or `copy_world.sh` .*

### Configuration

Run the following command inside the Docker container to configure the build software:

```bash
xmake f --s=[SoftwareName]
```

To configure additional options, create a `xmake.lua` file in the root directory of the build software:

```lua
config = {
    [ConfigKey1] = [ConfigValue1],
    [ConfigKey2] = [ConfigValue2],
    [ConfigKey3] = [ConfigValue3]
}
```

| Config Key | Default Value | Path | Description |
| ---- | ---- | ---- | ---- |
| world | `terraria_computer_large` | `hardware/world` | Name of the world |
| cpu | `cpu_cs100c_rv32imc` | `hardware/wiring/cpu` | Name of the CPU |
| driver | `6.96K` | `hardware/wiring/driver` | Value of the drive frequency |
| ram | `4K` | `hardware/wiring/memory/data` | Size of the data RAM |
| printf_float | `false` | `-` | Enable floating point output support |
| scanf_float | `false` | `-` | Enable floating point input support |
| terminal | `false` | `-` | Enable terminal input and output support |
