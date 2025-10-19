#include "myshell.h"
#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<sys/stat.h>
#include<fcntl.h>

char *gargv[ARGS] = {NULL};
int gargc = 0;
char pwd[1024];
int lastcode = 0;

#define NONE_REDIR 0
#define OUTPUT_REDIR 1
#define APPEND_REDIR 2
#define INPUT_REDIR 3

std::string filename;
int redir_type = NONE_REDIR;

void Begin()
{
  printf("ciallo~ 终端已就绪 QAQ\n");
}
static std::string GetUserName()
{
  std::string username = getenv("USER");
  return username.empty() ? "None" : username;
}
static std::string GetHostName()
{
  const char* host = getenv("HOSTNAME");
  std::string hostname = host ? host : "None";
  return hostname;
}

static std::string GetPwd()
{
  char temp[1024];
  getcwd(temp,sizeof(temp));
  //更新一下shell自己的环境变量pwd
  snprintf(pwd,sizeof(pwd),"PWD=%s",temp);
  putenv(pwd);

  std::string pwd_lable = temp;
  const std::string pathsep = "/";
  auto pos = pwd_lable.rfind(pathsep);
  if(pos == std::string::npos)
  {
    return "None";
  }

  pwd_lable = pwd_lable.substr(pos+pathsep.size());
  return pwd_lable.empty() ? "/" : pwd_lable;
}

static std::string GetHomePath()
{
  std::string homepath = getenv("HOME");
  return homepath.empty() ? "/" : homepath;
}

static std::string GetPrevPath()
{
  std:: string oldpwd = getenv("OLDPWD");
  return oldpwd.empty() ? "." : oldpwd;
}


void PrintCommandPromot()
{
  std::string user = GetUserName();
  std::string hostname = GetHostName();
  std::string pwd = GetPwd();
  printf("[%s@%s %s] Ciallo# ", user.c_str(), hostname.c_str(), pwd.c_str());
}

bool GetCommandString(char cmd_str_buff[],int len)
{
  if(cmd_str_buff == NULL || len <= 0)
    return false;
  char* res = fgets(cmd_str_buff,len,stdin);
  if(res == NULL)
    return false;
  cmd_str_buff[strlen(cmd_str_buff) - 1] = 0;

  return strlen(cmd_str_buff) == 0 ? false : true;
}

bool ParseCommandString(char cmd[])
{
  if(cmd == NULL)
    return false;
#define SEP " "
  gargv[gargc++] = strtok(cmd,SEP);
  while((bool)(gargv[gargc++] = strtok(NULL,SEP)));

  gargc--;


//#define DEBUG
#ifdef DEBUG
        printf("gargc: %d\n", gargc);
        printf("----------------------\n");
        for(int i = 0; i < gargc; i++)
        {
            printf("gargv[%d]: %s\n",i, gargv[i]);
        }
        printf("----------------------\n");
        for(int i = 0; gargv[i]; i++)
        {
            printf("gargv[%d]: %s\n",i, gargv[i]);
        }
#endif


  return true;
}
#define trimspace(start) do{\
  while(isspace(*start))\
  {\
    start++;\
  }\
}while(0)

void CheckRedir(char cmd[])
{
  char* start = cmd;
  char* end = cmd + strlen(cmd) -1;

  while(start <= end)
  {
    if(*start == '>'){
      if(*(start+1) == '>'){
        //追加重定向
        redir_type = APPEND_REDIR;
        *start = '\0';
        start+=2;
        trimspace(start);
        filename = start;
        break;
      }
      else{
        //输出重定向
        redir_type = OUTPUT_REDIR;
        *start = '\0';
        start++;
        trimspace(start);
        filename = start;
        break;
      }   
    }
    else if(*start == '<'){
      //输入重定向
      redir_type = INPUT_REDIR;
      *start = '\0';
      start++;
      trimspace(start);
      filename = start;
      break;
    }
    else{
      start++;
    }
  }
}

void ForkAndExec()
{
  pid_t id = fork();
  if(id < 0)
  {
    perror("fork");
    return;
  }
  else if(id == 0)
  {
    //子进程
    if(redir_type == OUTPUT_REDIR){
      int output = open(filename.c_str(),O_CREAT | O_WRONLY | O_TRUNC, 0666);
      if(output < 0){
        perror("open fail!\n");
        exit(1);
      }
      dup2(output,1);
    }
    else if(redir_type == INPUT_REDIR){
      int input = open(filename.c_str(),O_RDONLY);
      if(input < 0){
        perror("open fail!\n");
        exit(1);
      }
      dup2(input,0);
    }
    else if(redir_type == APPEND_REDIR){
      int appendfd = open(filename.c_str(),O_CREAT | O_WRONLY | O_APPEND);
      if(appendfd < 0){
        perror("open fail!\n");
        exit(1);
      }
      dup2(appendfd,1);
    }
    else{
      //Do Nothing
    }
    execvp(gargv[0],gargv);
    exit(0);
  }
  else 
  {
    //父进程
    int status = 0;
    pid_t rpid = waitpid(id,&status,0);
    if(rpid > 0)
    {
      lastcode = WEXITSTATUS(status);
    }
  }
}

void InitGlobal()
{
  gargc = 0;
  memset(gargv,0,sizeof(gargv));
  filename.clear();
  redir_type = NONE_REDIR;
}

bool BuildInCommandExec()
{
  std::string cmd = gargv[0];
  bool ret = false;
  if(cmd == "cd")
  {
    if(gargc == 2)
    {
      std::string target = gargv[1];
      if(target == "~")
      {
        ret = true;
        chdir(GetHomePath().c_str());
      }
      else if(target == "-")
      {
        ret = true;
        chdir(GetPrevPath().c_str());
      }
      else
      {
        ret = true;
        chdir(gargv[1]);
      }
    }
    else if(gargc == 1)
    {
      ret = true;
      chdir(GetHomePath().c_str());
    }
    else{
      //bug
    }
  }
  else if(cmd == "echo")
  {
    if(gargc == 2)
    {
      std::string args = gargv[1];
      if(args[0] == '$')
      {
        if(args[1] == '?')
        {
          printf("lastcode: %d\n", lastcode);
          lastcode = 0;
          ret = true;
        }
        else
        {
          const char *name = &args[1];
          printf("%s\n",getenv(name));
          lastcode = 0;
          ret = true;
        }
      }
      else{
        printf("%s\n",args.c_str());
        ret = true;
      }
    }
  }
  return ret;
}
