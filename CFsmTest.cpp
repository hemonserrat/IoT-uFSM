/**
 * CFsmTest.c
 *
 * This file is part of IoT u-Micro Finite State Machine library
 *
 * Copyright (C) 2012,  Hernan Monserrat hemonserrat<at>gmail<dot>com
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
#include <cstdio>
#include <cassert>
#include <list>
#include "uEventHandler.hpp"
#include "uFsm.hpp"
#include <iostream>

#define MAXNUMTRANSITIONS   26

// States
#define	IDLE                0x0001
#define	WAIT_COM            0x0002
#define	WAIT_DEV            0x0003
#define	WAIT_CRL            0x0004
#define	WAIT_END            0x0005
#define	ERROR               0xFFFF

// Define list of possible events

// COM Events to CEN
#define	EV_RESET              0x0003    // R
#define	EV_TRANSFER           0x0004    //


// Event handlers indexes
enum Handler_indexes {
    Ind_InternalError,

// COM Events to CEN
    Ind_reset,
    Ind_transfer,
};

class EvHandler : public uEventHandler
{
public:
    EvHandler( uint32_t inNumberTransitions);

private:
    void FillHandlersArray( void ) override;
    bool reset( void* );
    bool transfer( void*);
    bool handleInternalError( void* );

};

EvHandler::EvHandler( uint32_t inNumberTransitions )
        : uEventHandler(inNumberTransitions)
{
    FillHandlersArray();
}


bool EvHandler::handleInternalError( void* )
{
    std::cout << "handleInternalError\n";
    return false;
}

bool EvHandler::reset( void* )
{
    std::cout << "reset\n";
    return true;
}

bool EvHandler::transfer( void* )
{
    std::cout << "transfer\n";
    return true;
}

void EvHandler::FillHandlersArray( void ) {
    functions_[Ind_InternalError]
            = (TransitionFunc) &EvHandler::handleInternalError;

// COM Events to DEV
    functions_[Ind_reset]
            = (TransitionFunc) &EvHandler::reset;
    functions_[Ind_transfer]
            = (TransitionFunc) &EvHandler::transfer;
}

int main(int argc, const char * argv[]) {
    EvHandler  	*evHandler;
    uFsm        *eFsm;

    std::cout << "Starting tests!\n";

    evHandler = new EvHandler( MAXNUMTRANSITIONS);
    assert(evHandler!=NULL);

    eFsm = new uFsm(evHandler, MAXNUMTRANSITIONS, IDLE );
    assert(eFsm!=NULL);

    eFsm->defineTransition( IDLE, IDLE, INTERNAL_ERROR ,Ind_InternalError);
    eFsm->defineTransition( IDLE, WAIT_DEV, EV_RESET, Ind_reset);
    eFsm->defineTransition( IDLE, WAIT_DEV, EV_TRANSFER, Ind_transfer);

    eFsm->control(INTERNAL_ERROR, NULL);
    eFsm->control(EV_RESET, NULL);
  //  eFsm->control(EV_TRANSFER, NULL);


    delete eFsm;
    delete evHandler;
    std::cout << "bye\n";

    return EXIT_SUCCESS;
}
