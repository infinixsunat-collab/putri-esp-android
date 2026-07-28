#include "kmods.h"
#include "socket.h"
#include "StructsCommon.h"

using namespace std;

#define maxplayerCount 30
#define maxmonsterCount 30

bool Drone = false;
int DroneHeights = 0;
int MinimapSizee = 105;
int MinimapPoss = 340;

enum Mode {
	InitMode = 1,
	ESPMode = 2,
	HackMode = 3,
	StopMode = 4,
};

struct SetValue {
    int mode;
    int type;
};

struct CoolDownData {
    int skill1;
    int skill2;
    int skill3;
    int skill4;
    int spell;
};

struct PlayerData {
	bool isBot;
    int HeroLevel;
    int HeroID;
    float Health;
	float HealthMax;
    float Distance;
    Vector2 Location;
	char PlayerName[CHAR_MAX];
    char CoolDown[CHAR_MAX];
    CoolDownData coolDownData;
    Vector2 RadarPos;
};

struct MonsterData {
    Vector2 position;
    int m_ID;
    float Health;
    float HealthMax;
    int MonsterType;
};

struct TeamInfo {
    int heroID;
    char Information[CHAR_MAX];
};

struct EnemyInfo {
    int heroID;
    char Information[CHAR_MAX];
};

struct Request {
    int Mode;
    int ScreenWidth;
    int ScreenHeight;
	bool DroneView = false;
    int DroneHeight = 0;
	int MinimapPos = 105;
    int MinimapSize = 340;
	int IconSize = 140;
};

struct Response {
    bool Success;
    int PlayerCount;
	int MonsterCount;
	int TeamCount;
    int EnemyCount;
	Vector2 localPos;
    PlayerData Players[maxplayerCount];
	MonsterData Monsters[maxmonsterCount];
	TeamInfo teamInfo[10];
    EnemyInfo enemyInfo[10];
};

CoolDownData getSkillCoolDown(uintptr_t show_entity, int HeroID) {
    CoolDownData result{};
    auto cdComp = Read<uintptr_t>(show_entity + 0x100);
    if (!cdComp) return {};

    auto dicCoolInfoPtr1 = Read<uintptr_t>(cdComp + 0x18);
    if (!dicCoolInfoPtr1) return {};

    auto dicCoolInfoPtr = Read<monoDictionary>(dicCoolInfoPtr1);
    auto items = get_mono_dictionary_vector<int, uintptr_t>(dicCoolInfoPtr);

    auto now = Read<uint>(GetLogicBattleManagerInstance() + 0x194);//LogicBattleManager::m_uiFrameTime

    for (auto dictionaryEntry: items) {
        auto skillID = dictionaryEntry.key;
        auto coolDownData = dictionaryEntry.value;
        if (skillID < 0 || !coolDownData)
            continue;

        auto fSkillID = skillID % 100;

        auto startTime = Read<uint>(coolDownData + 0x1c);//CoolDownData::uiStartTime
        auto coolDownTime = Read<uint>(coolDownData + 0x14);//CoolDownData::uiCoolTime
        auto coolTime = coolDownTime + startTime;
        if (coolTime < now)
            coolTime = now;

        auto isCoolDown = (coolTime - now) == 0;
        auto timeLeft = (coolTime - now) / 1000;

        if (skillID < 20000) {
            if (HeroID == 126) {
                switch (fSkillID) {//Suyou
                    case 10:
                        result.skill1 = !isCoolDown ? timeLeft : 0;
                        break;
                    case 30:
                        result.skill2 = !isCoolDown ? timeLeft : 0;
                        break;
                    case 50:
                        result.skill3 = !isCoolDown ? timeLeft : 0;
                        break;
                    default:
                        break;
                }
            } else {
                switch (fSkillID) {
                    case 10:
                        result.skill1 = !isCoolDown ? timeLeft : 0;
                        break;
                    case 20:
                        result.skill2 = !isCoolDown ? timeLeft : 0;
                        break;
                    case 30:
                        result.skill3 = !isCoolDown ? timeLeft : 0;
                        break;
                    case 40:
                        result.skill4 = !isCoolDown ? timeLeft : -1;
                        break;
                    default:
                        break;
                }
            }
        } else if (getSpell(skillID)) {
            result.spell = !isCoolDown ? timeLeft : 0;
        }
    }
    return result;
}

int ListMonsterId[] = {
    2002,
    2003,
    2004,
    2005,
    2006,
    2008,
    2009,
    2011,
    2012,
    2013,
    2056,
    2059,
    2072,
};

