/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "CreatureGroups.h"
#include "CreatureScript.h"
#include "GameEventMgr.h"
#include "ObjectAccessor.h"
#include "ScriptedCreature.h"

enum COG_Paths
{
    STORMWIND_PATH = 80500,
    GOLDSHIRE_PATH = 80501,
    WOODS_PATH = 80502,
    HOUSE_PATH = 80503,
    LISA_PATH = 80700
};

enum COG_Sounds
{
    BANSHEE_DEATH = 1171,
    BANSHEEPREAGGRO = 1172,
    CTHUN_YOU_WILL_DIE = 8585,
    CTHUN_DEATH_IS_CLOSE = 8580,
    HUMAN_FEMALE_EMOTE_CRY = 6916,
    GHOSTDEATH = 3416
};

enum COG_Creatures
{
    NPC_DANA = 804,
    NPC_CAMERON = 805,
    NPC_JOHN = 806,
    NPC_LISA = 807,
    NPC_AARON = 810,
    NPC_JOSE = 811
};

enum COG_Events
{
    EVENT_WP_START_GOLDSHIRE = 1,
    EVENT_WP_START_WOODS = 2,
    EVENT_WP_START_HOUSE = 3,
    EVENT_WP_START_LISA = 4,
    EVENT_PLAY_SOUNDS = 5,
    EVENT_BEGIN_EVENT = 6,
    EVENT_RANDOM_MOVEMENT = 7
};

enum COG_GameEvent
{
    GAME_EVENT_CHILDREN_OF_GOLDSHIRE = 74
};

