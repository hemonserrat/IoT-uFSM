/**
 * \file uFsm.hpp
 *
 * \mainpage This file is part of IoT u-Micro Finite State Machine library
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
#if !defined(_UFSM_H)
#define _UFSM_H

#include <cstddef>
#include <cstdint>
#include <list>

#include "uEventHandler.hpp"

/**
 * Each uFsmEvent instance represents an event.
 * Note event parameters is a pointer, it is 
 * programmer responsability to clean it.
 */
class uFsmEvent {
public:
    /**
     * Destroy the event.
     */
    virtual ~uFsmEvent();

    /**
     * Create an event.
     *
     * \param id event identifier.
     * \param p optional event parameters owned by the caller.
     */
    uFsmEvent(int32_t id, void *p) {
        id_ = id;
        parameters_ = p;
    };
    int32_t id_;          ///< event identifier
    void *parameters_;    ///< event parameters
};

typedef std::list<uFsmEvent *> EvLst;

/**
 * Micro Event based Finite State Machine
 */
class uFsm {
public:
    /**
     * Create a finite state machine.
     *
     * \param trans event handler used to execute transitions.
     * \param maxNumTransitions maximum number of stored transitions.
     * \param initialState initial state identifier.
     */
    uFsm( uEventHandler *trans, uint32_t maxNumTransitions, uint32_t initialState);

    /**
     * Destroy the finite state machine and queued events.
     */
    virtual ~uFsm(void);

    /**
     * Define a state transition.
     *
     * \param sourceState transition source state.
     * \param destinationState transition destination state.
     * \param event event that triggers the transition.
     * \param index handler table index to call.
     * \return transition table index, or UINT32_MAX when the table is full.
     */
    uint32_t defineTransition(uint32_t sourceState, uint32_t destinationState,
                              uint32_t event, uint32_t index);

    /**
     * Queue and process an event.
     *
     * \param event event identifier.
     * \param parameters optional event parameters owned by the caller.
     * \return true when the event handler succeeds, false otherwise.
     */
    bool control(uint32_t event, void *parameters = NULL);

    /**
     * Add an event to the event queue.
     *
     * \param event event identifier.
     * \param parameters optional event parameters owned by the caller.
     */
    void generateEvent(uint32_t event, void *parameters = NULL);

private:

    void insertInQueue(uint32_t event, void *parameters);
    int32_t hash(uint32_t sourceState, uint32_t event);
    int32_t hash(uint32_t event);

    typedef struct {
        uint32_t source_state;
        uint32_t destination_state;
        uint32_t event;
        int32_t  index;
    } TransitionType;

    TransitionType *arrayTrans_; ///< a pointer to the array of transitions
    uint32_t maxNumTransitions_; ///< max transitions supported
    uint32_t currentState_;      ///< current state of the FSM
    uEventHandler *handler_;     ///< a pointer to the abstract server class
    EvLst Evlst_;
};

#endif
