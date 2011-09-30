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
	std::clog
		<< "Signal "
		<< signal
		<< "caught. Terminating mainloop"
		<< std::endl;
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

bool Reactor::is_running()
{
	return !should_stop;
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

DelayedCallComparator::DelayedCallComparator(DelayedCall* item)
 :_item(item)
{
}

bool DelayedCallComparator::operator() (DelayedCall* other)
{
	return (_item == other);
}

void Reactor::removeTimedCall(DelayedCall* timed_call)
{
	std::list<DelayedCall*>::iterator location;
	location = std::find_if(timer_list.begin(),
							timer_list.end(),
							DelayedCallComparator(timed_call));
	timer_list.erase(location);
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

bool DelayedCall::operator== (const DelayedCall& other)
{
	return this == &other;
}

bool DelayedCall::timedOut()
{
    timeval now;
    gettimeofday(&now, NULL);
    if (now.tv_sec > this->time.tv_sec ||
        (now.tv_sec == this->time.tv_sec &&
         now.tv_usec > this->time.tv_usec)
        )
    {
        return true;
    }
    return false;
}