bool bMonster(int iValue) {
    return std::find(std::begin(ListMonsterId), std::end(ListMonsterId), iValue) != std::end(ListMonsterId);
}
/*
void OnRoomInfoThread(Response &response) {
    auto logic_battle_mgr = GetLogicBattleManagerInstance();
    if (!logic_battle_mgr) {
        LOGE("logic_battle_mgr");
        return;
    }

    int battleState = Read<int>(logic_battle_mgr + 0x178);
    if (battleState == 2 || battleState == 3) {
        auto system_data = GetSystemData();
        if (!system_data)
            return;

        auto m_RoomPlayerInfo = Read<uintptr_t>(system_data + 0x308);
        if (!m_RoomPlayerInfo)
            return;

        int iSelfCamp = -1;
        int m_uiID = Read<int>(system_data + 0x318);
        auto roomInfoDict = Read<monoDictionary>(m_RoomPlayerInfo);
        auto items = get_mono_dictionary_vector<int, uintptr_t>(roomInfoDict);
        for (auto room: items) {
            auto m = Read<monoList>(room.value);
            auto array = get_mono_list_vector<uintptr_t>(m);
            for (auto info: array) {
                auto iCamp = Read<int>(info + 0x30);
                auto lUid = Read<int>(info + 0x20);
                if (lUid != m_uiID) continue;
                iSelfCamp = iCamp;
            }
            for (auto info: array) {
                auto iCamp = Read<int>(info + 0x30);
                auto lUid = Read<int>(info + 0x20);
                auto uiZoneId = Read<int>(info + 0x60);
                auto heroid = Read<int>(info + 0x4c);
                auto uiHeroIDChoose = Read<int>(info + 0x1f8);
                auto uiRankLevel = Read<int>(info + 0x128);
                auto iMythPoint = Read<int>(info + 0x1cc);
                auto PlayerName = Read<uintptr_t>(info + 0x40);
                auto sCreateRoleCountry = Read<uintptr_t>(info + 0xd0);
                auto Rank = RankToString(uiRankLevel, iMythPoint);
                if (iCamp == iSelfCamp) {
                    auto *teamRoom = &response.teamInfo[response.TeamCount];
                    teamRoom->heroID = (uiHeroIDChoose) ? uiHeroIDChoose : heroid;
                    std::string information = (getCharacterName(PlayerName).empty() ? std::to_string(lUid) + " (" + std::to_string(uiZoneId) + ")" : getCharacterName(PlayerName)) + " | " + Rank + " | <CID>" + getCharacterName(sCreateRoleCountry) +"<CID>";
                    sprintf(teamRoom->Information, "%s", information.c_str());
                    response.TeamCount++;
                } else {
                    auto *enemyRoom = &response.enemyInfo[response.EnemyCount];
                    enemyRoom->heroID = (uiHeroIDChoose) ? uiHeroIDChoose : heroid;
                    std::string information = (getCharacterName(PlayerName).empty() ? std::to_string(lUid) + " (" + std::to_string(uiZoneId) + ")" : getCharacterName(PlayerName)) + " | " + Rank + " | <CID>" + getCharacterName(sCreateRoleCountry) +"<CID>";
                    sprintf(enemyRoom->Information, "%s", information.c_str());
                    response.EnemyCount++;
                }
            }
        }
    }
}
*/
void UpdateCoordinator(Response &response);
void _3rdPartyFix(){
    system("setenforce 0");
    system("echo 8192 > /proc/sys/fs/inotify/max_user_watches");
    system("echo 8192 > /proc/sys/fs/inotify/max_queued_events");
    system("echo 8192 > /proc/sys/fs/inotify/max_user_instances");
    system("settings put global block_untrusted_touches 0");
}


