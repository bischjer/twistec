#include "reactor.h"

Reactor::Reactor()
{
  should_stop = true;
  FD_ZERO(&read_filedescriptors);
  FD_ZERO(&write_filedescriptors);
  FD_ZERO(&error_filedescriptors);
  select_timeval = 0.01;
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
  std::string s;
  std::stringstream out;
  out << signal;
  s = out.str();
  std::string log_output("Signal ");
  log_output += s;
  log_output += " caught. Terminating mainloop";
  log((char*)log_output.c_str());
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
}

void Reactor::stop()
{
  this->should_stop = true;
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
