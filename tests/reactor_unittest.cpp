#include <limits.h>
#include "reactor.hpp"
#include "gtest/gtest.h"

void is_running(void)
{
	Reactor* reactor = Reactor::getInstance();
	EXPECT_TRUE(reactor->is_running());
	reactor->stop();
}

TEST(ReactorTest, run_and_stop) {
	Reactor* reactor = Reactor::getInstance();
	reactor->callLater(0, &is_running);
	reactor->run();
	EXPECT_FALSE(reactor->is_running());
}