class npc_catchup_leveler : public CreatureScript
{
public:
    npc_catchup_leveler() : CreatureScript("npc_catchup_leveler") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);
        if (action == 1001)
        {
            QueryResult result = CharacterDatabase.Query("SELECT MAX(characters.level) FROM `characters` WHERE characters.account = {}", player->GetSession()->GetAccountId());
            if (result)
            {
                Field* fields = result->Fetch();
                uint32 minmax = fields[0].Get<uint32>();
                if (minmax < 60)
                {
                    creature->Whisper("You must have a level 60 character on your account to use the boost!", LANG_UNIVERSAL, player, false);
                    CloseGossipMenuFor(player);
                    return false;
                }
            }
            player->GiveLevel(58);
            player->StoreNewItemInBestSlots(5763, 4); // 8 slot bag.
            player->AddItem(13935, 20); // Baked Salmon
            player->AddItem(38429, 20); // Blackrock Spring Water

            player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 12001, true);
            player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 12001, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 1404, true);
            player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 5079, true);
            player->EquipNewItem(EQUIPMENT_SLOT_NECK, 12032, true);

            //Flight Paths
            if (player->GetTeamId() == TEAM_HORDE)
            {
            }

            //Cloth
            if (player->getClass() == CLASS_MAGE || player->getClass() == CLASS_WARLOCK || player->getClass() == CLASS_PRIEST)
            {
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 8115, true);
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 13868, true);
                player->EquipNewItem(EQUIPMENT_SLOT_FEET, 10039, true);
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 13870, true);
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 14444, true);
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 14296, true);
                player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 8253, true);
                player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 14448, true);
                player->StoreNewItemInBestSlots(9482, 1); //weapon
            }
            if (player->getClass() == CLASS_DRUID || player->getClass() == CLASS_ROGUE) // Leather
            {
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 15185, true);
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 10105, true);
                player->EquipNewItem(EQUIPMENT_SLOT_FEET, 15426, true);
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 20661, true);
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 10112, true);
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 15432, true);
                player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 10145, true);
                player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 10147, true);
                if (player->getClass() == CLASS_ROGUE)
                {
                    player->StoreNewItemInBestSlots(15783, 2);
                }
                else
                    player->StoreNewItemInBestSlots(15278, 1);
            }
            if (player->getClass() == CLASS_HUNTER || player->getClass() == CLASS_SHAMAN) // Mail
            {
                player->learnSpell(8737); // Mail Armor
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 15664, true);
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 10118, true);
                player->EquipNewItem(EQUIPMENT_SLOT_FEET, 15674, true);
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 20659, true);
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 10236, true);
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 10125, true);
                player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 10233, true);
                player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 15668, true);
                if (player->getClass() == CLASS_HUNTER)
                {
                    if (!player->HasSkill(SKILL_BOWS))
                    {
                        player->LearnDefaultSkill(SKILL_BOWS, 1);
                    }
                    player->learnSpell(200); //Polearms
                    player->StoreNewItemInBestSlots(18482, 1);
                    player->StoreNewItemInBestSlots(11864, 1);
                }
                else
                {
                    player->StoreNewItemInBestSlots(15229, 2);
                    player->StoreNewItemInBestSlots(22395, 1); // Totems.
                }
            }
            if (player->getClass() == CLASS_WARRIOR || player->getClass() == CLASS_PALADIN) // Plate
            {
                player->learnSpell(750); // Plate Armor
                player->learnSpell(202); // 2h swords
                player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 14858, true);
                player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 8312, true);
                player->EquipNewItem(EQUIPMENT_SLOT_FEET, 10376, true);
                player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 14863, true);
                player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 10280, true);
                player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 8319, true);
                player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 14864, true);
                player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, 10377, true);
                player->StoreNewItemInBestSlots(15255, 1);
            }

            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                player->learnSpell(33388); // Apprentice riding
                if (player->getRace() == RACE_HUMAN)
                {
                    player->AddItem(5656, 1); // Horse (Alliance)
                }
                if (player->getRace() == RACE_DWARF)
                {
                    player->AddItem(5872, 1); // Mount (Alliance)
                }
                if (player->getRace() == RACE_NIGHTELF)
                {
                    player->AddItem(8629, 1); // Mount (Alliance)
                }
                if (player->getRace() == RACE_GNOME)
                {
                    player->AddItem(13321, 1); // Mount (Alliance)
                }
            }
            if (player->GetTeamId() == TEAM_HORDE)
            {
                player->learnSpell(33388); // Apprentice riding
                if (player->getRace() == RACE_ORC)
                {
                    player->AddItem(5665, 1); // Horse (Horde)
                }
                if (player->getRace() == RACE_UNDEAD_PLAYER)
                {
                    player->AddItem(13331, 1); // Mount (Horde)
                }
                if (player->getRace() == RACE_TAUREN)
                {
                    player->AddItem(15277, 1); // Mount (Horde)
                }
                if (player->getRace() == RACE_TROLL)
                {
                    player->AddItem(8591, 1); // Mount (Horde)
                }
            }
            if (player->GetTeamId() == TEAM_HORDE)
            {
                player->TeleportTo(1, 1598.277f, -4454.35f, 45.83f, 1.35f); // Orgrimmar
            }
            else
            {
                player->TeleportTo(0, -8541.46f, 1017.141f, 89.73f, 3.78f); // Stormwind
            }
            //player->TeleportTo(0, -11811.286f, -3191.507f, -30.722f, 3.29f); // Blasted Lands (Dark Portal)
            player->UpdateAllStats();
        }
        /*if (action == 1002)
        {
            player->SetLevel(67);
            player->AddItem(5763, 4); // 8 slot bag.
            player->TeleportTo(0, -11811.286f, -3191.507f, -30.722f, 3.29f); // Blasted Lands (Dark Portal)
        }*/
        if (action == 1003)
        {
            player->GetSession()->LogoutPlayer(true);
        }
        if (action == 1004)
        {
            if (player->getClass() == CLASS_WARRIOR)
            {
                // Stances (Defensive, Berseker)
                player->learnSpell(71);
                player->learnSpell(2458);
                player->learnSpell(72);
                player->learnSpell(284);
                player->learnSpell(285);
                player->learnSpell(674);
                player->learnSpell(676);
                player->learnSpell(694);
                player->learnSpell(750);
                player->learnSpell(845);
                player->learnSpell(871);
                player->learnSpell(1160);
                player->learnSpell(1161);
                player->learnSpell(1464);
                player->learnSpell(1608);
                player->learnSpell(1680);
                player->learnSpell(1715);
                player->learnSpell(1719);
                player->learnSpell(2565);
                player->learnSpell(2687);
                player->learnSpell(5242);
                player->learnSpell(5246);
                player->learnSpell(5308);
                player->learnSpell(6178);
                player->learnSpell(6190);
                player->learnSpell(6192);
                player->learnSpell(6546);
                player->learnSpell(6547);
                player->learnSpell(6548);
                player->learnSpell(6552);
                player->learnSpell(6572);
                player->learnSpell(6574);
                player->learnSpell(7369);
                player->learnSpell(7379);
                player->learnSpell(7384);
                player->learnSpell(8198);
                player->learnSpell(8204);
                player->learnSpell(8205);
                player->learnSpell(8820);
                player->learnSpell(11549);
                player->learnSpell(11550);
                player->learnSpell(11551);
                player->learnSpell(11554);
                player->learnSpell(11555);
                player->learnSpell(11556);
                player->learnSpell(11564);
                player->learnSpell(11565);
                player->learnSpell(11566);
                player->learnSpell(11567);
                player->learnSpell(11572);
                player->learnSpell(11573);
                player->learnSpell(11578);
                player->learnSpell(11580);
                player->learnSpell(11600);
                player->learnSpell(11601);
                player->learnSpell(11604);
                player->learnSpell(11605);
                player->learnSpell(11608);
                player->learnSpell(11609);
                player->learnSpell(12678);
                player->learnSpell(18499);
                player->learnSpell(20230);
                player->learnSpell(20252);
                player->learnSpell(20658);
                player->learnSpell(20660);
                player->learnSpell(20661);
                player->learnSpell(20662);
                player->learnSpell(23922);
                player->learnSpell(23923);
                player->learnSpell(23924);

            }
            if (player->getClass() == CLASS_PALADIN)
            {
                player->learnSpell(7328); //Redemption
                player->learnSpell(633);
                player->learnSpell(642);
                player->learnSpell(643);
                player->learnSpell(647);
                player->learnSpell(750);
                player->learnSpell(853);
                player->learnSpell(879);
                player->learnSpell(1022);
                player->learnSpell(1026);
                player->learnSpell(1032);
                player->learnSpell(1038);
                player->learnSpell(1042);
                player->learnSpell(1044);
                player->learnSpell(1152);
                player->learnSpell(2800);
                player->learnSpell(2812);
                player->learnSpell(3127);
                player->learnSpell(3472);
                player->learnSpell(4987);
                player->learnSpell(5502);
                player->learnSpell(5588);
                player->learnSpell(5589);
                player->learnSpell(5599);
                player->learnSpell(5614);
                player->learnSpell(5615);
                player->learnSpell(6940);
                player->learnSpell(7294);
                player->learnSpell(10278);
                player->learnSpell(10290);
                player->learnSpell(10291);
                player->learnSpell(10292);
                player->learnSpell(10298);
                player->learnSpell(10299);
                player->learnSpell(10300);
                player->learnSpell(10301);
                player->learnSpell(10308);
                player->learnSpell(10310);
                player->learnSpell(10312);
                player->learnSpell(10313);
                player->learnSpell(10322);
                player->learnSpell(10324);
                player->learnSpell(10326);
                player->learnSpell(10328);
                player->learnSpell(10329);
                player->learnSpell(19742);
                player->learnSpell(19746);
                player->learnSpell(19750);
                player->learnSpell(19752);
                player->learnSpell(19834);
                player->learnSpell(19835);
                player->learnSpell(19836);
                player->learnSpell(19837);
                player->learnSpell(19838);
                player->learnSpell(19850);
                player->learnSpell(19852);
                player->learnSpell(19853);
                player->learnSpell(19854);
                player->learnSpell(19876);
                player->learnSpell(19888);
                player->learnSpell(19891);
                player->learnSpell(19895);
                player->learnSpell(19896);
                player->learnSpell(19897);
                player->learnSpell(19898);
                player->learnSpell(19899);
                player->learnSpell(19939);
                player->learnSpell(19940);
                player->learnSpell(19941);
                player->learnSpell(19942);
                player->learnSpell(20116);
                player->learnSpell(20164);
                player->learnSpell(20165);
                player->learnSpell(20166);
                player->learnSpell(20217);
                player->learnSpell(20772);
                player->learnSpell(20922);
                player->learnSpell(20923);
                player->learnSpell(24274);
                player->learnSpell(24275);
                player->learnSpell(25780);
                player->learnSpell(25782);
                player->learnSpell(25894);
                player->learnSpell(26573);
                player->learnSpell(31789);
                player->learnSpell(53407);
                player->learnSpell(53408);
                player->learnSpell(62124);
            }
            if (player->getClass() == CLASS_HUNTER)
            {
                player->learnSpell(1515); // Tame Beast
                player->learnSpell(883); // Call Pet
                player->learnSpell(2641); // Dismiss Pet
                player->learnSpell(6991); // Feed Pet
                player->learnSpell(982); // Revive Pet
                //Hunter Spells up to 56.
                player->learnSpell(136);
                player->learnSpell(674);
                player->learnSpell(781);
                player->learnSpell(1002);
                player->learnSpell(1462);
                player->learnSpell(1495);
                player->learnSpell(1499);
                player->learnSpell(1510);
                player->learnSpell(1513);
                player->learnSpell(1543);
                player->learnSpell(2643);
                player->learnSpell(2974);
                player->learnSpell(3034);
                player->learnSpell(3043);
                player->learnSpell(3045);
                player->learnSpell(3111);
                player->learnSpell(3127);
                player->learnSpell(3661);
                player->learnSpell(3662);
                player->learnSpell(5116);
                player->learnSpell(5118);
                player->learnSpell(5384);
                player->learnSpell(6197);
                player->learnSpell(8737);
                player->learnSpell(13159);
                player->learnSpell(13161);
                player->learnSpell(13165);
                player->learnSpell(13542);
                player->learnSpell(13543);
                player->learnSpell(13549);
                player->learnSpell(13550);
                player->learnSpell(13551);
                player->learnSpell(13552);
                player->learnSpell(13553);
                player->learnSpell(13554);
                player->learnSpell(13795);
                player->learnSpell(13809);
                player->learnSpell(13813);
                player->learnSpell(14260);
                player->learnSpell(14261);
                player->learnSpell(14262);
                player->learnSpell(14263);
                player->learnSpell(14264);
                player->learnSpell(14265);
                player->learnSpell(14266);
                player->learnSpell(14269);
                player->learnSpell(14270);
                player->learnSpell(14281);
                player->learnSpell(14282);
                player->learnSpell(14283);
                player->learnSpell(14284);
                player->learnSpell(14285);
                player->learnSpell(14286);
                player->learnSpell(14288);
                player->learnSpell(14289);
                player->learnSpell(14290);
                player->learnSpell(14294);
                player->learnSpell(14302);
                player->learnSpell(14303);
                player->learnSpell(14304);
                player->learnSpell(14305);
                player->learnSpell(14310);
                player->learnSpell(14316);
                player->learnSpell(14317);
                player->learnSpell(14318);
                player->learnSpell(14319);
                player->learnSpell(14320);
                player->learnSpell(14321);
                player->learnSpell(14323);
                player->learnSpell(14324);
                player->learnSpell(14326);
                player->learnSpell(14327);
                player->learnSpell(19878);
                player->learnSpell(19879);
                player->learnSpell(19880);
                player->learnSpell(19882);
                player->learnSpell(19883);
                player->learnSpell(19884);
                player->learnSpell(19885);
                player->learnSpell(20043);
                player->learnSpell(20190);
                player->learnSpell(20736);
                player->learnSpell(34074);
                player->learnSpell(56641);
                player->CreatePet(524); // Create hunter a pet
            }
            if (player->getClass() == CLASS_SHAMAN)
            {
                player->learnSpell(131);
                player->learnSpell(324);
                player->learnSpell(325);
                player->learnSpell(370);
                player->learnSpell(421);
                player->learnSpell(526);
                player->learnSpell(529);
                player->learnSpell(546);
                player->learnSpell(547);
                player->learnSpell(548);
                player->learnSpell(556);
                player->learnSpell(905);
                player->learnSpell(913);
                player->learnSpell(915);
                player->learnSpell(930);
                player->learnSpell(939);
                player->learnSpell(943);
                player->learnSpell(945);
                player->learnSpell(959);
                player->learnSpell(1064);
                player->learnSpell(1535);
                player->learnSpell(2008);
                player->learnSpell(2645);
                player->learnSpell(2860);
                player->learnSpell(5675);
                player->learnSpell(5730);
                player->learnSpell(6041);
                player->learnSpell(6196);
                player->learnSpell(6363);
                player->learnSpell(6364);
                player->learnSpell(6365);
                player->learnSpell(6375);
                player->learnSpell(6377);
                player->learnSpell(6390);
                player->learnSpell(6391);
                player->learnSpell(6392);
                player->learnSpell(6495);
                player->learnSpell(8004);
                player->learnSpell(8005);
                player->learnSpell(8008);
                player->learnSpell(8010);
                player->learnSpell(8012);
                player->learnSpell(8018);
                player->learnSpell(8019);
                player->learnSpell(8024);
                player->learnSpell(8027);
                player->learnSpell(8030);
                player->learnSpell(8033);
                player->learnSpell(8038);
                player->learnSpell(8044);
                player->learnSpell(8045);
                player->learnSpell(8046);
                player->learnSpell(8050);
                player->learnSpell(8052);
                player->learnSpell(8053);
                player->learnSpell(8056);
                player->learnSpell(8058);
                player->learnSpell(8075);
                player->learnSpell(8134);
                player->learnSpell(8143);
                player->learnSpell(8154);
                player->learnSpell(8155);
                player->learnSpell(8160);
                player->learnSpell(8161);
                player->learnSpell(8170);
                player->learnSpell(8177);
                player->learnSpell(8181);
                player->learnSpell(8184);
                player->learnSpell(8190);
                player->learnSpell(8227);
                player->learnSpell(8232);
                player->learnSpell(8235);
                player->learnSpell(8249);
                player->learnSpell(8498);
                player->learnSpell(8499);
                player->learnSpell(8512);
                player->learnSpell(8737);
                player->learnSpell(10391);
                player->learnSpell(10392);
                player->learnSpell(10395);
                player->learnSpell(10396);
                player->learnSpell(10399);
                player->learnSpell(10406);
                player->learnSpell(10407);
                player->learnSpell(10408);
                player->learnSpell(10412);
                player->learnSpell(10413);
                player->learnSpell(10427);
                player->learnSpell(10431);
                player->learnSpell(10432);
                player->learnSpell(10437);
                player->learnSpell(10442);
                player->learnSpell(10447);
                player->learnSpell(10448);
                player->learnSpell(10456);
                player->learnSpell(10462);
                player->learnSpell(10466);
                player->learnSpell(10467);
                player->learnSpell(10472);
                player->learnSpell(10478);
                player->learnSpell(10479);
                player->learnSpell(10486);
                player->learnSpell(10495);
                player->learnSpell(10496);
                player->learnSpell(10497);
                player->learnSpell(10526);
                player->learnSpell(10537);
                player->learnSpell(10585);
                player->learnSpell(10586);
                player->learnSpell(10587);
                player->learnSpell(10595);
                player->learnSpell(10600);
                player->learnSpell(10605);
                player->learnSpell(10622);
                player->learnSpell(10623);
                player->learnSpell(11314);
                player->learnSpell(11315);
                player->learnSpell(15207);
                player->learnSpell(15208);
                player->learnSpell(16339);
                player->learnSpell(16341);
                player->learnSpell(16342);
                player->learnSpell(16355);
                player->learnSpell(20608);
                player->learnSpell(20609);
                player->learnSpell(20610);
                player->learnSpell(20776);
                player->learnSpell(36936);
                player->learnSpell(51730);
                player->learnSpell(51988);
                player->learnSpell(51991);
                player->learnSpell(52127);
                player->learnSpell(52129);
                player->learnSpell(52131);
                player->learnSpell(52134);
                player->learnSpell(52136);
                player->learnSpell(52138);
                player->learnSpell(57994);
                player->learnSpell(66842);
                player->learnSpell(66843);
                player->learnSpell(66844);
            }
            if (player->getClass() == CLASS_DRUID)
            {
                player->learnSpell(9634); // Dire bear form
                player->learnSpell(1066); // Aquatic form
                player->learnSpell(99);
                player->learnSpell(339);
                player->learnSpell(740);
                player->learnSpell(768);
                player->learnSpell(769);
                player->learnSpell(770);
                player->learnSpell(779);
                player->learnSpell(780);
                player->learnSpell(782);
                player->learnSpell(783);
                player->learnSpell(1058);
                player->learnSpell(1062);
                player->learnSpell(1066);
                player->learnSpell(1075);
                player->learnSpell(1079);
                player->learnSpell(1082);
                player->learnSpell(1430);
                player->learnSpell(1735);
                player->learnSpell(1822);
                player->learnSpell(1823);
                player->learnSpell(1824);
                player->learnSpell(1850);
                player->learnSpell(2090);
                player->learnSpell(2091);
                player->learnSpell(2637);
                player->learnSpell(2782);
                player->learnSpell(2893);
                player->learnSpell(2908);
                player->learnSpell(2912);
                player->learnSpell(3029);
                player->learnSpell(3627);
                player->learnSpell(5178);
                player->learnSpell(5179);
                player->learnSpell(5180);
                player->learnSpell(5186);
                player->learnSpell(5187);
                player->learnSpell(5188);
                player->learnSpell(5189);
                player->learnSpell(5195);
                player->learnSpell(5196);
                player->learnSpell(5201);
                player->learnSpell(5209);
                player->learnSpell(5211);
                player->learnSpell(5215);
                player->learnSpell(5217);
                player->learnSpell(5221);
                player->learnSpell(5225);
                player->learnSpell(5229);
                player->learnSpell(5232);
                player->learnSpell(5234);
                player->learnSpell(6756);
                player->learnSpell(6778);
                player->learnSpell(6780);
                player->learnSpell(6785);
                player->learnSpell(6787);
                player->learnSpell(6793);
                player->learnSpell(6798);
                player->learnSpell(6800);
                player->learnSpell(6808);
                player->learnSpell(6809);
                player->learnSpell(8903);
                player->learnSpell(8905);
                player->learnSpell(8907);
                player->learnSpell(8910);
                player->learnSpell(8914);
                player->learnSpell(8918);
                player->learnSpell(8924);
                player->learnSpell(8925);
                player->learnSpell(8926);
                player->learnSpell(8927);
                player->learnSpell(8928);
                player->learnSpell(8929);
                player->learnSpell(8936);
                player->learnSpell(8938);
                player->learnSpell(8939);
                player->learnSpell(8940);
                player->learnSpell(8941);
                player->learnSpell(8949);
                player->learnSpell(8950);
                player->learnSpell(8951);
                player->learnSpell(8955);
                player->learnSpell(8972);
                player->learnSpell(8983);
                player->learnSpell(8992);
                player->learnSpell(8998);
                player->learnSpell(9000);
                player->learnSpell(9005);
                player->learnSpell(9490);
                player->learnSpell(9492);
                player->learnSpell(9493);
                player->learnSpell(9634);
                player->learnSpell(9745);
                player->learnSpell(9747);
                player->learnSpell(9750);
                player->learnSpell(9752);
                player->learnSpell(9754);
                player->learnSpell(9756);
                player->learnSpell(9758);
                player->learnSpell(9821);
                player->learnSpell(9823);
                player->learnSpell(9827);
                player->learnSpell(9829);
                player->learnSpell(9830);
                player->learnSpell(9833);
                player->learnSpell(9834);
                player->learnSpell(9839);
                player->learnSpell(9840);
                player->learnSpell(9845);
                player->learnSpell(9849);
                player->learnSpell(9852);
                player->learnSpell(9856);
                player->learnSpell(9857);
                player->learnSpell(9862);
                player->learnSpell(9866);
                player->learnSpell(9875);
                player->learnSpell(9880);
                player->learnSpell(9884);
                player->learnSpell(9888);
                player->learnSpell(9889);
                player->learnSpell(9892);
                player->learnSpell(9894);
                player->learnSpell(9898);
                player->learnSpell(9901);
                player->learnSpell(9904);
                player->learnSpell(9908);
                player->learnSpell(9910);
                player->learnSpell(9912);
                player->learnSpell(16689);
                player->learnSpell(16810);
                player->learnSpell(16811);
                player->learnSpell(16812);
                player->learnSpell(16813);
                player->learnSpell(16857);
                player->learnSpell(16914);
                player->learnSpell(17401);
                player->learnSpell(18657);
                player->learnSpell(20484);
                player->learnSpell(20719);
                player->learnSpell(20739);
                player->learnSpell(20742);
                player->learnSpell(20747);
                player->learnSpell(21849);
                player->learnSpell(22568);
                player->learnSpell(22812);
                player->learnSpell(22827);
                player->learnSpell(22828);
                player->learnSpell(22829);
                player->learnSpell(22842);
                player->learnSpell(29166);
                player->learnSpell(50766);
                player->learnSpell(50767);
                player->learnSpell(50768);
                player->learnSpell(50769);
                player->learnSpell(62600);
            }
            if (player->getClass() == CLASS_ROGUE)
            {
                player->learnSpell(2842); // Poisons
                player->learnSpell(408);
                player->learnSpell(674);
                player->learnSpell(703);
                player->learnSpell(1725);
                player->learnSpell(1758);
                player->learnSpell(1759);
                player->learnSpell(1760);
                player->learnSpell(1766);
                player->learnSpell(1804);
                player->learnSpell(1833);
                player->learnSpell(1842);
                player->learnSpell(1856);
                player->learnSpell(1857);
                player->learnSpell(1860);
                player->learnSpell(1943);
                player->learnSpell(1966);
                player->learnSpell(2070);
                player->learnSpell(2094);
                player->learnSpell(2589);
                player->learnSpell(2590);
                player->learnSpell(2591);
                player->learnSpell(2836);
                player->learnSpell(2983);
                player->learnSpell(3127);
                player->learnSpell(5171);
                player->learnSpell(5277);
                player->learnSpell(6760);
                player->learnSpell(6761);
                player->learnSpell(6762);
                player->learnSpell(6768);
                player->learnSpell(6770);
                player->learnSpell(6774);
                player->learnSpell(8621);
                player->learnSpell(8623);
                player->learnSpell(8624);
                player->learnSpell(8631);
                player->learnSpell(8632);
                player->learnSpell(8633);
                player->learnSpell(8637);
                player->learnSpell(8639);
                player->learnSpell(8640);
                player->learnSpell(8643);
                player->learnSpell(8647);
                player->learnSpell(8676);
                player->learnSpell(8696);
                player->learnSpell(8721);
                player->learnSpell(8724);
                player->learnSpell(8725);
                player->learnSpell(11267);
                player->learnSpell(11268);
                player->learnSpell(11273);
                player->learnSpell(11274);
                player->learnSpell(11279);
                player->learnSpell(11280);
                player->learnSpell(11289);
                player->learnSpell(11290);
                player->learnSpell(11293);
                player->learnSpell(11294);
                player->learnSpell(11297);
                player->learnSpell(11299);
                player->learnSpell(11300);
                player->learnSpell(11303);
                player->learnSpell(26669);
                player->learnSpell(51722);
            }
            if (player->getClass() == CLASS_PRIEST)
            {
                player->learnSpell(139);
                player->learnSpell(453);
                player->learnSpell(527);
                player->learnSpell(528);
                player->learnSpell(552);
                player->learnSpell(586);
                player->learnSpell(588);
                player->learnSpell(592);
                player->learnSpell(594);
                player->learnSpell(596);
                player->learnSpell(598);
                player->learnSpell(600);
                player->learnSpell(602);
                player->learnSpell(605);
                player->learnSpell(970);
                player->learnSpell(976);
                player->learnSpell(984);
                player->learnSpell(988);
                player->learnSpell(992);
                player->learnSpell(996);
                player->learnSpell(1004);
                player->learnSpell(1006);
                player->learnSpell(1244);
                player->learnSpell(1245);
                player->learnSpell(1706);
                player->learnSpell(2006);
                player->learnSpell(2010);
                player->learnSpell(2053);
                player->learnSpell(2054);
                player->learnSpell(2055);
                player->learnSpell(2060);
                player->learnSpell(2061);
                player->learnSpell(2096);
                player->learnSpell(2767);
                player->learnSpell(2791);
                player->learnSpell(2944);
                player->learnSpell(3747);
                player->learnSpell(6060);
                player->learnSpell(6063);
                player->learnSpell(6064);
                player->learnSpell(6065);
                player->learnSpell(6066);
                player->learnSpell(6074);
                player->learnSpell(6075);
                player->learnSpell(6076);
                player->learnSpell(6077);
                player->learnSpell(6078);
                player->learnSpell(6346);
                player->learnSpell(7128);
                player->learnSpell(8092);
                player->learnSpell(8102);
                player->learnSpell(8103);
                player->learnSpell(8104);
                player->learnSpell(8105);
                player->learnSpell(8106);
                player->learnSpell(8122);
                player->learnSpell(8124);
                player->learnSpell(8129);
                player->learnSpell(9472);
                player->learnSpell(9473);
                player->learnSpell(9474);
                player->learnSpell(9484);
                player->learnSpell(9485);
                player->learnSpell(10880);
                player->learnSpell(10881);
                player->learnSpell(10888);
                player->learnSpell(10890);
                player->learnSpell(10892);
                player->learnSpell(10893);
                player->learnSpell(10898);
                player->learnSpell(10899);
                player->learnSpell(10900);
                player->learnSpell(10909);
                player->learnSpell(10915);
                player->learnSpell(10916);
                player->learnSpell(10917);
                player->learnSpell(10927);
                player->learnSpell(10928);
                player->learnSpell(10929);
                player->learnSpell(10933);
                player->learnSpell(10934);
                player->learnSpell(10937);
                player->learnSpell(10945);
                player->learnSpell(10946);
                player->learnSpell(10951);
                player->learnSpell(10957);
                player->learnSpell(10958);
                player->learnSpell(10960);
                player->learnSpell(10963);
                player->learnSpell(10964);
                player->learnSpell(14752);
                player->learnSpell(14818);
                player->learnSpell(14819);
                player->learnSpell(14914);
                player->learnSpell(15237);
                player->learnSpell(15262);
                player->learnSpell(15263);
                player->learnSpell(15264);
                player->learnSpell(15265);
                player->learnSpell(15266);
                player->learnSpell(15267);
                player->learnSpell(15430);
                player->learnSpell(15431);
                player->learnSpell(19276);
                player->learnSpell(19277);
                player->learnSpell(19278);
                player->learnSpell(19279);
                player->learnSpell(21562);
                player->learnSpell(27683);
                player->learnSpell(27799);
                player->learnSpell(27800);
            }
            if (player->getClass() == CLASS_WARLOCK)
            {
                player->learnSpell(697); // Voidwalker
                player->learnSpell(712); // Succubus
                player->learnSpell(691); // Felhunter
                player->learnSpell(126);
                player->learnSpell(132);
                player->learnSpell(688);
                player->learnSpell(689);
                player->learnSpell(693);
                player->learnSpell(696);
                player->learnSpell(698);
                player->learnSpell(699);
                player->learnSpell(705);
                player->learnSpell(706);
                player->learnSpell(707);
                player->learnSpell(709);
                player->learnSpell(710);
                player->learnSpell(755);
                player->learnSpell(980);
                player->learnSpell(1014);
                player->learnSpell(1086);
                player->learnSpell(1088);
                player->learnSpell(1094);
                player->learnSpell(1098);
                player->learnSpell(1106);
                player->learnSpell(1108);
                player->learnSpell(1120);
                player->learnSpell(1455);
                player->learnSpell(1456);
                player->learnSpell(1490);
                player->learnSpell(1710);
                player->learnSpell(1714);
                player->learnSpell(1949);
                player->learnSpell(2362);
                player->learnSpell(2941);
                player->learnSpell(3698);
                player->learnSpell(3699);
                player->learnSpell(3700);
                player->learnSpell(5138);
                player->learnSpell(5484);
                player->learnSpell(5500);
                player->learnSpell(5676);
                player->learnSpell(5697);
                player->learnSpell(5699);
                player->learnSpell(5740);
                player->learnSpell(5782);
                player->learnSpell(6201);
                player->learnSpell(6202);
                player->learnSpell(6205);
                player->learnSpell(6213);
                player->learnSpell(6215);
                player->learnSpell(6217);
                player->learnSpell(6219);
                player->learnSpell(6222);
                player->learnSpell(6223);
                player->learnSpell(6229);
                player->learnSpell(6353);
                player->learnSpell(6366);
                player->learnSpell(6789);
                player->learnSpell(7641);
                player->learnSpell(7646);
                player->learnSpell(7648);
                player->learnSpell(7651);
                player->learnSpell(8288);
                player->learnSpell(8289);
                player->learnSpell(11659);
                player->learnSpell(11660);
                player->learnSpell(11665);
                player->learnSpell(11667);
                player->learnSpell(11671);
                player->learnSpell(11672);
                player->learnSpell(11675);
                player->learnSpell(11677);
                player->learnSpell(11683);
                player->learnSpell(11684);
                player->learnSpell(11687);
                player->learnSpell(11688);
                player->learnSpell(11689);
                player->learnSpell(11693);
                player->learnSpell(11694);
                player->learnSpell(11699);
                player->learnSpell(11700);
                player->learnSpell(11707);
                player->learnSpell(11708);
                player->learnSpell(11711);
                player->learnSpell(11712);
                player->learnSpell(11719);
                player->learnSpell(11721);
                player->learnSpell(11725);
                player->learnSpell(11729);
                player->learnSpell(11733);
                player->learnSpell(11734);
                player->learnSpell(11739);
                player->learnSpell(11740);
                player->learnSpell(17727);
                player->learnSpell(17919);
                player->learnSpell(17920);
                player->learnSpell(17921);
                player->learnSpell(17922);
                player->learnSpell(17924);
                player->learnSpell(17925);
                player->learnSpell(17928);
                player->learnSpell(17951);
                player->learnSpell(17952);
                player->learnSpell(17953);
                player->learnSpell(18647);
                player->learnSpell(20752);
                player->learnSpell(20755);
                player->learnSpell(20756);
            }
            if (player->getClass() == CLASS_MAGE)
            {
                player->learnSpell(10);
                player->learnSpell(118);
                player->learnSpell(120);
                player->learnSpell(122);
                player->learnSpell(130);
                player->learnSpell(145);
                player->learnSpell(205);
                player->learnSpell(475);
                player->learnSpell(543);
                player->learnSpell(597);
                player->learnSpell(604);
                player->learnSpell(759);
                player->learnSpell(837);
                player->learnSpell(865);
                player->learnSpell(990);
                player->learnSpell(1008);
                player->learnSpell(1449);
                player->learnSpell(1460);
                player->learnSpell(1461);
                player->learnSpell(1463);
                player->learnSpell(1953);
                player->learnSpell(2120);
                player->learnSpell(2121);
                player->learnSpell(2137);
                player->learnSpell(2138);
                player->learnSpell(2139);
                player->learnSpell(2948);
                player->learnSpell(3140);
                player->learnSpell(3552);
                player->learnSpell(5143);
                player->learnSpell(5144);
                player->learnSpell(5145);
                player->learnSpell(5505);
                player->learnSpell(5506);
                player->learnSpell(6117);
                player->learnSpell(6127);
                player->learnSpell(6129);
                player->learnSpell(6131);
                player->learnSpell(6141);
                player->learnSpell(6143);
                player->learnSpell(7300);
                player->learnSpell(7301);
                player->learnSpell(7302);
                player->learnSpell(7320);
                player->learnSpell(7322);
                player->learnSpell(8400);
                player->learnSpell(8401);
                player->learnSpell(8402);
                player->learnSpell(8406);
                player->learnSpell(8407);
                player->learnSpell(8408);
                player->learnSpell(8412);
                player->learnSpell(8413);
                player->learnSpell(8416);
                player->learnSpell(8417);
                player->learnSpell(8422);
                player->learnSpell(8423);
                player->learnSpell(8427);
                player->learnSpell(8437);
                player->learnSpell(8438);
                player->learnSpell(8439);
                player->learnSpell(8444);
                player->learnSpell(8445);
                player->learnSpell(8446);
                player->learnSpell(8450);
                player->learnSpell(8451);
                player->learnSpell(8455);
                player->learnSpell(8457);
                player->learnSpell(8458);
                player->learnSpell(8461);
                player->learnSpell(8462);
                player->learnSpell(8492);
                player->learnSpell(8494);
                player->learnSpell(8495);
                player->learnSpell(10053);
                player->learnSpell(10138);
                player->learnSpell(10139);
                player->learnSpell(10144);
                player->learnSpell(10145);
                player->learnSpell(10148);
                player->learnSpell(10149);
                player->learnSpell(10150);
                player->learnSpell(10156);
                player->learnSpell(10157);
                player->learnSpell(10159);
                player->learnSpell(10160);
                player->learnSpell(10169);
                player->learnSpell(10170);
                player->learnSpell(10173);
                player->learnSpell(10177);
                player->learnSpell(10179);
                player->learnSpell(10180);
                player->learnSpell(10181);
                player->learnSpell(10185);
                player->learnSpell(10186);
                player->learnSpell(10191);
                player->learnSpell(10192);
                player->learnSpell(10197);
                player->learnSpell(10199);
                player->learnSpell(10201);
                player->learnSpell(10202);
                player->learnSpell(10205);
                player->learnSpell(10206);
                player->learnSpell(10211);
                player->learnSpell(10212);
                player->learnSpell(10215);
                player->learnSpell(10216);
                player->learnSpell(10219);
                player->learnSpell(10223);
                player->learnSpell(10230);
                player->learnSpell(12051);
                player->learnSpell(12824);
                player->learnSpell(12825);
                player->learnSpell(22782);
                player->learnSpell(23028);
                player->learnSpell(45438);
            }
            CloseGossipMenuFor(player);
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        /*if (player->GetLevel() >= 2 && player->GetLevel() <= 57 && CONFIG_MAX_PLAYER_LEVEL == 70 || player->IsGameMaster())
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Give me more information about Level 58 Boost.", GOSSIP_SENDER_MAIN, 1003, "To begin with a level 58 boost you must create a new character and not be higher than level 1. Do you want to be logged out to create a character?", 0, false);
        }*/
        if (player->GetLevel() <= 56) //CONFIG_MAX_PLAYER_LEVEL == 60
        {
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "I already have a level 60 character and would like to boost to 58.", GOSSIP_SENDER_MAIN, 1001, "Are you sure you want to Level Boost to 58? You will be logged out to the character screen.", 0, false);
        }
        if (player->GetLevel() >= 57 && player->GetLevel() <= 59) // CONFIG_MAX_PLAYER_LEVEL == 60 && 
        {
            AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "I'd like to train my spells and abilities.", GOSSIP_SENDER_MAIN, 1004, "You will instantly learn your spells and abilities up to 58. Continue?", 0, false);
        }
        /*if (CONFIG_MAX_PLAYER_LEVEL == 80 && player->GetLevel() == 1)
        {
            AddGossipItemFor(player, 1, "I have already heard the stories of Outlands, i'd like to begin my adventure in Northrend.", GOSSIP_SENDER_MAIN, 1002);
        }*/

        SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }
};

