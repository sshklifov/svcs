#ifndef START_STATE_INCLUDED
#define START_STATE_INCLUDED

#include "StateBase.h"

class StartState : public StateBase
{
public:
    StartState () = default;

    StartState (const StartState&) = delete;
    void operator= (const StartState&) = delete;

    virtual ~StartState () = default;
    virtual void DeltaFunction (char c, Automata::StatePowerSet& res) const;
    virtual void EpsilonArrows (Automata::StatePowerSet& res) const;
    virtual StateBase* Clone () const;
};

#endif /* START_STATE_INCLUDED */
