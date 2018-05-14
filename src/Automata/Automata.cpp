#include "Automata.h"
#include "ConcreteStates.h"
#include "WildcardParser.h"

#include <HashPointer.h>

#include <utility>
#include <cstring>

size_t Automata::StatePowerSetSurject::operator() (const StateBase* node) const
{
    return HashPointer (node);
}

Automata::Automata () : begin (NULL), states (0)
{
}

Automata::Automata (const char* s) : Automata ()
{
    AssignWildcard (s);
}

Automata::Automata (const Automata& rhs) : Automata ()
{
    CopyAssign (rhs);
}

Automata::Automata (Automata&& rhs) : Automata ()
{
    MoveAssign (rhs);
}

Automata& Automata::operator= (const Automata& rhs)
{
    if (this != &rhs)
    {
        CopyAssign (rhs);
    }
    return (*this);
}

Automata& Automata::operator= (Automata&& rhs)
{
    if (this != &rhs)
    {
        MoveAssign (rhs);
    }
    return (*this);
}

Automata::~Automata ()
{
    Clear ();
}

void Automata::AssignWildcard (const char* s)
{
    Automata tmp;
    tmp.begin = new StartState;
    tmp.states = 1;

    StateBase* tail = tmp.begin;
    unsigned offset = 0;
    while (s[offset])
    {
        unsigned n;
        tail->Attach (WildcardParser::GetInstance().Parse (s + offset, n));
        offset += n;

        tail = tail->GetLink ();
        ++tmp.states;
    }

    tail->Attach (new FinishState);
    ++tmp.states;

    (*this) = std::move (tmp);
}

bool Automata::Accepts (const char* s) const
{
    assert (s != NULL);
    return Accepts (s, strlen (s));
}

bool Automata::Accepts (const char* s, unsigned n) const
{
    StatePowerSet image = DeltaStar (s, n);
    for (StatePowerSet::ConstIterator it = image.CBegin (); it != image.CEnd (); ++it)
    {
        const FinishState* pFinish = dynamic_cast<const FinishState*> (*it);
        if (pFinish != NULL)
        {
            return true;
        }
    }
    return false;
}

bool Automata::HasPrefix (const char* s) const
{
    assert (s != NULL);
    return HasPrefix (s, strlen (s));
}

bool Automata::HasPrefix (const char* s, unsigned n) const
{
    StatePowerSet image = DeltaStar (s, n);
    return !image.Empty ();
}

Automata::StatePowerSet Automata::DeltaStar (const char* s, unsigned n) const
{
    StatePowerSet domain (states);
    StatePowerSet image (states);

    begin->EpsilonArrows (domain);

    for (unsigned i = 0; i<n && !domain.Empty (); ++i)
    {
        char c = s[i];
        for (StatePowerSet::ConstIterator it = domain.CBegin (); it != domain.CEnd (); ++it)
        {
            (*it)->DeltaFunction (c, image);
        }

        domain.Swap (image);
        image.Clear ();
    }

    return domain;
}

void Automata::Clear ()
{
    StateBase* head = begin;
    while (head != NULL)
    {
        StateBase* link = head->GetLink ();
        delete head;
        head = link;
    }
}

void Automata::CopyAssign (const Automata& rhs)
{
    Automata tmp;
    if (rhs.begin != NULL)
    {
        tmp.begin = rhs.begin->Clone();
        StateBase* tail = tmp.begin;

        const StateBase* rhsState = rhs.begin->GetLink ();
        while (rhsState != NULL)
        {
            tail->Attach (rhsState->Clone ());
            tail = tail->GetLink ();
            rhsState = rhsState->GetLink ();
        }
        tmp.states = rhs.states;
    }

    (*this) = std::move (tmp);
}

void Automata::MoveAssign (Automata& rhs)
{
    Clear ();
    begin = rhs.begin;
    states = rhs.states;

    rhs.begin = NULL;
}