class npc_startzone_teleporter : public CreatureScript
{
public:
    npc_startzone_teleporter() : CreatureScript("npc_startzone_teleporter") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);
        if (action == 1001)
        {
            player->TeleportTo(1, 10309.735f, 831.3456f, 1326.4413f, 2.2f);
        }
        if (action == 1002)
        {
            player->TeleportTo(0, -6231.77f, 332.993f, 383.171f, 0.48f);
        }
        if (action == 1003)
        {
            player->TeleportTo(0, -8952.698f, -108.925f, 83.47f, 5.55f);
        }
        if (action == 1004)
        {
            player->TeleportTo(1, -607.24f, -4248.208f, 38.95f, 4.12f);
        }
        if (action == 1005)
        {
            player->TeleportTo(1, -2927.30f, -256.88f, 53.66f, 0.37f);
        }
        if (action == 1006)
        {
            player->TeleportTo(0, 1670.98f, 1663.164f, 141.233f, 0.02f);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (player->GetTeamId() == TEAM_ALLIANCE)
        {
            AddGossipItemFor(player, 1, "Teleport me to the Night Elf starting area.", GOSSIP_SENDER_MAIN, 1001);
            AddGossipItemFor(player, 1, "Teleport me to the Dwarf/Gnome starting area.", GOSSIP_SENDER_MAIN, 1002);
            AddGossipItemFor(player, 1, "Teleport me to the Human starting area.", GOSSIP_SENDER_MAIN, 1003);
        }
        if (player->GetTeamId() == TEAM_HORDE)
        {
            AddGossipItemFor(player, 1, "Teleport me to the Orc/Troll starting area.", GOSSIP_SENDER_MAIN, 1004);
            AddGossipItemFor(player, 1, "Teleport me to the Tauren starting area.", GOSSIP_SENDER_MAIN, 1005);
            AddGossipItemFor(player, 1, "Teleport me to the Undead starting area.", GOSSIP_SENDER_MAIN, 1006);
        }

        SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }
};

