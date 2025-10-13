#ifndef __MYSTDIO_H__
#define __MYSTDIO_H__ 
 
#define FLUSH_NONE 1
#define FLUSH_LINE 2
#define FLUSH_FULL 4

#define SIZE 1024
#define UMASK 0666

#define FORCE 1
#define NORMAL 2

typedef struct MY_IO_FILE
{
  int fileno;
  int flag;
  char outbuffer[SIZE];
  int curr;
  int cap;
}MYFILE;

MYFILE* my_fopen(const char* filename, const char* mode);

void my_fclose(MYFILE* fp);

int my_fwrite(const char* s, int size, MYFILE* fp);

void my_fflush(MYFILE* fp);

#endif
