#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Unit.h"

enum Npcs
{
    NPC_CASTER_ADD = 99998,
    NPC_MELEE_ADD = 99997,
    NPC_WATER_ELEMENTAL = 99995,
    NPC_MINI_WATER_ELEMENTAL = 45678,
};

enum Spells
{
    // Admiral Proudmoore
    SPELL_FROSTBOLT_VOLLEY = 72905,
    SPELL_LIGHTNING_NOVA = 23456,
    SPELL_COLUMN_OF_FROST = 56789,
    SPELL_WATER_CYCLONE = 38497,

    // Caster Add
    SPELL_FROSTBOLT = 34567,

    // Melee Add
    SPELL_ENRAGE = 45678,

    // Water Ele
    SPELL_WATER_WAVES = 67890,
    SPELL_ICE_TOMB = 56789,

};

enum Events
{
    // Admiral Proudmoore
    EVENT_FROSTBOLT_VOLLEY = 1,
    EVENT_LIGHTNING_NOVA = 2,
    EVENT_COLUMN_OF_FROST = 3,
    EVENT_SUMMON_CYCLONE = 4,
    EVENT_SUMMON_ADDS = 5,

    // Caster Add
    EVENT_SUMMON_MINIWATER = 6,
    EVENT_FROSTBOLT = 7,

    // Melee Add
    EVENT_ENRAGE = 8,

    // Water Ele
    EVENT_WATER_WAVES = 9,
    EVENT_ICE_TOMB = 10,

};

enum Phases
{
    PHASE_ONE = 1,
    PHASE_TWO = 2,
    PHASE_THREE = 3,
};

class boss_admiral_proudmoore : public CreatureScript
{
public:
    boss_admiral_proudmoore() : CreatureScript("boss_admiral_proudmoore") { }

    struct boss_admiral_proudmooreAI : public BossAI
    {
        boss_admiral_proudmooreAI(Creature* creature) : BossAI(creature, 0)
        {
        }

        void Reset() override
        {
            _Reset();
        }

        void EnterCombat(Unit*) override
        {
            events.SetPhase(PHASE_ONE);
            events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 5000);
            events.ScheduleEvent(EVENT_SUMMON_ADDS, 10000, 0, PHASE_ONE);
        }

        void KilledUnit(Unit*) override
        {
        }

        void JustDied(Unit*) override
        {
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (events.IsInPhase(PHASE_ONE) && HealthBelowPct(65))
            {
                events.SetPhase(PHASE_TWO);
                /*
                events.ScheduleEvent(EVENT_WATER_WAVES, 7500, 0, PHASE_TWO);
                events.ScheduleEvent(EVENT_COLUMN_OF_FROST, urand(10000, 12500), 0, PHASE_TWO);
                */
                me->SummonCreature(NPC_WATER_ELEMENTAL, me->GetHomePosition(), TEMPSUMMON_CORPSE_DESPAWN);
                me->Yell("First Phase completed.", LANG_UNIVERSAL, NULL);
                me->Yell("Hello", LANG_UNIVERSAL, NULL);
            }

            if (events.IsInPhase(PHASE_TWO) && HealthBelowPct(30))
            {
                events.SetPhase(PHASE_THREE);
                events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 5000, 0, PHASE_THREE);
                events.ScheduleEvent(EVENT_SUMMON_CYCLONE, 13000, 0, PHASE_THREE);
                me->Yell("I've reached my final form!", LANG_UNIVERSAL, NULL);
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FROSTBOLT_VOLLEY:
                    DoCastAOE(SPELL_FROSTBOLT_VOLLEY);
                    events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, urand(12500, 17000));
                    break;
                case EVENT_SUMMON_ADDS:
                    me->SummonCreature(NPC_CASTER_ADD, me->GetPositionX(), me->GetPositionY() + 10.0f, me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_CORPSE_DESPAWN);
                    me->SummonCreature(NPC_MELEE_ADD, me->GetPositionX(), me->GetPositionY() - 10.0f, me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_CORPSE_DESPAWN);
                    events.ScheduleEvent(EVENT_SUMMON_ADDS, 25000, 0, PHASE_ONE);
                    break;
                case EVENT_SUMMON_CYCLONE:
                    break;
                default:
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_admiral_proudmooreAI(creature);
    }

};

void AddSC_admiral_proudmoore()
{
    new boss_admiral_proudmoore();
}
