#pragma once 

#ifndef __MYSHELL_H__
#define __MYSHELL_H__

#include<cstdio>

#define ARGS 64

void Begin();

void InitGlobal();

void PrintCommandPromot();

bool GetCommandString(char cmd_str_buff[],int len);

bool ParseCommandString(char cmd[]);

void ForkAndExec();

bool BuildInCommandExec();
#endif
