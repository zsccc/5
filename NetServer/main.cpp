// Copyright 2020, Zhu shuocheng.
//
//Author: Zhu shuocheng
//
//main

#include <signal.h>
#include "EventLoop.h"
//#include "TcpServer.h"
#include "HttpServer.h"
#include "EchoServer.h"

EventLoop *lp;
//gprof
static void sighandler1( int sig_no )   
{   
      exit(0);   
}   
static void sighandler2( int sig_no )   
{   
    lp->Quit();
}   

int main(int argc, char *argv[])
{
    signal(SIGUSR1, sighandler1);
    signal(SIGUSR2, sighandler2);
    signal(SIGINT, sighandler2);
    signal(SIGPIPE, SIG_IGN); 

    int port = 80;
    int iothreadnum = 4;
    int workerthreadnum = 4;
    if(argc == 4)
    {
        port = atoi(argv[1]);
        iothreadnum = atoi(argv[2]);
        workerthreadnum = atoi(argv[3]);
    }   

    EventLoop loop;
    lp = &loop;
    HttpServer httpserver(&loop, port, iothreadnum, workerthreadnum);
    httpserver.Start();
    
    try
    {
        loop.loop();
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr << "bad_alloc caught in ThreadPool::ThreadFunc task: " << ba.what() << '\n';
    }
    //loop.loop();

    // EventLoop loop;
    // lp = &loop;
    // EchoServer echoserver(&loop, port, iothreadnum);
    // echoserver.Start();
    // loop.loop();
    // return 0;
}
