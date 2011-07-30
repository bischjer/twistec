#ifndef REACTOR_H
#define REACTOR_H
#define NULL 0

#include "log.h"
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <boost/ptr_container/ptr_list.hpp>

class DelayedCall{
  friend class Reactor;
 private:
  timeval time;
  void (*func)();
  void (*canceller)(DelayedCall*);
  bool cancelled;
  bool called;

 public:
  DelayedCall(double time,
              void (*func)());
  ~DelayedCall();
  bool timedOut();
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
    boost::ptr_list<DelayedCall> timer_list;
    Reactor();

public :
    void run();
    void stop();

    static Reactor* getInstance();
    void cancelTimedCall(DelayedCall* timedcall);
    void removeTimedCall(DelayedCall* timed_call);
    DelayedCall* callLater(int time, void (*func)());
};

Reactor* Reactor::instance = NULL;

#endif
