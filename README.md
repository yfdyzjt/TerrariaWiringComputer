<div align="center">
  <h1>泰拉瑞亚电路计算机</h1>

  <div>
    <a href="https://github.com/yfdyzjt/TerrariaWiringComputer/blob/main/LICENSE">
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

## 构建

在项目根目录执行下面的指令即可完成从硬件到软件的构建

```bash
xmake
```
生成的文件会放置在 `./system` 文件夹，并且会将生成的地图文件复制到 Terraria 和 tModLoader 地图存档文件夹

---

使用下面的指令配置需要构建的软件项目

```bash
xmake f --soft=[software_name]
```

*其中 software_name 对应在 `./software` 文件夹内的软件项目，默认值为 test*

---

使用下面的指令配置需要构建的硬件项目

```bash
xmake f --hard=[hardware_name]
```

*其中 hardware_name 对应在 `./hardware/module` 文件夹内的硬件项目，默认值为 mini*

