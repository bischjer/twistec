#include "reactor.hpp"

Reactor* Reactor::instance = NULL;

Reactor::Reactor()
  :should_stop(true),
   select_timeval(0.01)
{
  FD_ZERO(&read_filedescriptors);
  FD_ZERO(&write_filedescriptors);
  FD_ZERO(&error_filedescriptors);
}

Reactor* Reactor::getInstance()
{
  if (instance != NULL)
  {
    return instance;
  }
  instance = new Reactor();
  return instance;
}

void Reactor::handle_signal(int signal)
{
  log("Signal %d caught. Terminating mainloop", signal);
  Reactor* reactor = Reactor::getInstance();
  reactor->stop();
}

void Reactor::register_signal_handlers()
{
  signal(SIGTERM, Reactor::handle_signal);
  signal(SIGINT, Reactor::handle_signal);
}

void Reactor::run()
{
  this->register_signal_handlers();
  this->should_stop = false;
  while(!this->should_stop)
  {
    this->check_selects();
    this->run_timers();
  }
  this->timer_list.clear();
  return;
}

void Reactor::check_selects()
{
  fd_set read_filedescriptors;
  fd_set write_filedescriptors;
  fd_set error_filedescriptors;
  FD_COPY(&this->read_filedescriptors,
          &read_filedescriptors);
  FD_COPY(&this->write_filedescriptors,
          &write_filedescriptors);
  FD_COPY(&this->error_filedescriptors,
          &error_filedescriptors);
  timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = this->select_timeval * 1000000;
  if (select(0,
             &read_filedescriptors,
             &write_filedescriptors,
             &error_filedescriptors,
             &timeout) > 0)
  {
  }
}

void Reactor::run_timers()
{
  std::list<DelayedCall*>::iterator timer;
  for(timer = this->timer_list.begin();
      timer != this->timer_list.end();
      timer ++)
  {
    if ((*timer)->timedOut())
    {
      (*timer)->func();
      this->removeTimedCall(*timer);
    }
  }
}

void Reactor::stop()
{
  this->should_stop = true;
}

DelayedCall* Reactor::newDelayedCall(int time, void (*func)())
{
  DelayedCall* call_later = new DelayedCall(time,
                                            func);
  this->timer_list.push_back(call_later);
  return call_later;
}

DelayedCall* Reactor::callLater(int time, void (*func)())
{
  return this->newDelayedCall(time, func);
}

void Reactor::cancelTimedCall(DelayedCall* timed_call)
{
  this->removeTimedCall(timed_call);
}

void Reactor::removeTimedCall(DelayedCall* timed_call)
{
    //this->timer_list.erase(timed_call);

  std::list<DelayedCall*>::iterator timer;
  for (timer=this->timer_list.begin();
       timer!=this->timer_list.end();
       timer++)
  {

    if(*timer == timed_call)
    {
#ifdef DEBUG
        log("%p %p\n", (void *) *timer, (void *) timed_call);
#endif
        delete *timer;
        break;
    }
  }

}

DelayedCall::DelayedCall(double time,
                         void (*func)())
{
  gettimeofday(&this->time, NULL);
  this->time.tv_sec += (int) time;
  this->time.tv_usec += time-(int)time;
  this->func = func;
  this->called = false;
  this->cancelled = false;
}

DelayedCall::~DelayedCall()
{
}

bool DelayedCall::timedOut()
{
  timeval now;
  gettimeofday(&now, NULL);
#ifdef DEBUG
  log_debug("%d.%d %d.%d\n",
            (int)this->time.tv_sec,
            (int)this->time.tv_usec,
            (int)now.tv_sec,
            (int)now.tv_usec);
#endif
  if (now.tv_sec > this->time.tv_sec ||
      (now.tv_sec == this->time.tv_sec &&
       now.tv_usec > this->time.tv_usec)
      )
  {
    return true;
  }
  return false;
}
