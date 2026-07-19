  #include <stdio.h>      
  #include <stdlib.h>     
  #include <unistd.h>     
  #include <fcntl.h>      // open 的选项
  #include <time.h>       // clock_gettime

  int main(int argc, char* argv[]){
      // argv[1]=源文件  argv[2]=目标文件
      if(argc != 3){
          printf("用法: %s 源文件 目标文件\n", argv[0]);
          return 1;
      }

      // 只读方式打开源文件
      int src_fd = open(argv[1], O_RDONLY);
      if(src_fd == -1){ perror("open源"); return 1; }

      // 写方式打开目标文件（不存在则创建，存在则清空）
      int dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if(dst_fd == -1){ perror("open目标"); close(src_fd); return 1; }

      // 4KB 缓冲区——read 读到这，write 从这写
      char buf[4096];
      ssize_t n;  // read/write 的返回值类型

      // 计时开始
      struct timespec t1, t2;
      clock_gettime(CLOCK_MONOTONIC, &t1);

      // 核心循环：read→write，读到0字节表示文件读完
      while((n = read(src_fd, buf, sizeof(buf))) > 0){
          write(dst_fd, buf, n);
      }

      // 计时结束
      clock_gettime(CLOCK_MONOTONIC, &t2);

      // 计算耗时（纳秒转毫秒）
      long ns = t2.tv_nsec - t1.tv_nsec;
      if(ns < 0){ t2.tv_sec--; ns += 1000000000L; }
      long ms = (t2.tv_sec - t1.tv_sec) * 1000 + ns / 1000000;
      printf("traditional: %ld ms\n", ms);

      close(src_fd);
      close(dst_fd);
      return 0;
  }
