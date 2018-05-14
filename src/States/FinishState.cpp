#include "ConcreteStates.h"

void FinishState::DeltaFunction (char, Automata::StatePowerSet&) const
{
}

void FinishState::EpsilonArrows (Automata::StatePowerSet&) const
{
}

StateBase* FinishState::Clone () const
{
    return new FinishState;
}
