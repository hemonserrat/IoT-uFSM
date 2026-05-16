#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <list>
#include "uEventHandler.hpp"
#include "uFsm.hpp"

#define MAXNUMTRANSITIONS   26
#define IDLE                0x0001
#define WAIT_DEV            0x0003
#define EV_RESET            0x0003
#define EV_TRANSFER         0x0004

enum Handler_indexes {
    Ind_InternalError,
    Ind_reset,
    Ind_transfer,
};

class EvHandler : public uEventHandler {
public:
    EvHandler(uint32_t inNumberTransitions) : uEventHandler(inNumberTransitions) { FillHandlersArray(); }
private:
    void FillHandlersArray(void) override {
        functions_[Ind_InternalError] = (TransitionFunc)&EvHandler::handleInternalError;
        functions_[Ind_reset] = (TransitionFunc)&EvHandler::reset;
        functions_[Ind_transfer] = (TransitionFunc)&EvHandler::transfer;
    }
    bool reset(void*) { return true; }
    bool transfer(void*) { return true; }
    bool handleInternalError(void*) { return false; }
};

TEST_CASE("FSM handles internal error", "[fsm]") {
    EvHandler evHandler(MAXNUMTRANSITIONS);
    uFsm eFsm(&evHandler, MAXNUMTRANSITIONS, IDLE);
    eFsm.defineTransition(IDLE, IDLE, INTERNAL_ERROR, Ind_InternalError);
    REQUIRE(eFsm.control(INTERNAL_ERROR, nullptr) == false);
}

TEST_CASE("FSM handles reset", "[fsm]") {
    EvHandler evHandler(MAXNUMTRANSITIONS);
    uFsm eFsm(&evHandler, MAXNUMTRANSITIONS, IDLE);
    eFsm.defineTransition(IDLE, WAIT_DEV, EV_RESET, Ind_reset);
    REQUIRE(eFsm.control(EV_RESET, nullptr) == true);
}

TEST_CASE("FSM handles transfer", "[fsm]") {
    EvHandler evHandler(MAXNUMTRANSITIONS);
    uFsm eFsm(&evHandler, MAXNUMTRANSITIONS, IDLE);
    eFsm.defineTransition(IDLE, WAIT_DEV, EV_TRANSFER, Ind_transfer);
    REQUIRE(eFsm.control(EV_TRANSFER, nullptr) == true);
}
