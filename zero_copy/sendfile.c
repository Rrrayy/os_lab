  #include <stdio.h>          
  #include <stdlib.h>         
  #include <unistd.h>         
  #include <fcntl.h>          
  #include <time.h>           // clock_gettime
  #include <sys/sendfile.h>   // sendfile
  #include <sys/stat.h>       // fstat 拿文件大小

  int main(int argc, char* argv[]){
      if(argc != 3){
          printf("用法: %s 源文件 目标文件\n", argv[0]);
          return 1;
      }

      int src_fd = open(argv[1], O_RDONLY);
      if(src_fd == -1){ perror("open源"); return 1; }

      int dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if(dst_fd == -1){ perror("open目标"); close(src_fd); return 1; }

      // 用 fstat 获取源文件大小（字节数）
      struct stat st;
      fstat(src_fd, &st);
      size_t file_size = st.st_size;

      off_t offset = 0;   // 从文件开头开始传
      ssize_t sent;       // sendfile 每次实际传了多少

      struct timespec t1, t2;
      clock_gettime(CLOCK_MONOTONIC, &t1);

      // sendfile 循环：每次传剩余的部分，直到全部传完
      while(offset < (off_t)file_size){
          sent = sendfile(dst_fd, src_fd, &offset, file_size - offset);
          if(sent == -1){ perror("sendfile"); return 1; }
          // 注意：offset 传的是指针，sendfile 会自动更新它
      }

      clock_gettime(CLOCK_MONOTONIC, &t2);

      long ns = t2.tv_nsec - t1.tv_nsec;
      if(ns < 0){ t2.tv_sec--; ns += 1000000000L; }
      long ms = (t2.tv_sec - t1.tv_sec) * 1000 + ns / 1000000;
      printf("sendfile: %ld ms\n", ms);

      close(src_fd);
      close(dst_fd);
      return 0;
  }
