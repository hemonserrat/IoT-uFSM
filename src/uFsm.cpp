/**
 * \file uFsm.cpp
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
#include <cstdio>
#include <cassert>
#include <list>
#include <iostream>

#include "uEventHandler.hpp"
#include "uFsm.hpp"

/**
 * uFsm - Event based Finite State Machine
 * @param trans              - Event Handler instance
 * @param maxNumTransitions  - maximum allocation for transitions
 * @param initialState       - machine initial state.
 */
uFsm::uFsm(uEventHandler *trans, uint32_t maxNumTransitions, uint32_t initialState)
{
	assert( trans!=NULL );
   handler_ = trans;

 // Initialize the maximal number of transitions
  maxNumTransitions_ = maxNumTransitions;

 // Initialize the current state
 currentState_ = initialState;

 // Initialize the array of transitions
 arrayTrans_ = 0;
 arrayTrans_ = new TransitionType[ maxNumTransitions_ ];
 memset( arrayTrans_, -1, maxNumTransitions_ * sizeof(TransitionType));

}

/**
 * Used for debug proposes
 */
uFsmEvent::~uFsmEvent(void)
{
   std::cout << "bye, World!";
}

/**
 * Clean/deallocates transition array and Events in the queue.
 */
uFsm::~uFsm(void)
{
 if ( arrayTrans_ )
    delete []arrayTrans_;

    while (!Evlst_.empty())
    {
       uFsmEvent *pe=Evlst_.back();
        delete pe;
        Evlst_.pop_back();
    }
}

/**
 * Creates a new uFsmEvent object and inserts it into the back of the queue.
 * @param event
 * @param parameters
 */
void uFsm::insertInQueue( uint32_t event, void *parameters )
{
	uFsmEvent *newevent = new uFsmEvent( event, parameters );
    Evlst_.push_back(newevent);
}

/**
 * Compute an initial hash based on event and sourceState, finds and returns
 * the first available position based on that hash.
 * @param sourceState - source state
 * @param event       - trigger event
 * @return >0 on success, position index in the transition array.
 */
int32_t uFsm::hash( uint32_t sourceState, uint32_t event )
{
    int32_t  i=0;
    int32_t  where = ( ( event << 8 ) + sourceState) % maxNumTransitions_;

 // Look for the first available position
 while( (arrayTrans_[ (where + i ) % maxNumTransitions_].index != -1)
			 && (i < maxNumTransitions_) )
		i++;

 // Return negative value if the table is full
 if ( i >= maxNumTransitions_ )
    return -1;
 else  // Return free position�s index
    return ((where + i)%maxNumTransitions_) ;
}

/**
 * Compute an initial hash based on event and current state, finds and returns
 * the first available position based on that hash.
 * @param sourceState - source state
 * @param event       - trigger event
 * @return >0 on success, position index in the transition array.
 */
int32_t uFsm::hash( uint32_t event )
{
    int32_t  result = -1;
    int32_t  i = 0;
    int32_t  where = ( ( event << 8 ) + currentState_) % maxNumTransitions_;

 // Look for the first available position
 while( (arrayTrans_[(where+i) % maxNumTransitions_].source_state !=
                 currentState_  ||
         arrayTrans_[(where+i) % maxNumTransitions_].event != event)
		   && i < maxNumTransitions_ )
   	i++;

 // Return negative value if event not found
 result = (i >= maxNumTransitions_)?-1:((where + i) % maxNumTransitions_);

 return result;
}

/**
 * Defines a transition based on an Event
 * @param sourceState       - State "from"
 * @param destinationState  - State "to"
 * @param event             - happened Event
 * @param index             - function handler index
 * @return hash index assigned
 */
uint32_t uFsm::defineTransition( uint32_t sourceState, uint32_t destinationState,
                            uint32_t event, uint32_t index )
{
    uint32_t hashIndex;

 // Search free position in the table of transitions
    hashIndex = hash(sourceState, event);
 if ( hashIndex != -1 )
 {
     arrayTrans_[hashIndex].source_state		= sourceState;
     arrayTrans_[hashIndex].destination_state	= destinationState;
     arrayTrans_[hashIndex].event				= event;
     arrayTrans_[hashIndex].index				= index;
 }

 return hashIndex;
}

/**
 * Inserts an Event in the Event queue and then process it.
 * @param event      - new event Id to add
 * @param parameters - pointer to location with extra data to pass on event handling or NULL
 * @return  true/false from the event function handler.
 */
bool uFsm::control( uint32_t event, void *parameters )
{
    bool			result = false;
    int32_t		transition_num;

 // Insert the received event in the insertInQueue
 insertInQueue(event, parameters);

 while( !Evlst_.empty() )
 {
	uFsmEvent* pe = Evlst_.front();
	assert( pe!=NULL );
	// check if it is valid transition
    if ( (transition_num = hash(pe->id_)) >= 0 )
    {
       if ( arrayTrans_[transition_num].index == -1 )
       {
          // Missing transition - generate an internal error event
          generateEvent(INTERNAL_ERROR);
	   }
	   else
	   {
		  // Execute an event handler
		  result = (handler_->*handler_->functions_[
                                        arrayTrans_[transition_num].index])(pe->parameters_);

		  // Change the FSM's state
          currentState_ = arrayTrans_[transition_num].destination_state;
       }
    }

    // Missing transition - generate an internal error event
    else
    {
       assert( pe->id_ == INTERNAL_ERROR );
	   generateEvent(INTERNAL_ERROR);
    }

     Evlst_.pop_front();
	delete pe;
 } 

 return result;
}

/**
 * Add an Event to the Event queue
 * @param event      - Event type
 * @param parameters - data pointer to be accessed by the event handler function
 */
void uFsm::generateEvent( uint32_t event, void *parameters )
{
 insertInQueue(event, parameters);
}
