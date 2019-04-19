#include "CommonHelpers.h"
#include "Containers.h"
#include "GridNotifiers.h"
#include "InstanceScript.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "theramore.h"

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
    SPELL_ICE_TOMB = 69712,

    SPELL_ROOT = 42716,
    SPELL_ICE_TOMB_DAMAGE = 70157,
    SPELL_ICE_TOMB_DUMMY = 69675,
    SPELL_ICE_TOMB_UNTARGETABLE = 69700

};

enum Events
{
    // Admiral Proudmoore
    EVENT_FROSTBOLT_VOLLEY = 1,
    EVENT_LIGHTNING_NOVA,
    EVENT_COLUMN_OF_FROST,
    EVENT_SUMMON_CYCLONE,
    EVENT_SUMMON_ADDS,

    // Caster Add
    EVENT_SUMMON_MINIWATER,
    EVENT_FROSTBOLT,

    // Melee Add
    EVENT_ENRAGE,

    // Water Ele
    EVENT_WATER_WAVES,
    EVENT_ICE_TOMB_REMOVAL
};

enum Phases
{
    PHASE_ONE = 1,
    PHASE_TRANSITION_1,
    PHASE_TWO,
    PHASE_TRANSITION_2,
    PHASE_THREE,
    PHASE_ALL
};

Position const tideStoneSpawn[3] =
{
    {10.0f, 20.0f, 30.0f, 40.0f},
    {10.0f, 20.0f, 30.0f, 40.0f},
    {10.0f, 20.0f, 30.0f, 40.0f},
};

Position const centerPlatform  = { 327.830627f, 540.413696f, 25.454988f, 3.125883f };
Position const hoverPoint      = { 327.830627f, 540.413696f, 35.454988f, 3.125883f };

class boss_admiral_proudmoore : public CreatureScript
{
public:
        boss_admiral_proudmoore() : CreatureScript("boss_admiral_proudmoore") { }

        struct boss_admiral_proudmooreAI : public BossAI
        {
            boss_admiral_proudmooreAI(Creature* creature) : BossAI(creature, BOSS_ADMIRAL)
            {

            }

            void InitializeAI() override
            {
                
            }

            void Reset() override
            {
                _Reset();
                tideStoneCount = 0;
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ICE_TOMB_DAMAGE);
                me->SetReactState(REACT_DEFENSIVE);
                me->SetSpeed(MOVE_RUN, 5.0f);
                /*
                std::list<Player*> player;
                me->GetPlayerListInGrid(player, 300.0f);
                for each (Player* itr in player)
                {
                    itr->RemoveAura(SPELL_ICE_TOMB_DAMAGE);
                }
                */
            }

            void TransitionPhase(uint8 Phase)
            {
                me->Yell("Just reached Transition Phase", LANG_UNIVERSAL, NULL);
                std::list<Unit*> targets;
                SelectTargetList(targets, 10, SELECT_TARGET_RANDOM, 0.0f, 0.0f, true, true, -SPELL_ICE_TOMB_DAMAGE);
                {
                    me->Yell("Created the List", LANG_UNIVERSAL, NULL);
                    if (!targets.empty())
                    {
                        me->Yell("Found entries in the list", LANG_UNIVERSAL, NULL);
                        for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); itr++)
                        {
                            me->Yell("Adding Root to player number", LANG_UNIVERSAL, NULL);
                            DoCast(*itr, SPELL_ICE_TOMB_DAMAGE);
                        }
                    }
                }

                switch (Phase)
                {
                    case PHASE_ONE:
                        me->SetReactState(REACT_PASSIVE);
                        me->SetSpeedRate(MOVE_RUN, 3.0f);
                        me->NearTeleportTo(centerPlatform);
                        me->SetFacingTo(3.132854f);
                        setPhase(PHASE_TRANSITION_1);
                        events.ScheduleEvent(EVENT_ICE_TOMB_REMOVAL, 5s, 0, PHASE_TRANSITION_1);
                        break;
                    case PHASE_TWO:
                        me->SetReactState(REACT_PASSIVE);
                        me->SetSpeedRate(MOVE_RUN, 3.0f);
                        me->NearTeleportTo(centerPlatform);
                        me->SetFacingTo(3.132854f);
                        setPhase(PHASE_TRANSITION_2);
                        events.ScheduleEvent(EVENT_ICE_TOMB_REMOVAL, 5s, 0, PHASE_TRANSITION_2);
                        break;
                    default:
                        break;
                }
            }

            void JustEngagedWith(Unit* /*who*/) override
            {
                _JustEngagedWith();
                events.ScheduleEvent(EVENT_ENRAGE, 5s, 0, PHASE_ONE);
                events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 10s, 0, PHASE_ONE);
                setPhase(PHASE_ONE);
                DoZoneInCombat(me);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
            {
                if (events.IsInPhase(PHASE_ONE) && HealthBelowPct(66))
                {
                    TransitionPhase(PHASE_ONE);
                    me->Yell("Setting Phase to Phase 2", LANG_UNIVERSAL, NULL);
                }

                if (events.IsInPhase(PHASE_TWO) && HealthBelowPct(33))
                {
                    TransitionPhase(PHASE_TWO);
                    me->Yell("Just reached Phase 3", LANG_UNIVERSAL, NULL);
                }
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventID = events.ExecuteEvent())
                {
                    switch (eventID)
                    {
                    case EVENT_FROSTBOLT_VOLLEY:
                        DoCastAOE(SPELL_FROSTBOLT_VOLLEY);
                        events.RescheduleEvent(EVENT_FROSTBOLT_VOLLEY, Seconds(5), Seconds(10), 0, PHASE_ONE);
                        break;
                    case EVENT_ICE_TOMB_REMOVAL:
                        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ICE_TOMB_DAMAGE);
                        setPhase(PHASE_TWO);
                        break;
                    case EVENT_ENRAGE:
                        me->Yell("Just got this string", LANG_UNIVERSAL, NULL);
                        break;
                    default:
                        break;
                    }

                }

                DoMeleeAttackIfReady();
            }
        private:
            void setPhase(Phases newPhase)
            {
                _phase = newPhase;
                events.SetPhase(newPhase);
            }

            uint8 tideStoneCount;
            Phases _phase;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetTheramoreAI<boss_admiral_proudmooreAI>(creature);
        }
};

