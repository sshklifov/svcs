#ifndef AUTOMATA_INCLUDED
#define AUTOMATA_INCLUDED

#include <Set.h>

class StateBase;

class Automata
{
private:
    struct StatePowerSetSurject;

public:
    using StatePowerSet = Set<const StateBase*,StatePowerSetSurject>;

public:
    Automata ();
    Automata (const char* s);

    Automata (const Automata& rhs);
    Automata (Automata&& rhs);
    Automata& operator= (const Automata& rhs);
    Automata& operator= (Automata&& rhs);

    ~Automata ();

    void AssignWildcard (const char* s);

    bool Accepts (const char* s) const;
    bool Accepts (const char* s, unsigned n) const;
    bool HasPrefix (const char* s) const;
    bool HasPrefix (const char* s, unsigned n) const;

private:
    void CopyAssign (const Automata& rhs);
    void MoveAssign (Automata& rhs);
    void Clear ();

    StatePowerSet DeltaStar (const char* s, unsigned n) const;

private:
    StateBase* begin;
    unsigned states;
};

struct Automata::StatePowerSetSurject
{
    size_t operator() (const StateBase* node) const;
};

#include <StateBase.h>

#endif /* AUTOMATA_INCLUDED */
