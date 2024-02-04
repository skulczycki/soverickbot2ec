#ifndef SHARED_H
#define SHARED_H

#ifdef _KERNEL_MODE
extern "C" int _fltused;
#endif

#include "cs.h"

//
// features.cpp
//
namespace features
{
	void run(void);
	void reset(void);
	void dump_player_positions(QWORD local_player, QWORD local_controller);
}


namespace cs2
{
	inline void run(void)
	{
		if (cs::running())
		{
			features::run();
		}
		else
		{
			features::reset();
		}
	}
}


#endif // SHARED_H

