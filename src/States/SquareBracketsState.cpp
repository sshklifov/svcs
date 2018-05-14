#include "SquareBracketsState.h"

SquareBracketsState::SquareBracketsState (const BracketExpr& charSet)
{
    CopyBracketExpr (this->charSet, charSet);
}

void SquareBracketsState::DeltaFunction (char c, Automata::StatePowerSet& res) const
{
    if (BracketExprContainsChar (charSet, c))
    {
        res.Join (GetLink ());
        GetLink()->EpsilonArrows (res);
    }
}

void SquareBracketsState::EpsilonArrows (Automata::StatePowerSet&) const
{
}

StateBase* SquareBracketsState::Clone () const
{
    return new SquareBracketsState (charSet);
}
