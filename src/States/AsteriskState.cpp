#include "ConcreteStates.h"

void AsteriskState::DeltaFunction (char c, Automata::StatePowerSet& res) const
{
    res.Join (this);
    res.Join (GetLink ());
    GetLink()->DeltaFunction (c, res);
}

void AsteriskState::EpsilonArrows (Automata::StatePowerSet& res) const
{
    res.Join (GetLink ());
}

StateBase* AsteriskState::Clone () const
{
    return new AsteriskState;
}