struct npc_cameron : public ScriptedAI
{
    npc_cameron(Creature* creature) : ScriptedAI(creature)
    {
        _started = false;
    }

    static uint32 SoundPicker()
    {
        return RAND(
            BANSHEE_DEATH,
            BANSHEEPREAGGRO,
            CTHUN_YOU_WILL_DIE,
            CTHUN_DEATH_IS_CLOSE,
            HUMAN_FEMALE_EMOTE_CRY,
            GHOSTDEATH
        );
    }

    void MoveTheChildren()
    {
        std::vector<Position> MovePosPositions =
        {
            { -9373.521f, -67.71767f, 69.201965f, 1.117011f },
            { -9374.94f, -62.51654f, 69.201965f, 5.201081f },
            { -9371.013f, -71.20811f, 69.201965f, 1.937315f },
            { -9368.419f, -66.47543f, 69.201965f, 3.141593f },
            { -9372.376f, -65.49946f, 69.201965f, 4.206244f },
            { -9377.477f, -67.8297f, 69.201965f, 0.296706f }
        };

        Acore::Containers::RandomShuffle(MovePosPositions);

        // first we break formation because children will need to move on their own now
        for (auto& guid : _childrenGUIDs)
            if (Creature* child = ObjectAccessor::GetCreature(*me, guid))
                if (child->GetFormation())
                    child->GetFormation()->RemoveMember(child);

        // Move each child to an random position
        for (uint32 i = 0; i < _childrenGUIDs.size(); ++i)
        {
            if (Creature* children = ObjectAccessor::GetCreature(*me, _childrenGUIDs[i]))
            {
                children->SetWalk(true);
                children->GetMotionMaster()->MovePoint(0, MovePosPositions[i], true, MovePosPositions[i].GetOrientation());
            }
        }
        me->SetWalk(true);
        me->GetMotionMaster()->MovePoint(0, MovePosPositions.back(), true, MovePosPositions.back().GetOrientation());
    }

