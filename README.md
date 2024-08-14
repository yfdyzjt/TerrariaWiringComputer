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