SetValue sv{};
int main(int argc, char*argv[]){
        
     if (!Create()) {
         perror("Creation Failed");
         return 0;
     }
     if (!Connect()) {	 
		 perror("Connection Failed");
         return 0;
     }
     LOGI("[Server] Socket connected");
     int no;
     receive((void*)&sv);
     no = sv.mode;    
     if (no == 1) {
         Offsets::globalOffsets();
         target_pid = find_pid("com.mobile.legends:UnityKillsMe");
     } 
     if (Height > Width)
     {
         int t = Width;
         Width = Height;
         Height = t;
     }
     LOGI("Device screen:");
     LOGI("Width : %d", (int) Width);
     LOGI("Height : %d", (int) Height);
     if (target_pid < 0){
         LOGE("[Server] Can't get game pid");
         exit(1);
     }
     libbase = getBase("libcsharp.so");
     if (libbase < 0){
         LOGE("[Server] Can't get game base");
         exit(1);
     }
	 _3rdPartyFix();
    Request request{};
    while ((receive((void*)&request) > 0)){    
        Width = request.ScreenWidth;
        Height = request.ScreenHeight;
	    Drone = request.DroneView;
		DroneHeights = request.DroneHeight;
		MinimapSizee = request.MinimapSize;
		MinimapPoss = request.MinimapPos;
        Response response{};
        response.Success = true;
        response.PlayerCount = 0;
		response.MonsterCount = 0;
		response.EnemyCount = 0;
		response.TeamCount = 0;
		response.localPos = Vector2::Zero();
		//OnRoomInfoThread(response);
        UpdateCoordinator(response);
        send((void*)&response, sizeof(response));
    }
    Close();
    return 0;
}
 
void UpdateCoordinator(Response& response) {
	auto Instance = GetGameInstance();
    if (!Instance) {
        LOGE("Instance");
        return;
    }
	
    auto main_cam = GetMainCamera();
    if (!main_cam)
        return;

    auto Internal_cam = Read<uintptr_t>(main_cam + 0x10);
    if (!Internal_cam)
        return;

	SetFieldOfView(Internal_cam, Drone ? 30 + DroneHeights : 30);
	
    auto ViewMatrix = Read<Camera>(Internal_cam + 0x5C);
	
    _vMatrix = ViewMatrix.projectionMatrix * ViewMatrix.worldToCameraMatrix;

    auto localPlayer = Read<uintptr_t>(Instance + 0x50);
    if (!localPlayer) {
        LOGE("localPlayer");
        return;
    }

    auto localCamp = Read<int>(localPlayer + 0xd8);//ShowEntity::m_EntityCampType

    auto local_pos = Read<Vector3>(localPlayer + 0x294);
	
    WorldToScreen(local_pos, &response.localPos);

    auto m_PawnsPtr = Read<uintptr_t>(Instance + 0x78);//BattleManager::_showPlayers
    if (!m_PawnsPtr) {
        LOGE("m_PawnsPtr");
        return;
    }
    auto m_entityList = Read<monoList>(m_PawnsPtr);
    auto array = get_mono_list_vector<uintptr_t>(m_entityList);

    for (auto entity : array) {
        if (!entity) continue;

        auto m_bDeath = Read<bool>(entity + 0xcd);//ShowEntity::m_bDeath
        if (m_bDeath) continue;

        auto entityCamp = Read<int>(entity + 0xd8);//ShowEntity::m_EntityCampType
        if (entityCamp == localCamp) continue;
		
        auto HeroID = Read<int>(entity + 0x194);//ShowEntity::m_ID

        auto en_pos = Read<Vector3>(entity + 0x294);//ShowEntity::m_vCachePosition

        Vector2 en_posSc;
        WorldToScreen(en_pos, &en_posSc);

        auto entityObj = Read<uintptr_t>(entity + 0x468);//ShowEntity::m_RoleName
        auto playerName = getCharacterName(entityObj);

        auto Distance = Vector3::Distance(local_pos, en_pos);

        auto _bAiControl = Read<bool>(entity + 0x419);//ShowEntity::_bAiControl

        auto m_Level = Read<int>(entity + 0x198);//ShowEntity::m_Level

        int m_Hp = Read<int>(entity + 0x1ac);//ShowEntity::m_Hp
        int m_HpMax = Read<int>(entity + 0x1b0);//ShowEntity::m_HpMax
		
	    auto radar_pos = WorldToMinimap(entityCamp, en_pos, MinimapSizee, MinimapPoss);
		CoolDownData coolDownData = getSkillCoolDown(entity, HeroID);
		
        PlayerData* data = &response.Players[response.PlayerCount];
        data->Health = (float)m_Hp;
	    data->HealthMax = (float)m_HpMax;
        data->HeroID = HeroID; 
        data->HeroLevel = m_Level;
        data->isBot = _bAiControl;
	    data->Location = en_posSc;
	    data->Distance = Distance;
		data->coolDownData = coolDownData;
		data->RadarPos = radar_pos;

        sprintf(data->PlayerName, "%s", playerName.c_str());
 
	    if (response.PlayerCount >= maxplayerCount) 
	    {
            continue;
        }
        response.PlayerCount++;
    }
}
