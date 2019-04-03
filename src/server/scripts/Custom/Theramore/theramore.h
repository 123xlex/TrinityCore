#ifndef DEF_THERAMORE_H
#define DEF_THERAMORE_H

#include "CreatureAIImpl.h"

#define TheramoreScriptName "instance_theramore"
#define DataHeader "TM"

uint32 const encounterCount = 2;

enum TMEncounters
{
    BOSS_ADMIRAL        = 0,
    BOSS_COUNCIL        = 1
};

enum TMDataTypes
{
    DATA_ADMIRAL        = 0,
    DATA_COUNCIL        = 1
};

enum TMCreatureIds
{
    NPC_ADMIRAL         = 99999
};

template <class AI, class T>
inline AI* GetTheramoreAI(T* obj)
{
    return GetInstanceAI<AI>(obj, TheramoreScriptName);
}

#endif
