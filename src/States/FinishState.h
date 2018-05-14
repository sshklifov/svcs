#ifndef FINISH_STATE_INCLUDED
#define FINISH_STATE_INCLUDED

#include "StateBase.h"

class FinishState : public StateBase
{
public:
    FinishState () = default;

    FinishState (const FinishState&) = delete;
    void operator= (const FinishState&) = delete;

    virtual ~FinishState () = default;
    virtual void DeltaFunction (char c, Automata::StatePowerSet& res) const;
    virtual void EpsilonArrows (Automata::StatePowerSet& res) const;
    virtual StateBase* Clone () const;
};

#endif /* FINISH_STATE_INCLUDED */
