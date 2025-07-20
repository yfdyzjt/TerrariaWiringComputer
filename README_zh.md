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

### 导航到项目文件夹

```bash
cd /TerrariaWiringComputer
```

*需替换为实际的目录*

### 拉取 docker 镜像

```bash
docker pull yfdyzjt/terraria-wiring-computer
```

### 运行 docker 容器

```bash
startup
```

*执行 `startup.bat` 或 `startup.sh`*

### 安装电路加速模组 WireShark （可选） 

如果电路运行缓慢，可以使用电路加速模组 [WireShark](https://github.com/cc004/wireshark "WireShark") ，该模组可预加载电路，在不改变电路逻辑的情况下提高电路部分代码的执行效率

## 使用

### 构建

在 docker 容器中执行下面的指令可完成从硬件到软件的构建

```bash
xmake
```

*生成的文件会放置在 `./system` 文件夹*

在项目目录中执行下面的指令将生成的地图文件复制到 Terraria 和 tModLoader 地图存档文件夹

```bash
copy_world
```

*执行 `copy_world.bat` 或 `copy_world.sh`*

### 配置

在 docker 容器中执行下面的指令设置构建软件对象

```bash
xmake f --s=[SoftwareName]
```

如需配置其它选项，请在对应软件根目录新建一个内容如下的 `xmake.lua` 文件

```lua
config = {
    [ConfigKey1] = [ConfigValue1],
    [ConfigKey2] = [ConfigValue2],
    [ConfigKey3] = [ConfigValue3]
}
```

| 配置项 | 默认值 | 路径 | 描述 |
| --- | --- | --- | --- |
| world | `terraria_computer_large` | `hardware/world` | 使用的世界名称 |
| cpu | `cpu_cs100c_rv32imc` | `hardware/wiring/cpu` | 使用的CPU名称 |
| driver | `6.96K` | `hardware/wiring/driver` | 使用的驱动频率大小 |
| ram | `4K` | `hardware/wiring/memory/data` | 使用的数据RAM大小 |
| printf_float | `false` | `-` | 启用浮点数输出支持 |
| scanf_float | `false` | `-` | 启用浮点数输入支持 |
| terminal | `false` | `-` | 启用终端输入输出支持 |