#ifndef REACTOR_H
#define REACTOR_H
#define NULL 0

#include "log.h"
#include <signal.h>
#include <string>
#include <sstream>
#include <sys/select.h>
#include <sys/time.h>

class DelayedCall{
 private:
  int time;
  void (*func)();
  void (*canceller)(DelayedCall*);
  bool cancelled;
  bool called;

 public:
  DelayedCall(int time,
              void (*func)());
  ~DelayedCall();
  void cancel();
  void isActive();
};

class Reactor {
private :
    static Reactor *instance;
    DelayedCall* newDelayedCall(int time, void (*func)());
    void check_selects();
    void run_timers();
    void register_signal_handlers();
    static void handle_signal(int signal);
    bool should_stop;
    double select_timeval;
    fd_set read_filedescriptors;
    fd_set write_filedescriptors;
    fd_set error_filedescriptors;
    Reactor();

public :
    void run();
    void stop();

    static Reactor* getInstance();
    void cancelTimedCall(DelayedCall* timedcall);
    DelayedCall* callLater(int time, void (*func)());
};

Reactor* Reactor::instance = NULL;

#endif