void AddSC_boss_admiral_proudmoore()
{
    new boss_admiral_proudmoore();
}

/*
class boss_admiral_proudmoore : public CreatureScript
{
    public: boss_admiral_proudmoore() : CreatureScript("boss_admiral_proudmoore") { }

        struct boss_admiral_proudmooreAI : public ScriptedAI
        {
            boss_admiral_proudmooreAI(Creature* creature) : ScriptedAI(creature)
            {
                Initialize();                
            }

            void Initialize()
            {
            }

            void Reset() override
            {
                Initialize();
            }

            void EnterCombat(Unit*)
            {
                me->Yell("Combat start", LANG_UNIVERSAL, NULL);
                events->SetPhase(PHASE_ONE);
                events->ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 5000);
                events->ScheduleEvent(EVENT_SUMMON_ADDS, 10000, 0, PHASE_ONE);
                World* map;
                map->NewMaxVisibleDistanceInInstances(10.0f);
            }

            void KilledUnit(Unit*) override
            {
            }

            void JustDied(Unit*) override
            {
                World* map;
                map->GetMaxVisibleDistanceInInstances();
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events->Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (events->IsInPhase(PHASE_ONE) && HealthBelowPct(65))
                {
                    events->SetPhase(PHASE_TWO);
                    events.ScheduleEvent(EVENT_WATER_WAVES, 7500, 0, PHASE_TWO);
                    events.ScheduleEvent(EVENT_COLUMN_OF_FROST, urand(10000, 12500), 0, PHASE_TWO);
                    me->SummonCreature(NPC_WATER_ELEMENTAL, me->GetHomePosition(), TEMPSUMMON_CORPSE_DESPAWN);
                    me->Yell("First Phase completed.", LANG_UNIVERSAL, NULL);
                    me->Yell("Hello", LANG_UNIVERSAL, NULL);
                }

                if (events->IsInPhase(PHASE_TWO) && HealthBelowPct(30))
                {
                    events->SetPhase(PHASE_THREE);
                    events->ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 5000, 0, PHASE_THREE);
                    events->ScheduleEvent(EVENT_SUMMON_CYCLONE, 13000, 0, PHASE_THREE);
                    me->Yell("I've reached my final form!", LANG_UNIVERSAL, NULL);
                }

                while (uint32 eventId = events->ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_FROSTBOLT_VOLLEY:
                        DoCastAOE(SPELL_FROSTBOLT_VOLLEY);
                        events->ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, urand(12500, 17000));
                        break;
                    case EVENT_SUMMON_ADDS:
                        me->SummonCreature(NPC_CASTER_ADD, me->GetPositionX(), me->GetPositionY() + 10.0f, me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_CORPSE_DESPAWN);
                        me->SummonCreature(NPC_MELEE_ADD, me->GetPositionX(), me->GetPositionY() - 10.0f, me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_CORPSE_DESPAWN);
                        events->ScheduleEvent(EVENT_SUMMON_ADDS, 25000, 0, PHASE_ONE);
                        break;
                    case EVENT_SUMMON_CYCLONE:
                        break;
                    default:
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap* events;
            uint8 tideStoneCount;
            uint8 platformGrid[9];
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

*/
