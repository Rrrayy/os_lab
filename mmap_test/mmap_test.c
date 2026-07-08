#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <unistd.h>     // write/read/close
#include <fcntl.h>      // open/O_RDWR
#include <sys/mman.h>   // mmap/munmap
#include <string.h>     // strlen
#include <sys/stat.h>   // fstat

/*
 * 实验1: mmap 文件映射 —— 写内存 = 写文件
 * 实验2: perf 观察缺页次数
 */

void experiment1() {
	printf("===== 实验1: mmap 文件映射 =====\n");

	// 1. 创建测试文件并写入内容
	int fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) { perror("open"); exit(1); }

	const char *initial = "Hello mmap! This is a test of virtual memory.";
	write(fd, initial, strlen(initial));

	// 2. mmap 映射文件到内存
	//    参数: 地址NULL(让内核选) / 长度 / 读写权限 / 共享映射(改内存=改文件) / 文件描述符 / 偏移0
	size_t size = strlen(initial);
	char *addr = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED) { perror("mmap"); close(fd); exit(1); }

	// 3. mmap 返回的是虚拟地址
	printf("mmap 返回的地址: %p (虚拟地址)\n", addr);
	printf("修改前: %s\n", addr);

	// 4. 通过指针修改——直接改文件内容！
	addr[0] = 'X';   // H → X
	addr[6] = 'M';   // m → M

	// 5. 解除映射
	munmap(addr, size);
	close(fd);

	// 6. 重新打开文件验证内容确实被改了
	fd = open("test.txt", O_RDONLY);
	char buf[256] = {0};
	read(fd, buf, sizeof(buf));
	printf("修改后: %s\n", buf);
	printf("结论: 写内存 = 写文件，因为 mmap 直接映射了文件内核缓存\n");
	close(fd);
}


void experiment2() {
	printf("\n===== 实验2: perf 缺页观察 =====\n");
	printf("请用 perf 运行: perf stat -e page-faults ./a.out\n\n");

	// 分配 1MB 匿名内存
	size_t size_1mb = 1024 * 1024; // 1MB
	char *addr = (char*)mmap(NULL, size_1mb, PROT_READ | PROT_WRITE,
	                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) { perror("mmap"); exit(1); }

	// 逐页写入（步长 4096 = 一页）
	// 第一次访问才会触发缺页（物理内存还没分配）
	for (size_t i = 0; i < size_1mb; i += 4096) {
		addr[i] = 'A';  // 每页写一个字节触发缺页
	}
	printf("1MB 逐页写入完成\n");
	munmap(addr, size_1mb);

	// 再分配 256MB
	size_t size_256mb = 256 * 1024 * 1024;
	addr = (char*)mmap(NULL, size_256mb, PROT_READ | PROT_WRITE,
	                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) { perror("mmap"); exit(1); }

	for (size_t i = 0; i < size_256mb; i += 4096) {
		addr[i] = 'B';
	}
	printf("256MB 逐页写入完成\n");
	munmap(addr, size_256mb);
}


int main() {
	experiment1();
	experiment2();

	printf("\n=== 理论验证点 ===\n");
	printf("1. mmap 返回的是虚拟地址（/proc/pid/maps 可见）\n");
	printf("2. 第一次访问每个页 → page fault → OS 分配物理页框\n");
	printf("3. 小文件缺页少，大文件缺页多——TLB 覆盖范围有限\n");
	printf("4. MAP_SHARED 写内存 = 写文件（共享物理页框）\n");
	return 0;
}
