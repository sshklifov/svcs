#include "ConcreteStates.h"

void QuestionMarkState::DeltaFunction (char, Automata::StatePowerSet& res) const
{
    res.Join (GetLink ());
    GetLink()->EpsilonArrows (res);
}

void QuestionMarkState::EpsilonArrows (Automata::StatePowerSet&) const
{
}

StateBase* QuestionMarkState::Clone () const
{
    return new QuestionMarkState;
}
