# os_lab — 操作系统实验合集

操作系统核心机制的手写代码验证实验。

```
os_lab/
├── syscall_trace/      # 系统调用追踪（strace）
│   └── strace 跟踪系统调用，区分用户态/内核态切换
├── fork_test/          # 进程创建与僵尸进程
│   └── fork 父子进程实验，验证 COW、孤儿/僵尸进程
├── scheduler_sim/      # 进程调度模拟
│   └── FCFS / RR / MLFQ 三种调度算法手写实现
├── mmap_test/          # 虚拟内存与缺页
│   └── mmap 文件映射，perf 观察缺页中断次数
├── malloc_test/        # 堆内存分配路径
│   └── mallopt 调阈值，strace 验证 brk vs mmap 切换
└── inode_test/         # 文件系统 inode
    └── stat 读 inode，ln 硬链接/软链接，strace cp/mv/rm
```

## 快速开始

```bash
git clone https://github.com/Rrrayy/os_lab.git
cd os_lab

# 每个实验独立目录，各自编译运行
cd scheduler_sim
g++ -std=c++17 -o scheduler scheduler.cpp
./scheduler
```

## 环境

- 系统：Ubuntu（实验运行）
- 编译器：g++，标准 C++17
- 工具：gdb / strace / perf
