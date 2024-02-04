#include "../../cs2/shared/shared.h"

#include <sys/time.h>
#include <linux/types.h>
#include <linux/input-event-codes.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <chrono>
#include <thread>

int main(void)
{

	while (1)
	{
		cs2::run();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		// std::cout << "Sleep" << std::endl;
	}

	return 0;
}

