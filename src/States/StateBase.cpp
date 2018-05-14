#include "StateBase.h"

void StateBase::Attach (StateBase* state)
{
    assert (state != NULL);
    link = state;
}

const StateBase* StateBase::GetLink () const
{
    return link;
}

StateBase* StateBase::GetLink ()
{
    return link;
}
