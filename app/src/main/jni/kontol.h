#ifndef KONTOL_H
#define KONTOL_H

#include <jni.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/mman.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cerrno>
#include <sys/un.h>
#include <cstring>
#include <string>
#include <cmath>
#include "StructsCommon.h"
#include "Log.h"

bool isESP = false;
//Player
bool isPlayerName = false;
bool isPlayerDist = false;
bool isPlayerHealth = false;
bool isShowTeamRoom = false;
bool isShowEnemyRoom = false;
bool isPlayerBox = false;
bool isPlayerLine = false;
bool isPlayer360 = false;
bool isNearEnemy= false;
bool ismDrawAvatar = false;
bool ismDrawHealth = false;
bool isDrawCoolDown= false;
bool Drone = false;
bool isDrawRadar = false;
float playerTextSize = 15;
bool BulletTrack = false;
bool AimBot = false;
bool AimIgnoreKnocked = false;
int droneH = 0;
int iconSizee = 140;
int minimapPoss = 105;
int minimapSizee = 340;
#endif //KONTOL_H
