<div align="center">
  <h1>泰拉瑞亚电路计算机</h1>

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
  <b>在原版泰拉瑞亚中使用电路制作的 RISC-V 32 位计算机</b><br/>
  <i>模块化计算机系统：简单、灵活、开放的高性能与高密度电路</i><br/>
</div>

## 简介

Terraria Wiring Computer 是一个基于 [泰拉瑞亚](https://store.steampowered.com/app/105600 "Terraria") 原版电路的 RISC-V 32 位计算机。

- 实现标准的 rv32imc 指令集，可以使用标准的 RISC-V 工具链；

- 完全模块化的硬件，可自由的选择合适的硬件，开发简单，使用灵活；

- 使用 [tmake](https://github.com/yfdyzjt/TMake "tmake") + [xmake](https://github.com/xmake-io/xmake "xmake") 脚本构建硬件与软件，功能强大、构建便捷；

- 针对电路的面积和性能进行大量优化，致力于探索泰拉瑞亚电路的极限。

## 安装

### Windows

#### 安装 Terraria Wiring Computer

使用 winget 安装 git

```bash
winget install Git.Git
```

使用 git 安装 Terraria Wiring Computer

```bash
git clone https://github.com/yfdyzjt/TerrariaWiringComputer
cd TerrariaWiringComputer
```

*如无法使用上述方式安装 Terraria Wiring Computer ，请下载 [TerrariaWiringComputer-master.zip](https://github.com/yfdyzjt/TerrariaWiringComputer/archive/refs/heads/master.zip "TerrariaWiringComputer-master.zip") 并手动解压*

#### 安装 tmake

使用 winget 安装 curl

```bash
winget install cURL.cURL
```

使用 curl 安装 tmake

```bash
curl -o tmake.exe -L https://github.com/yfdyzjt/TMake/releases/latest/download/tmake-win-x64.exe
```

*如无法使用上述方式安装 tmake ，请下载 [tmake-win-x64.exe](https://github.com/yfdyzjt/TMake/releases/latest/download/tmake-win-x64.exe "tmake-win-x64.exe") 并手动重命名为 tmake.exe*

#### 安装 xmake

使用 winget 安装 xmake

```bash
winget install xmake
```

*如无法使用上述方式安装 winget ，请查看 [其它 xmake 安装方法](https://xmake.io/#/zh-cn/guide/installation "xmake install")*

#### 安装 riscv 工具链

使用 winget 安装 npm

```bash
winget install OpenJS.NodeJS
```

使用 npm 安装 xpm

```bash
npm install --location=global xpm@latest
```

使用 xpm 安装 riscv-none-elf-gcc-xpark

```bash
xpm init
xpm install @xpack-dev-tools/riscv-none-elf-gcc@latest --verbose
```

*如无法使用上述方式安装 riscv 工具链，请查看 [其它 riscv-none-elf-gcc-xpark 安装方法](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack "riscv-none-elf-gcc-xpark install")*

#### 安装电路加速模组 WireShark （可选） 

如果电路运行缓慢，可以使用电路加速模组 [WireShark](https://github.com/cc004/wireshark "WireShark") ，该模组可预加载电路，在不改变电路逻辑的情况下提高电路部分代码的执行效率

### Linux

#### 安装 Terraria Wiring Computer

使用 apt 安装 git

```bash
sudo apt-get install git
```

使用 git 安装 Terraria Wiring Computer

```bash
git clone https://github.com/yfdyzjt/TerrariaWiringComputer
cd TerrariaWiringComputer
```

*如无法使用上述方式安装 Terraria Wiring Computer ，请下载 [TerrariaWiringComputer-master.zip](https://github.com/yfdyzjt/TerrariaWiringComputer/archive/refs/heads/master.zip "TerrariaWiringComputer-master.zip") 并手动解压*

#### 安装 tmake

使用 apt 安装 curl

```bash
sudo apt-get install curl
```

使用 curl 安装 tmake

```bash
curl -o tmake -L https://github.com/yfdyzjt/TMake/releases/latest/download/tmake-linux-x64
chmod +x tmake
```

*如无法使用上述方式安装 tmake ，请下载 [tmake-linux-x64.exe](https://github.com/yfdyzjt/TMake/releases/latest/download/tmake-linux-x64.exe "tmake-linux-x64.exe") 并手动重命名为 tmake.exe*

#### 安装 xmake

使用 apt 安装 xmake

```bash
sudo apt-get install xmake
```

*如无法使用上述方式安装 winget ，请查看 [其它 xmake 安装方法](https://xmake.io/#/zh-cn/guide/installation "xmake install")*

#### 安装 riscv 工具链

使用 apt 安装 npm

```bash
sudo apt-get install npm
```

使用 npm 安装 xpm

```bash
npm install --location=global xpm@latest
```

使用 xpm 安装 riscv-none-elf-gcc-xpark

```bash
xpm init
xpm install @xpack-dev-tools/riscv-none-elf-gcc@latest --verbose
```

*如无法使用上述方式安装 riscv 工具链，请查看 [其它 riscv-none-elf-gcc-xpark 安装方法](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack "riscv-none-elf-gcc-xpark install")*

#### 安装电路加速模组 WireShark （可选） 

如果电路运行缓慢，可以使用电路加速模组 [WireShark](https://github.com/cc004/wireshark "WireShark") ，该模组可预加载电路，在不改变电路逻辑的情况下提高电路部分代码的执行效率

## 构建

在项目根目录执行下面的指令即可完成从硬件到软件的构建

```bash
xmake
```

生成的文件会放置在 `./system` 文件夹，并且会将生成的地图文件复制到 Terraria 和 tModLoader 地图存档文件夹

#### 配置构建软件

使用下面的指令配置需要构建的软件项目

```bash
xmake f --soft=[software_name]
```

*其中 software_name 对应在 `./software` 文件夹内的软件项目，默认值为 test*

#### 配置构建硬件

使用下面的指令配置需要构建的硬件项目

```bash
xmake f --hard=[hardware_name]
```

*其中 hardware_name 对应在 `./hardware/module` 文件夹内的硬件项目，默认值为 mini*

## 支持

本项目还有很多未完成和不完善的部分，参与到本项目的制作和完善就是对本项目最大的支持

### 硬件

#### 构建脚本

硬件构建脚本指的是 `./hardware/module/[hardware_name].lua` ，该脚本包括以下三个属性：

|  名称  |  类型  |  描述  |
|  :---  |  :---  |  :---  |
|  World  |  string  |  使用的世界，位于 `/hardware/world/[World].wld`  |
|  Link  |  string  |  使用的部件链接脚本，位于 `/hardware/link/[Link].lua` |
|  Parts  |  table  |  使用的部件描述，详见下表 |

构建脚本会在世界中按照部件链接脚本描述的算法放置部件

其中 `Parts` 的成员为部件，包括下面四个属性：

|  名称  |  类型  |  描述  |
|  :---  |  :---  |  :---  |
|  Name  |  string  |  部件的名称，位于 `/hardware/wiring` 文件夹  |
|  Type  |  string  |  部件的类型，详见下表 |
|  Origin  |  int  |  部件的地址，单位字节 |
|  Length  |  int  |  部件的大小，单位字节 |

部件的类型有以下六种：

|  名称  |  描述  |
|  :---  |  :---  |
|  cpu  |  中央处理器  |
|  memory_ins  |  指令内存 |
|  memory_data  |  数据内存 |
|  input  |  输入，例如键盘 |
|  ouput  |  输出，例如屏幕 |
|  driver  |  驱动，提供时钟信号 |

*硬件构建脚本可执行 lua 代码对属性赋值*

#### 部件

##### 电路

部件应是矩形的，外侧需围绕一圈四色电线，且其上不能包括分线盒。该四色电线用于部件间通信，部件间的所有数据通过这四色电线传输。以下是四色电线的功能定义：

|  电线颜色  |  功能  |
|  :---  |  :---  |
|  红  |  驱动产生的时钟信号  |
|  蓝  |  数据内存地址 |
|  绿  |  指令内存地址与数据 |
|  黄  |  数据内存数据 |

*蓝、绿、黄线使用状态串行传输数据*

以下是蓝线传输的内存地址格式：

|  位  |  描述  |
|  :---  |  :---  |
|  0 - 11  |  地址高12位（部件间地址）  |
|  12  |  标志：半字 |
|  13  |  标志：字 |
|  14  |  标志：符号位扩展 |
|  15  |  标志：写 |
|  16 - 35  |  地址低20位（部件内地址） |

*其中标志位为1时使能*

*地址以高12位起始，一般使用状态串行序列比较判断地址高12位是否与部件地址相同*

##### 地址

部件使用 MMIO（内存映射技术），既给每个部件分配一个地址， CPU 对特定地址的读写既访问对应部件。部件的起始地址由 Origin 确定，地址范围由 Length 确定，故每个部件的可访问地址从 Origin 到 Origin + Length。

规定地址的高12位用于区分不同部件，每个部件最多使用地址的低20位（既每个部件的最大寻址范围为 1MB）。地址 0x0000_0000 分配给指令内存，地址 0xFFFF_FFFF 分配给驱动，全部驱动使用相同的地址。

##### 文件

部件放置在 `/hardware/wiring` 文件夹，按照部件类型分类。每个部件由简图 `[Name].TEditSch` 和脚本 `[Name].lua` 两个文件描述。

*.TEditSch 文件是 [泰拉瑞亚地图编辑器 TEdit](https://github.com/TEdit/Terraria-Map-Editor "泰拉瑞亚地图编辑器") 的剪贴板简图文件，可以使用 TEdit 读取或写入*

脚本文件包括以下三个成员：

|  名称  |  描述  |
|  :---  |  :---  |
|  AddrPos  |  地址读写位置  |
|  Write  |  向部件写入数据的函数 |
|  Read  |  从部件读取数据的函数（暂未实现） |

*AddrPos 为状态串行序列比较的序列逻辑灯位置，用于读写部件地址*

*通常只有 memory 类型的部件需要读写数据，仅大小不同的部件可使用相同的读写函数简化代码*

### 软件

软件源码放置在 `./software/[software_name]` 文件夹，使用 C 语言编写。其中 `src` 文件夹放置源文件， `include` 文件夹放置头文件。

软件使用根目录的 `./xmake.lua` 作为构建脚本，语法请查看 [xmake 文档](https://xmake.io/#/zh-cn/getting_started "xmake 文档") 。更换编程语言需修改编译目标。
