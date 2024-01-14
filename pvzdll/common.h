#pragma once

#define WIN32_LEAN_AND_MEAN

#include "minhook/MinHook.h"
#include "cstdlib"
#include "vector"
#include "unordered_map"
#include "map"
#include "atomic"
#include "iostream"
#include "algorithm"

#define EXPORT extern "C" __declspec(dllexport)