//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
/**
 * \file PerfTimer.cpp
 *
 * \date 2016-03-16
 * \author Mikko Mononen (modified by consultit)
 */

#include "PerfTimer.h"

#if defined(WIN32)

// Win32
#include <windows.h>

namespace rnsup
{
TimeVal getPerfTime()
{
	__int64 count;
	QueryPerformanceCounter((LARGE_INTEGER*)&count);
	return count;
}

int getPerfTimeUsec(const TimeVal duration)
{
	static __int64 freq = 0;
	if (freq == 0)
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	return (int)(duration*1000000 / freq);
}
}  // namespace rnsup

#else

// Linux, BSD, OSX

//#include <sys/time.h>

namespace rnsup
{
TimeVal getPerfTime()
{
	return 0;
}

int getPerfTimeUsec(const TimeVal duration)
{
	return (int)duration;
}
}  // namespace rnsup

#endif
