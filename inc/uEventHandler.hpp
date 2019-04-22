/**
 * \file uEventHandler.hpp
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
#if !defined(_UEVENTHANDLER_H)
#define _UEVENTHANDLER_H

#define INTERNAL_ERROR		0		///< Internal error event

class uEventHandler;

typedef bool (uEventHandler::*TransitionFunc)(void*);


class uEventHandler
{
public:
	uEventHandler(void);
	uEventHandler(uint32_t numberTransitions);
	virtual ~uEventHandler(void);

protected:
	TransitionFunc *functions_;
	virtual void FillHandlersArray(void) = 0;

    friend class uFsm;
};

#endif // !defined(_UEVENTHANDLER_H)
