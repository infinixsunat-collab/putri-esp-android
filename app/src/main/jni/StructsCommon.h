#ifndef STRUCTSCOMM_H
#define STRUCTSCOMM_H

#include "Vector3.h"
#include "Vector2.h"
#include "Rect.h"

using namespace std;

#define maxplayerCount 100
#define maxmonsterCount 100
enum Mode {
	InitMode = 1,
	ESPMode = 2,
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

#endif