    void PathEndReached(uint32 pathId) override
    {
        switch (pathId)
        {
            case STORMWIND_PATH:
            {
                _events.ScheduleEvent(EVENT_RANDOM_MOVEMENT, 2s);
                _events.ScheduleEvent(EVENT_WP_START_GOLDSHIRE, 11min);
                break;
            }
            case GOLDSHIRE_PATH:
            {
                _events.ScheduleEvent(EVENT_RANDOM_MOVEMENT, 2s);
                _events.ScheduleEvent(EVENT_WP_START_WOODS, 15min);
                break;
            }
            case WOODS_PATH:
            {
                _events.ScheduleEvent(EVENT_RANDOM_MOVEMENT, 2s);
                _events.ScheduleEvent(EVENT_WP_START_HOUSE, 6min); // 6 minutes
                _events.ScheduleEvent(EVENT_WP_START_LISA, 362s);
                break;
            }
            case HOUSE_PATH:
            {
                // Move childeren at last point
                MoveTheChildren();
                // After 30 seconds a random sound should play
                _events.ScheduleEvent(EVENT_PLAY_SOUNDS, 30s);
                break;
            }
        }
    }

    void sOnGameEvent(bool start, uint16 eventId) override
    {
        if (start && eventId == GAME_EVENT_CHILDREN_OF_GOLDSHIRE)
        {
            // Start event at 7 am
            // Begin pathing
            _events.ScheduleEvent(EVENT_BEGIN_EVENT, 2s);
            _started = true;
        }
        else if (!start && eventId == GAME_EVENT_CHILDREN_OF_GOLDSHIRE)
        {
            // Reset event at 8 am
            _started = false;
            _events.Reset();
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!_started)
            return;

        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_WP_START_GOLDSHIRE:
                me->GetMotionMaster()->MovePath(GOLDSHIRE_PATH, false);
                break;
            case EVENT_WP_START_WOODS:
                me->GetMotionMaster()->MovePath(WOODS_PATH, false);
                break;
            case EVENT_WP_START_HOUSE:
                me->GetMotionMaster()->MovePath(HOUSE_PATH, false);
                break;
            case EVENT_WP_START_LISA:
                for (uint32 i = 0; i < _childrenGUIDs.size(); ++i)
                {
                    if (Creature* lisa = ObjectAccessor::GetCreature(*me, _childrenGUIDs[i]))
                    {
                        if (lisa->GetEntry() == NPC_LISA)
                        {
                            lisa->GetMotionMaster()->MovePath(LISA_PATH, false);
                            break;
                        }
                    }
                }
                break;
            case EVENT_PLAY_SOUNDS:
                me->PlayDistanceSound(SoundPicker());
                break;
            case EVENT_BEGIN_EVENT:
            {
                _childrenGUIDs.clear();

                // Get all childeren's guid's.
                if (Creature* dana = me->FindNearestCreature(NPC_DANA, 25.0f))
                    _childrenGUIDs.push_back(dana->GetGUID());

                if (Creature* john = me->FindNearestCreature(NPC_JOHN, 25.0f))
                    _childrenGUIDs.push_back(john->GetGUID());

                if (Creature* lisa = me->FindNearestCreature(NPC_LISA, 25.0f))
                    _childrenGUIDs.push_back(lisa->GetGUID());

                if (Creature* aaron = me->FindNearestCreature(NPC_AARON, 25.0f))
                    _childrenGUIDs.push_back(aaron->GetGUID());

                if (Creature* jose = me->FindNearestCreature(NPC_JOSE, 25.0f))
                    _childrenGUIDs.push_back(jose->GetGUID());

                // If Formation was disbanded, remake.
                if (!me->GetFormation()->IsFormed())
                    for (auto& guid : _childrenGUIDs)
                        if (Creature* child = ObjectAccessor::GetCreature(*me, guid))
                            child->SearchFormation();

                // Start movement
                me->GetMotionMaster()->MovePath(STORMWIND_PATH, false);

                break;
            }
            case EVENT_RANDOM_MOVEMENT:
            {
                me->GetMotionMaster()->MoveRandom(10.0f);
                break;
            }
            default:
                break;
            }
        }
    }

