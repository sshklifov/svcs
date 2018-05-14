#ifndef SQUARE_BRACKETS_STATE_INCLUDED
#define SQUARE_BRACKETS_STATE_INCLUDED

#include "StateBase.h"
#include <BracketExpr.h>

class SquareBracketsState : public StateBase
{
public:
    SquareBracketsState () = delete;
    explicit SquareBracketsState (const BracketExpr& charSet);
    SquareBracketsState (const SquareBracketsState&) = delete;
    void operator= (const SquareBracketsState&) = delete;

    virtual ~SquareBracketsState () = default;
    virtual void DeltaFunction (char c, Automata::StatePowerSet& res) const;
    virtual void EpsilonArrows (Automata::StatePowerSet& res) const;
    virtual StateBase* Clone () const;

private:
    BracketExpr charSet;
};

#endif /* SQUARE_BRACKETS_STATE_INCLUDED */

