#pragma once
#ifndef _PROFILER_H
#define _PROFILER_H

#include <chrono>
#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <ratio>
#include <utility>

#ifdef WIN32;
#include <Windows.h>
#include <Psapi.h>
#pragma comment (lib,"psapi.lib")
#else
#include <sys/resource>
#include <sys/utime.h>
#endif



#endif // !_PROFILER_H