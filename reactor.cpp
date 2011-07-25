#include "reactor.h"

Reactor* Reactor::getInstance()
{
  if (instance != NULL)
  {
    return instance;
  }
  instance = new Reactor();
  return instance;
}

void Reactor::run()
{
  return;
}

DelayedCall* Reactor::newDelayedCall(int time, void (*func)())
{
  DelayedCall* call_later = new DelayedCall(time,
                                            func);
  return call_later;
}

DelayedCall* Reactor::callLater(int time, void (*func)())
{
  return this->newDelayedCall(time, func);
}

void Reactor::cancelTimedCall(DelayedCall* timed_call)
{
  delete timed_call;
  return;
}

DelayedCall::DelayedCall(int time,
                         void (*func)())
{
  this->time = time;
  this->func = func;
  this->called = false;
  this->cancelled = false;
}

DelayedCall::~DelayedCall()
{
}
