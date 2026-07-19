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
  ├── innode_test/        # 文件系统 inode
  │   └── stat 读 inode，ln 硬链接/软链接，strace cp/mv/rm
  ├── multithread/        # 多线程竞态与同步
  │   └── mutex / atomic / 无锁三版 benchmark 对比
  ├── spinlock/           # 自旋锁手写
  │   └── atomic_flag 实现 spin_lock，多核 vs 单核 benchmark
  ├── dead_lock/          # 死锁复现与定位
  │   └── ABBA 死锁 + gdb attach 抓现场 + 条件变量生产者消费者
  ├── io_model/           # IO 模型对比
  │   └── 阻塞 read vs 非阻塞 read + O_NONBLOCK + EAGAIN
  └── zero_copy/          # 零拷贝 benchmark
      └── sendfile vs read+write，strace 10 万次 vs 1 次系统调用

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
