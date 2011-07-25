#ifndef REACTOR_H
#define REACTOR_H
#define NULL 0

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

public :
    void run();
    static Reactor* getInstance();
    void cancelTimedCall(DelayedCall* timedcall);
    DelayedCall* callLater(int time, void (*func)());
};

Reactor* Reactor::instance = NULL;

#endif
