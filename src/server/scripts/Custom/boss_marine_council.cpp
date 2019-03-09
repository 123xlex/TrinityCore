/*
	siegemaster:
    inactive: bombards one part of the area( announce with flare) - constant spawn points (quadrants) spawn visuals around there, invis stalkers do explosions - randomize which quadrants
	active: snipe a target/flamethrower(cone if front of boss)
	
    infantry:
    inactive: spear throw(ticking debuff on random target)
    active: fighting skills(reflects casts or physical dmg depending on stance) / leap on target (aoe dmg on impact / some kind of announcement targetwise)
	
    mage:
    inactive: polymorph(dispell)
    active:	kamehameha (targets one player have to face it into one direction) / shield needs to be destroyed before certain timer else big dmges
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"
#include "MotionMaster.h"
#include "Spell.h"
#include "SpellMgr.h"
#include "SpellAuras.h"

enum Creatures
{
    NPC_SIEGEMASTER_COUNCIL     = 99991,
    NPC_INFANTRY_COUNCIL        = 99992,
    NPC_MAGE_COUNCIL            = 99993
};

enum Spells
{
    // Siegemaster
    SPELL_SNIPE                 = 12345,
    SPELL_FLAMETHROW            = 67890,
    SPELL_BOMBARDEMENT          = 23456,
    
    // Infantry
    SPELL_SPEARTHROW            = 78901,
    SPELL_MAGICRSTANCE          = 34567,
    SPELL_ARMORSTANCE           = 89012,
    SPELL_REFLECT               = 45678,
    SPELL_LEAP                  = 90123,

    // Mage
    SPELL_POLYMOPRH             = 56789,
    SPELL_BEAM                  = 01234,
    SPELL_SHIELD                = 76890,
    
    // Shared
    SPELL_BERSERK               = 99999
};

enum Events
{
    EVENT_SNIPE             = 1,
    EVENT_FLAMETHROW        = 2,
    EVENT_BOMBARDEMENT      = 3,
    EVENT_SPEARTHROW        = 4,
    EVENT_MAGICSTANCE       = 5,
    EVENT_ARMORSTANCE       = 6,
    EVENT_REFLECT           = 7,
    EVENT_LEAP              = 8,
    EVENT_POLYMORPH         = 9,
    EVENT_BEAM              = 10,
    EVENT_SHIELD            = 11,
    EVENT_BERSERK           = 12
};

