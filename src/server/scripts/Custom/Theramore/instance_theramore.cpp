#include "ScriptMgr.h"
#include "Creature.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Map.h"
#include "theramore.h"

class instance_theramore : public InstanceMapScript
{
    public: instance_theramore(): InstanceMapScript(TheramoreScriptName, 803) { }

        struct instance_theramore_InstanceMapScript : public InstanceScript
        {
            instance_theramore_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(encounterCount);
            }

            bool IsEncounterInProgress() const override
            {
                return true;
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch (creature->GetEntry())
                {
                case NPC_ADMIRAL:
                    _admiralGUID = creature->GetGUID();
                    break;
                default:
                    break;
                }
            }

            ObjectGuid GetGuidData(uint32 uiData) const override
            {
                switch (uiData)
                {
                case DATA_ADMIRAL:
                    return _admiralGUID;
                    break;
                case DATA_COUNCIL:
                    return _councilGUID;
                    break;
                default:
                    break;
                }
                return ObjectGuid::Empty;
            }

        private:

            ObjectGuid _admiralGUID;
            ObjectGuid _councilGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_theramore_InstanceMapScript(map);
        }
};

void AddSC_instance_theramore()
{
    new instance_theramore();
}