private:
    EventMap _events;
    bool _started;
    GuidVector _childrenGUIDs;
};

/*######
## npc_supervisor_raelen
######*/

enum SupervisorRaelen
{
    EVENT_FIND_PEASENTS  = 8,
    EVENT_NEXT_PEASENT   = 9,
    NPC_EASTVALE_PEASENT = 11328
};

struct npc_supervisor_raelen : public ScriptedAI
{
    npc_supervisor_raelen(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        _PeasentId = 0;
        peasentGUIDs.clear();
        _events.ScheduleEvent(EVENT_FIND_PEASENTS, 4s);
    }

    void SetData(uint32 /*type*/, uint32 data) override
    {
        if (data == 1)
        {
            ++_PeasentId;
            if (_PeasentId == 5) _PeasentId = 0;
            _events.ScheduleEvent(EVENT_NEXT_PEASENT, 2s, 6s);
        }
    }

    void CallPeasent()
    {
        if (Creature* peasent = ObjectAccessor::GetCreature(*me, peasentGUIDs[_PeasentId]))
            peasent->AI()->SetData(1, 1);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        if (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_FIND_PEASENTS:
                {
                    GuidVector tempGUIDs;
                    std::list<Creature*> peasents;
                    GetCreatureListWithEntryInGrid(peasents, me, NPC_EASTVALE_PEASENT, 100.f);
                    for (Creature* peasent : peasents)
                    {
                        tempGUIDs.push_back(peasent->GetGUID());
                    }
                    peasentGUIDs.push_back(tempGUIDs[2]);
                    peasentGUIDs.push_back(tempGUIDs[3]);
                    peasentGUIDs.push_back(tempGUIDs[0]);
                    peasentGUIDs.push_back(tempGUIDs[1]);
                    peasentGUIDs.push_back(tempGUIDs[4]);
                    _events.ScheduleEvent(EVENT_NEXT_PEASENT, 1s);
                    break;
                }
                case EVENT_NEXT_PEASENT:
                    CallPeasent();
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
private:
    EventMap _events;
    uint8 _PeasentId;
    GuidVector peasentGUIDs;
};

/*######
## npc_eastvale_peasent
######*/

enum EastvalePeasent
{
    EVENT_MOVETORAELEN                = 10,
    EVENT_TALKTORAELEN1               = 11,
    EVENT_TALKTORAELEN2               = 12,
    EVENT_RAELENTALK                  = 13,
    EVENT_TALKTORAELEN3               = 14,
    EVENT_TALKTORAELEN4               = 15,
    EVENT_PATHBACK                    = 16,
    NPC_SUPERVISOR_RAELEN             = 10616,
    PATH_PEASENT_0                    = 813490,
    PATH_PEASENT_1                    = 813480,
    PATH_PEASENT_2                    = 812520,
    PATH_PEASENT_3                    = 812490,
    PATH_PEASENT_4                    = 812500,
    SAY_RAELEN                        = 0,
    SOUND_PEASENT_GREETING_1          = 6289,
    SOUND_PEASENT_GREETING_2          = 6288,
    SOUND_PEASENT_GREETING_3          = 6290,
    SOUND_PEASENT_LEAVING_1           = 6242,
    SOUND_PEASENT_LEAVING_2           = 6282,
    SOUND_PEASENT_LEAVING_3           = 6284,
    SOUND_PEASENT_LEAVING_4           = 6285,
    SOUND_PEASENT_LEAVING_5           = 6286,
    SPELL_TRANSFORM_PEASENT_WITH_WOOD = 9127
};

struct npc_eastvale_peasent : public ScriptedAI
{
    npc_eastvale_peasent(Creature* creature) : ScriptedAI(creature)
    {
        Initialize();
    }

    void Initialize()
    {
        _path = me->GetSpawnId() * 10;
    }

    void Reset() override {}

    void SetData(uint32 /*type*/, uint32 data) override
    {
        if (data == 1)
        {
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
            me->CastSpell(me, SPELL_TRANSFORM_PEASENT_WITH_WOOD);
            me->SetSpeed(MOVE_WALK, 1.0f);
            me->GetMotionMaster()->MovePath(_path, false);
        }
    }

    void PathEndReached(uint32 pathId) override
    {
        if (pathId == _path)
        {
            CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(me->GetEntry());
            me->SetSpeed(MOVE_WALK, cinfo->speed_walk);
            me->RemoveAura(SPELL_TRANSFORM_PEASENT_WITH_WOOD);
            _events.ScheduleEvent(EVENT_MOVETORAELEN, 3s);
        }
        else if (pathId == _path + 1)
        {
            _events.ScheduleEvent(EVENT_TALKTORAELEN1, 1s);
        }
        else if (pathId == _path + 2)
        {
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_WORK_CHOPWOOD);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        if (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_MOVETORAELEN:
                me->GetMotionMaster()->MovePath(_path + 1, false);
                break;
            case EVENT_TALKTORAELEN1:
                if (Creature* realen = me->FindNearestCreature(NPC_SUPERVISOR_RAELEN, 2.0f, true))
                {
                    _realenGUID = realen->GetGUID();
                    me->SetFacingToObject(realen);

                    switch (_path)
                    {
                    case PATH_PEASENT_0:
                        me->PlayDirectSound(SOUND_PEASENT_GREETING_1);
                        _events.ScheduleEvent(EVENT_TALKTORAELEN2, 2s);
                        break;
                    case PATH_PEASENT_1:
                    case PATH_PEASENT_3:
                        me->PlayDirectSound(SOUND_PEASENT_GREETING_3);
                        _events.ScheduleEvent(EVENT_RAELENTALK, 2s);
                        break;
                    case PATH_PEASENT_2:
                    case PATH_PEASENT_4:
                        me->PlayDirectSound(SOUND_PEASENT_GREETING_2);
                        _events.ScheduleEvent(EVENT_RAELENTALK, 2s);
                        break;
                    }
                }
                else
                {
                    // Path back if realen cannot be found alive
                    _events.ScheduleEvent(EVENT_PATHBACK, 2s);
                }
                break;
            case EVENT_TALKTORAELEN2:
                me->PlayDirectSound(SOUND_PEASENT_GREETING_2);
                _events.ScheduleEvent(EVENT_RAELENTALK, 2s);
                break;
            case EVENT_RAELENTALK:
                if (Creature* realen = ObjectAccessor::GetCreature(*me, _realenGUID))
                {
                    realen->AI()->Talk(SAY_RAELEN);
                    _events.ScheduleEvent(EVENT_TALKTORAELEN3, 5s);
                }
                break;
            case EVENT_TALKTORAELEN3:
                {
                    switch (_path)
                    {
                    case PATH_PEASENT_0:
                        me->PlayDirectSound(SOUND_PEASENT_LEAVING_1);
                        _events.ScheduleEvent(EVENT_PATHBACK, 2s);
                        break;
                    case PATH_PEASENT_1:
                    case PATH_PEASENT_3:
                        me->PlayDirectSound(SOUND_PEASENT_LEAVING_4);
                        _events.ScheduleEvent(EVENT_TALKTORAELEN4, 2s);
                        break;
                    case PATH_PEASENT_2:
                        me->PlayDirectSound(SOUND_PEASENT_LEAVING_3);
                        _events.ScheduleEvent(EVENT_PATHBACK, 2s);
                        break;
                    case PATH_PEASENT_4:
                        me->PlayDirectSound(SOUND_PEASENT_LEAVING_2);
                        _events.ScheduleEvent(EVENT_PATHBACK, 2s);
                        break;
                    }
                }
                break;
            case EVENT_TALKTORAELEN4:
                me->PlayDirectSound(SOUND_PEASENT_LEAVING_5);
                _events.ScheduleEvent(EVENT_PATHBACK, 2s);
                break;
            case EVENT_PATHBACK:
                if (Creature* realen = ObjectAccessor::GetCreature(*me, _realenGUID))
                    realen->AI()->SetData(1, 1);
                me->GetMotionMaster()->MovePath(_path + 2, false);
                break;
            }
        }

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
    ObjectGuid _realenGUID;
    uint32 _path;
};

void AddSC_elwynn_forest()
{
    new npc_catchup_leveler();
    new npc_startzone_teleporter();
    RegisterCreatureAI(npc_cameron);
    RegisterCreatureAI(npc_supervisor_raelen);
    RegisterCreatureAI(npc_eastvale_peasent);
}
