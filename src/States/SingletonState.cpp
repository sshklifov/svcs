#include "SingletonState.h"

SingletonState::SingletonState (char c) : c (c)
{
}

void SingletonState::DeltaFunction (char c, Automata::StatePowerSet& res) const
{
    if (this->c == c)
    {
        res.Join (GetLink ());
        GetLink()->EpsilonArrows (res);
    }
}
void SingletonState::EpsilonArrows (Automata::StatePowerSet&) const
{
}

StateBase* SingletonState::Clone () const
{
    return new SingletonState (c);
}
