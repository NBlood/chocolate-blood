/*
 * Copyright (C) 2018, 2022 nukeykt
 *
 * This file is part of Blood-RE.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _DEBUG4G_H_
#define _DEBUG4G_H_

#include "typedefs.h"

void __dassert(const char*, const char*, int);

//#define dassert(x) if (!(x)) __dassert(#x,__FILE__,__LINE__)
#define dassert(x, line) if (!(x)) __dassert(#x,__FILE__,line)

#endif
