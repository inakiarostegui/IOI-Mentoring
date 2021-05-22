/***************************************************************************//**
 * @filename pch.h
 * @brief	 Precompile Header.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <cstddef>

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <utility>
#include <cassert>
#include <unordered_map>
#include <type_traits>
#include <span>

// Windows API
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "DebugPrint.h"
