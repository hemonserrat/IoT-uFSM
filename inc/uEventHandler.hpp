/**
 * \file uEventHandler.hpp
 *
 * This file is part of IoT u-Micro Finite State Machine library
 *
 * \copyright (C) 2012,  Hernan Monserrat 
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
#if !defined(_UEVENTHANDLER_H)
#define _UEVENTHANDLER_H

#include <cstdint>

#define INTERNAL_ERROR		0		///< Internal error event

class uEventHandler;

typedef bool (uEventHandler::*TransitionFunc)(void*);

/**
 * Base class for event handler implementations.
 */
class uEventHandler
{
public:
	uEventHandler(void);

	/**
	 * Create a handler table.
	 *
	 * \param numberTransitions number of transition functions to allocate.
	 */
	uEventHandler(uint32_t numberTransitions);

	/**
	 * Destroy the handler table.
	 */
	virtual ~uEventHandler(void);

protected:
	TransitionFunc *functions_;

	/**
	 * Fill the handler function table.
	 */
	virtual void FillHandlersArray(void) = 0;

    friend class uFsm;
};

#endif // !defined(_UEVENTHANDLER_H)
