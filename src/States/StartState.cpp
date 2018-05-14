#include "ConcreteStates.h"

void StartState::DeltaFunction (char c, Automata::StatePowerSet& res) const
{
    GetLink()->DeltaFunction (c, res);
}

void StartState::EpsilonArrows (Automata::StatePowerSet& res) const
{
    res.Join (GetLink ());
}

StateBase* StartState::Clone () const
{
    return new StartState;
}
