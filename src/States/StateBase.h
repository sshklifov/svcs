#ifndef STATE_BASE_INCLUDED
#define STATE_BASE_INCLUDED

#include <Automata.h>

class StateBase
{
public:
    virtual void DeltaFunction (char c, Automata::StatePowerSet& res) const = 0;
    virtual void EpsilonArrows (Automata::StatePowerSet& res) const = 0;
    virtual StateBase* Clone () const = 0;
    virtual ~StateBase () = default;

    virtual void Attach (StateBase* state) final;
    virtual const StateBase* GetLink () const final;
    virtual StateBase* GetLink () final;

private:
    StateBase* link = NULL;
};

#endif /* STATE_BASE_INCLUDED */
