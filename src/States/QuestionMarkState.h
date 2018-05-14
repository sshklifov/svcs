#ifndef QUESTION_MARK_STATE_INCLUDED
#define QUESTION_MARK_STATE_INCLUDED

#include "StateBase.h"

class QuestionMarkState : public StateBase
{
public:
    QuestionMarkState () = default;

    QuestionMarkState (const QuestionMarkState&) = delete;
    void operator= (const QuestionMarkState&) = delete;

    virtual ~QuestionMarkState () = default;
    virtual void DeltaFunction (char c, Automata::StatePowerSet& res) const;
    virtual void EpsilonArrows (Automata::StatePowerSet& res) const;
    virtual StateBase* Clone () const;
};


#endif /* QUESTION_MARK_STATE_INCLUDED */
