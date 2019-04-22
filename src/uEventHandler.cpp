/**
 * \file uEventHandler.cpp
 *
 * This file is part of IoT u-Micro Finite State Machine library
 *
 * \copyright (C) 2012,  Hernan Monserrat hemonserrat<at>gmail<dot>com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cstdint>
#include <cstdio>
#include <cassert>
#include <list>
#include "uEventHandler.hpp"
#include "uFsm.hpp"

/**
 * Creates dynamically the function pointers table
 * @param numberTransitions - number of transition between states
 */
uEventHandler::uEventHandler(uint32_t numberTransitions) : functions_(NULL)
{
    assert( numberTransitions!= 0);
		functions_ = new TransitionFunc[numberTransitions];
		assert( functions_!=NULL );
}

/**
 * Frees up the function pointers table
 */
uEventHandler::~uEventHandler(void)
{
	if( functions_!=NULL )
		delete[] functions_;
}
