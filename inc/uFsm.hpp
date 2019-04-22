/**
 * \file uFsm.hpp
 *
 * \mainpage This file is part of IoT u-Micro Finite State Machine library
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
#if !defined(_UFSM_H)
#define _UFSM_H

/**
 * Each uFsmEvent instance represents an event.
 * Note event parameters is a pointer, it is 
 * programmer responsability to clean it.
 */
class uFsmEvent {
public:
    virtual ~uFsmEvent();

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
    uFsm( uEventHandler *trans, uint32_t maxNumTransitions, uint32_t initialState);
    virtual ~uFsm(void);

    uint32_t defineTransition(uint32_t sourceState, uint32_t destinationState,
                              uint32_t event, uint32_t index);
    bool control(uint32_t event, void *parameters = NULL);
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
