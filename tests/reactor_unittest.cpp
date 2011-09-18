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

TEST(ReactorTest, run_and_stop)
{
	Reactor* reactor = Reactor::getInstance();
	EXPECT_FALSE(reactor->is_running());
	reactor->callLater(0, &is_running);
	EXPECT_FALSE(reactor->is_running());
	reactor->run();
	EXPECT_FALSE(reactor->is_running());
	EXPECT_TRUE(ran_callback);
}
