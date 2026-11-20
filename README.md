# executable-entangler · 可执行文件纠缠器

把两个 Windows 可执行文件（`.exe`）**纠缠**进一个 `.exe`：运行宿主时，它会把自己体内藏着的两个程序释放到指定目录，并同时启动它们。

> 项目名 `executable-entangler` 直译就是“可执行文件纠缠器”。代码注释里作者把它称作「小白白的量子纠缠器」——两个 exe 被打包到一起，运行时再一起“坍缩”出来。

---

## 目录

- [特性](#特性)
- [环境要求](#环境要求)
- [快速开始](#快速开始)
- [命令行用法](#命令行用法)
- [工作原理](#工作原理)
- [目录结构](#目录结构)
- [注意事项与已知限制](#注意事项与已知限制)
- [常见问题](#常见问题)
- [免责声明](#免责声明)
- [许可证](#许可证)

---

## 特性

- **单文件分发**：两个程序被合并成一个 exe，不再需要额外附件。
- **无控制台窗口**：宿主使用 `-mwindows` 链接，启动时静默完成释放，只有被释放的两个子程序各自弹窗。
- **释放目录可配置**：运行时把两个程序写到指定目录，而不是只能放在当前目录。
- **纯 C++ / MinGW 实现**：只依赖 `g++` 与 `objcopy`，没有第三方库。
- **自带示例**：`hello/` 下有两个最小示例程序，配套脚本可以一键跑通整个流程。

---

## 环境要求

| 项目 | 要求 |
| --- | --- |
| 操作系统 | Windows（模板使用了 `windows.h`、`CreateProcess`，构建脚本是 PowerShell） |
| 编译器 | MinGW-w64 的 `g++`，且需能调用 binutils 的 `objcopy` |
| Shell | PowerShell（`.ps1` 脚本） |
| 编码 | 源码为 GBK 编码，请用支持 GBK 的编辑器打开 |

---

## 快速开始

### 方式一：先跑通示例（固定 awa / bwb）

```powershell
# 0. 准备输出目录（脚本不会自动创建）
mkdir build

# 1. 把两个 hello 示例编译成 awa.exe 和 bwb.exe
.\makeHelloExe.ps1

# 2. 用模板里的默认宏，直接编译出宿主 build\test.exe
.\makeModleExe.ps1

# 3. 运行宿主：它会在 temp\ 下释放 awa.exe、bwb.exe 并启动
.\build\test.exe
```

运行后你会看到两个控制台窗口分别输出 `Hello A!` 和 `Hello B!`，同时 `temp\` 目录里出现了这两个 exe。

> 这条路径编译出的宿主是**控制台程序**（没有加 `-mwindows`），所以会多一个宿主自己的黑窗口。

### 方式二：用纠缠器本体打包任意两个 exe

```powershell
# 1. 编译纠缠器本体
g++ main.cpp -o a.exe

# 2. 执行纠缠：A、B、输出宿主、释放目录
.\a.exe awa.exe bwb.exe build\test.exe temp\\

# 3. 运行生成的宿主，两个程序会被释放到 temp\ 并启动
.\build\test.exe
```

这一次生成的 `build\test.exe` 会以 `-mwindows` 链接，**没有宿主自己的控制台窗口**。

---

## 命令行用法

```
使用方法： a.exe 【文件A】 【文件B】 【输出】 【释放路径】
```

| 参数 | 含义 |
| --- | --- |
| `argv[1]` | 文件 A：要嵌入的第一个 exe |
| `argv[2]` | 文件 B：要嵌入的第二个 exe |
| `argv[3]` | 输出：生成的宿主 exe 路径 |
| `argv[4]` | 释放路径：运行时把两个程序写到哪里，需以分隔符结尾 |

示例：

```powershell
.\a.exe awa.exe bwb.exe out\qwq.exe temp\\
```

即：把 `awa.exe` 和 `bwb.exe` 变成一个 `out\qwq.exe`，打开 `qwq.exe` 时两者会被释放到 `temp\` 并运行。

### 关于释放路径的写法

释放路径会被原样拼进生成的 C++ 源码：

```cpp
#define RELASE_DIR "<你传入的路径>"
```

因此**反斜杠必须写成两个**，否则 `"temp\"` 里的 `\"` 会把引号转义掉，导致 `g++` 报“缺少结尾的 `"` 字符”。推荐写法：

| 写法 | 结果 |
| --- | --- |
| `temp\\` | ✅ 生成 `"temp\\"`，运行时目录为 `temp\` |
| `.\\` | ✅ 释放到当前目录 |
| `temp/` | ✅ 正斜杠也可以，Windows 大多接口都接受 |
| `temp\` | ❌ 生成非法字符串字面量，编译失败 |

---

## 工作原理

整体流程：

```
helloA.cpp ──g++──> awa.exe ──┐
                              ├── objcopy ──> awa.exe.o ─┐
helloB.cpp ──g++──> bwb.exe ──┘                          ├── g++ -mwindows ──> test.exe
                              └── objcopy ──> bwb.exe.o ─┤        （宿主）
                                                          │
                                        modle/modle.cpp ──┘  内含二进制数据 + 释放逻辑

运行 test.exe ──> 在 释放目录 写出 awa.exe / bwb.exe ──> CreateProcess 分别启动
```

分四步：

### 1. `objcopy`：把 exe 变成可链接的“二进制目标文件”

```powershell
objcopy -I binary -O pe-x86-64 -B i386:x86-64 awa.exe awa.exe.o
```

`-I binary` 表示源文件按原始二进制处理，`-O pe-x86-64` 生成 64 位 PE 目标文件。objcopy 会为数据自动生成一对符号：

```
_binary_awa_exe_start   ← 数据起始地址
_binary_awa_exe_end     ← 数据结束地址
```

符号名的规则是 `_binary_` + 文件名（点号变下划线）+ `_start` / `_end`。`main.cpp` 里的 `to_ofile()` 做的就是这件事。

### 2. 生成 `temp.cpp`：注入宏定义

`main.cpp` 用 `echo` 逐行写出 `temp.cpp`，内容包括：

```cpp
#define IS_DEBUG ...            // 表示宏由外部注入
#define RELASE_DIR "temp\\"     // 运行时释放目录
#define A_BINARY_START _binary_awa_exe_start
#define A_BINARY_END   _binary_awa_exe_end
#define A_BINARY_OUT   "awa.exe"
// B 同理……
```

随后用 `type modle\modle.cpp >> temp.cpp` 把宿主模板拼到后面。

> `modle/modle.cpp` 顶部的 `#if !defined(IS_DEBUG)` 是“宏是否由外部注入”的开关：直接编译模板时使用内置的默认值（对应 `awa.exe` / `bwb.exe`），作为 `temp.cpp` 的一部分时则采用注入的宏。

### 3. 编译宿主

```powershell
g++ -mwindows temp.cpp awa.exe.o bwb.exe.o -o build\test.exe
```

`-mwindows` 让最终程序成为 GUI 子系统程序，运行时不会额外弹出一个控制台。

### 4. 运行时释放并启动

`modle/modle.cpp` 里的 `relase_binary()` 会：

1. `mkdir` 创建释放目录；
2. 用 `ofstream` 以二进制方式，按 `_start` 到 `_end` 的范围把数据写回成 exe；
3. 用 `CreateProcess` 启动刚释放出来的程序。

最后 `main()` 依次释放 A、B，两个子进程几乎同时被拉起。

---

## 目录结构

```
executable-entangler/
├── main.cpp              # 纠缠器本体：把 A、B 打包成宿主
├── hello/
│   ├── helloA.cpp        # 示例程序 A（输出 Hello A!）
│   └── helloB.cpp        # 示例程序 B（输出 Hello B!）
├── modle/
│   └── modle.cpp         # 宿主模板：释放并启动两个二进制
├── makeHelloExe.ps1      # 编译 helloA / helloB 为 awa.exe、bwb.exe
├── makeModleExe.ps1      # 用模板默认宏直接编译宿主 build\test.exe
├── clean.ps1             # 清理 exe / o / temp.cpp
└── .gitignore            # 忽略 build、*.exe、*.o、temp.cpp
```

> 注意 `modle` 是作者对 `model` 的拼写，代码中保持一致即可。

---

## 注意事项与已知限制

- **仅支持 Windows**：宿主模板依赖 `windows.h` 与 `CreateProcess`，构建脚本依赖 PowerShell 和 cmd 内建命令（`echo`、`type`、`del`）。
- **路径和文件名不能含空格**：命令是字符串拼接出来的，空格会把参数拆断。
- **释放路径分隔符要写成 `\\`**：见上文[关于释放路径的写法](#关于释放路径的写法)。
- **输出目录需预先存在**：`-o build\test.exe` 不会自动创建 `build` 目录。
- **只支持两个文件**：代码里 A、B 是写死的，要捆绑更多程序需要自行扩展循环。
- **明文释放**：二进制没有加密或压缩，释放出来的就是原始 exe。
- **源码为 GBK 编码**：在 UTF-8 环境下阅读中文注释/帮助文本可能乱码。
- **脚本里的文件名大小写不一致**：`makeHelloExe.ps1` 写的是 `hello\HelloA.cpp`，实际文件是 `helloA.cpp`。Windows 文件系统大小写不敏感所以没事，但在大小写敏感的环境里需要改脚本。
- **可能被安全软件拦截**：“把一个 exe 藏在另一个 exe 里、运行时释放”正是程序捆绑（binder / dropper）的典型行为，杀毒软件有可能报毒，这属于预期现象。

---

## 常见问题

**Q：编译 `temp.cpp` 时报“缺少结尾的 `"` 字符”？**
释放路径末尾用了单个 `\`。把它改成 `\\`，例如 `temp\\`。

**Q：`makeModleExe.ps1` 报找不到输出文件？**
先手动 `mkdir build`，脚本不会创建目录。

**Q：`makeHelloExe.ps1` 报找不到 `HelloA.cpp`？**
脚本里的文件名大小写与实际文件不符。Windows 下通常没问题；若在大小写敏感环境，请把脚本改成 `helloA.cpp` / `helloB.cpp`。

**Q：生成的宿主为什么没有黑窗口？**
`main.cpp` 编译宿主时加了 `-mwindows`；`makeModleExe.ps1` 没有加，所以那条路径会有控制台。

**Q：能不能在 Linux 上跑？**
宿主模板不行（Windows API）。`main.cpp` 注释里给出了 Linux 下 `objcopy` 的参数（`elf32-i386` / `elf64-x86-64`）和生成 `.o` 的方式，但释放与启动逻辑需要重写。

**Q：能不能改释放目录或释放后的行为？**
可以。释放目录由第 4 个参数控制；释放后是否运行、如何运行在 `modle/modle.cpp` 的 `relase_binary()` 里。

---

## 免责声明

本项目是学习“可执行文件结构、二进制嵌入与进程启动”的教学示例。请仅用于自己拥有或已获授权的程序；将恶意程序打包分发可能违反法律法规，作者不对任何滥用行为负责。

---

## 许可证

本项目基于 [MIT License](LICENSE) 开源，Copyright (c) 2022 NekokeCore。
