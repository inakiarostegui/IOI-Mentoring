/***************************************************************************//**
 * @filename DebugPrint.h
 *
 * @brief    Contains the debug_print function.
 *
 * @author   Inaki Arostegui 
 ******************************************************************************/

#pragma once
#include "pch.h"

#define DEBUG true

inline void debug_print(const std::string& str)
{
#if DEBUG
    std::cout << str << std::endl;
#endif
}