#ifndef REACTOR_H
#define REACTOR_H
#define NULL 0

#include "log.hpp"
#include <boost/ptr_container/ptr_list.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>

#ifdef __cplusplus
}
#endif
#define FD_COPY(f, t) memcpy(t, f, sizeof(*(f))) 


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



#endif
