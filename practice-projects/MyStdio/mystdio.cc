#include "mystdio.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

MYFILE* my_fopen(const char* filename, const char* mode)
{
  int fd = -1;
  if(strcmp(mode, "w") == 0){
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, UMASK);
  }
  else if(strcmp(mode, "r") == 0){
    fd = open(filename, O_RDONLY);
  }
  else if(strcmp(mode, "a") == 0){
    fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, UMASK);
  }
  else if(strcmp(mode, "a+") == 0){
    fd = open(filename, O_CREAT | O_RDWR | O_APPEND, UMASK);
  }
  else{}

  if(fd < 0){
    return NULL;
  }
  MYFILE* fp = (MYFILE*)malloc(sizeof(MYFILE));
  if(!fp){
    return NULL;
  }
  fp->fileno = fd;
  fp->flag = FLUSH_LINE;
  fp->curr = 0;
  fp->cap = SIZE;
  fp->outbuffer[0] = 0;

  return fp;
}

static void my_fflush_core(MYFILE* fp, int force)
{
  if(fp->curr <= 0){
    return;
  }
  if(force == FORCE){
    write(fp->fileno, fp->outbuffer, fp->curr);
    fp->curr = 0;
  }
  else{
    //开始按刷新条件刷新
    if(fp->flag == FLUSH_LINE && fp->outbuffer[fp->curr - 1] == '\n'){
      //开始刷新
      write(fp->fileno, fp->outbuffer, fp->curr);
      fp->curr = 0;
    }
    else if(fp->flag == FLUSH_FULL && fp->curr == fp->cap){
      write(fp->fileno, fp->outbuffer, fp->curr);
      fp->curr = 0;
    }
    else{}
  }
}

int my_fwrite(const char* s, int size, MYFILE* fp)
{
  memcpy(fp->outbuffer + fp->curr, s, size);
  fp->curr += size;
  my_fflush_core(fp, NORMAL);
  return size;
}

void my_fflush(MYFILE* fp)
{
  my_fflush_core(fp, FORCE);
}

void my_fclose(MYFILE* fp)
{
  if(fp->fileno >= 0)
  {
    my_fflush(fp);//用户->语言缓冲区
    fsync(fp->fileno);//语言缓冲区->内核缓冲区
    close(fp->fileno);
    free(fp);
  }
}
