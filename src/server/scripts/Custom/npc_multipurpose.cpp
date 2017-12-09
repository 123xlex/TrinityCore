#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "Creature.h"
#include "GossipDef.h"

class npc_multi_purpose : public CreatureScript
{
    public:
        npc_multi_purpose() : CreatureScript("npc_multi_purpose") { }

        static bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetTeam() == ALLIANCE)
            {
                AddGossipItemFor(player, 3, "Learn quest spells", GOSSIP_SENDER_MAIN, 4000);
                AddGossipItemFor(player, 0, "Honor", GOSSIP_SENDER_MAIN, 5000);
                AddGossipItemFor(player, 0, "Raids", GOSSIP_SENDER_MAIN, 6000);
                AddGossipItemFor(player, 0, "New Boss Test Area", GOSSIP_SENDER_MAIN, 9000);
                AddGossipItemFor(player, 0, "Take me back!", GOSSIP_SENDER_MAIN, 9001);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }

            return true;

        }

        static bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            switch (action)
            {
            case 4000:
                CloseGossipMenuFor(player);
                player->LearnQuestRewardedSpells();
                break;
            case 5000:
                CloseGossipMenuFor(player);
                player->SetReputation(1119, 85000);
                break;
            case 6000:
                AddGossipItemFor(player, 0, "Naxxramas", GOSSIP_SENDER_MAIN, 6001);
                AddGossipItemFor(player, 0, "Obsidian Sanctum", GOSSIP_SENDER_MAIN, 6002);
                AddGossipItemFor(player, 0, "Eye of Eternity", GOSSIP_SENDER_MAIN, 6003);
                AddGossipItemFor(player, 0, "Uluduar", GOSSIP_SENDER_MAIN, 6004);
                AddGossipItemFor(player, 0, "Trial of the Crusader", GOSSIP_SENDER_MAIN, 6005);
                AddGossipItemFor(player, 0, "Icecrown Citadel", GOSSIP_SENDER_MAIN, 6006);
                AddGossipItemFor(player, 0, "Main Menu", GOSSIP_SENDER_MAIN, 6007);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 6001:
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 3668.71f, -1262.45f, 243.62f, 4.78f);
                break;
            case 6002:
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 3480.18f, 264.90f, -120.14f, 3.23f);
                break;
            case 6003:
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 3878.62f, 6979.75f, 152.04f, 2.66f);
                break;
            case 6004:
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 9331.47f, -1114.27f, 1245.14f, 6.28f);
                break;
            case 6005:
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 8515.43f, 723.12f, 558.24f, 1.57f);
                break;
            case 6006:
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 5798.85f, 2075.67f, 636.06f, 3.57f);
                break;
            case 6007:
                if (player->GetTeam() == ALLIANCE)
                {
                    AddGossipItemFor(player, 3, "Learn quest spells", GOSSIP_SENDER_MAIN, 4000);
                    AddGossipItemFor(player, 0, "Honor", GOSSIP_SENDER_MAIN, 5000);
                    AddGossipItemFor(player, 0, "Raids", GOSSIP_SENDER_MAIN, 6000);
                    AddGossipItemFor(player, 0, "New Boss Test Area", GOSSIP_SENDER_MAIN, 9000);
                    AddGossipItemFor(player, 0, "Take me back!", GOSSIP_SENDER_MAIN, 9001);
                    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                }
                break;
            case 9000:
                CloseGossipMenuFor(player);
                if (player->IsGameMaster())
                {
                    player->TeleportTo(0, -784.09f, 1516.02f, 137.39f, 1.06f);
                }
                else
                    player->TeleportTo(0, -642.13f, 1301.57f, 121.01f, 1.79f);
                break;
            case 9001:
                CloseGossipMenuFor(player);
                player->TeleportTo(571, 5723.39f, 721.67f, 656.83f, 0.84f);
                break;
            }
            return true;
        }

        static bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code)
        {
            player->PlayerTalkClass->ClearMenus();
            return true;
        }

        struct npc_multi_purposeAI : public ScriptedAI
        {
            npc_multi_purposeAI(Creature* creature) : ScriptedAI(creature) { }
            
            bool GossipHello(Player* player) override
            {
                return OnGossipHello(player, me);
            }

            bool GossipSelect(Player* player, uint32 menuId, uint32 gossipListId) override
            {
                uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
                uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
                return OnGossipSelect(player, me, sender, action);
            }

            bool GossipSelectCode(Player* player, uint32 menuId, uint32 gossipListId, char const* code) override
            {
                uint32 sender = player->PlayerTalkClass->GetGossipOptionSender(gossipListId);
                uint32 action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
                return OnGossipSelectCode(player, me, sender, action, code);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_multi_purposeAI(creature);
        }
};

void AddSC_npc_multi_purpose()
{
    new npc_multi_purpose();
}