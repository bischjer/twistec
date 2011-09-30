#include <limits.h>
#include "reactor.hpp"
#include "gtest/gtest.h"

bool ran_callback = false;

void is_running(void)
{
	ran_callback = true;
	Reactor* reactor = Reactor::getInstance();
	EXPECT_TRUE(reactor->is_running());
	reactor->stop();
}

void is_running2(void)
{
	ran_callback = true;
	EXPECT_FALSE(ran_callback);
}

TEST(ReactorTest, run_and_stop)
{
	ran_callback = false;
	Reactor* reactor = Reactor::getInstance();
	EXPECT_FALSE(reactor->is_running());
	reactor->callLater(0, &is_running);
	EXPECT_FALSE(reactor->is_running());
	reactor->run();
	EXPECT_FALSE(reactor->is_running());
	EXPECT_TRUE(ran_callback);
}

TEST(ReactorTest, remove_delayed_call)
{
	ran_callback = false;
	Reactor* reactor = Reactor::getInstance();
	DelayedCall* delayed_call = reactor->callLater(0, &is_running2);
    reactor->removeTimedCall(delayed_call);
    reactor->run_timers();
	EXPECT_FALSE(ran_callback);
}
