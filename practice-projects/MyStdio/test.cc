#include "mystdio.h"
#include "unistd.h"
#include "string.h"

int main()
{
  MYFILE* fp = my_fopen("log.txt","a");
  const char s[] = "Hello Mr.Young.";
  int cnt = 20;
  while(cnt--){
    my_fwrite(s, strlen(s), fp);
  }

  my_fclose(fp);
  return 0;
}
