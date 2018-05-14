#ifndef SINGLETON_STATE_INCLUDED
#define SINGLETON_STATE_INCLUDED

#include "StateBase.h"

class SingletonState : public StateBase
{
public:
    SingletonState () = delete;
    explicit SingletonState (char c);

    SingletonState (const SingletonState&) = delete;
    void operator= (const SingletonState&) = delete;

    virtual ~SingletonState () = default;
    virtual void DeltaFunction (char c, Automata::StatePowerSet& res) const;
    virtual void EpsilonArrows (Automata::StatePowerSet& res) const;
    virtual StateBase* Clone () const;

private:
    char c;
};

#endif /* SINGLETON_STATE_INCLUDED */
