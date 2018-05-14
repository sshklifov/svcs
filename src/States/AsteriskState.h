#ifndef ASTERISK_STATE_INCLUDED
#define ASTERISK_STATE_INCLUDED

#include "StateBase.h"

class AsteriskState : public StateBase
{
public:
    AsteriskState () = default;

    AsteriskState (const AsteriskState&) = delete;
    void operator= (const AsteriskState&) = delete;

    virtual ~AsteriskState () = default;
    virtual void DeltaFunction (char c, Automata::StatePowerSet& res) const;
    virtual void EpsilonArrows (Automata::StatePowerSet& res) const;
    virtual StateBase* Clone () const;
};

#endif /* ASTERISK_STATE_INCLUDED */
