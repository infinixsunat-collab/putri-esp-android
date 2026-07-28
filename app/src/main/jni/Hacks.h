#ifndef HACKS_H
#define HACKS_H
#include "jancok.h"
#include "Login.h"
#include <chrono>
#include <math.h>

Request request;
Response response;
Color clrNew;
int GetRealHeroID(int HeroId) {
    switch (HeroId) {
        case 400:
            return 1;
        case 401:
            return 2;
        case 402:
            return 3;
        case 403:
            return 4;
        case 404:
            return 5;
        case 405:
            return 6;
        case 406:
            return 7;
        case 407:
            return 8;
        case 408:
            return 9;
        case 409:
            return 10;
        case 410:
            return 11;
        case 411:
            return 4;
        case 414:
            return 14;
        case 416:
            return 16;
        case 418:
            return 18;
        case 419:
            return 18;
        case 420:
            return 20;
        case 422:
            return 22;
        case 423:
            return 18;
        case 424:
            return 35;
        case 425:
            return 7;
        case 426:
            return 2;
        case 427:
            return 4;
        case 428:
            return 4;
        case 429:
            return 22;
        case 430:
            return 18;
        case 431:
            return 4;
        case 432:
            return 2;
        case 433:
            return 2;
        case 434:
            return 1;
        case 435:
            return 36;
        case 436:
            return 4;
        case 437:
            return 18;
        case 438:
            return 1;
        case 439:
            return 1;
        case 440:
            return 15;
        case 441:
            return 2;
        case 442:
            return 2;
        case 443:
            return 15;
        case 444:
            return 2;
        case 445:
            return 15;
        case 446:
            return 18;
        case 447:
            return 16;
        case 448:
            return 4;
        case 449:
            return 2;
        case 450:
            return 2;
        case 451:
            return 4;
        case 452:
            return 72;
        case 453:
            return 6;
        case 454:
            return 72;
        case 455:
            return 85;
        case 456:
            return 15;
        case 457:
            return 2;
        case 458:
            return 85;
        case 459:
            return 4;
        case 460:
            return 2;
        case 461:
            return 57;
        case 462:
            return 60;
        case 463:
            return 85;
        case 464:
            return 2;
        case 465:
            return 18;
        case 466:
            return 18;
        case 467:
            return 1;
        case 468:
            return 57;
        case 469:
            return 35;
        case 470:
            return 60;
        case 471:
            return 15;
        case 472:
            return 85;
        case 473:
            return 2;
        case 474:
            return 7;
        case 475:
            return 4;
        case 476:
            return 15;
        case 477:
            return 14;
        case 478:
            return 6;
        case 479:
            return 4;
        case 480:
            return 2;
        case 481:
            return 60;
        case 482:
            return 2;
        case 483:
            return 18;
        case 484:
            return 6;
        case 485:
            return 1;
        case 486:
            return 35;
        case 487:
            return 16;
        case 488:
            return 15;
        case 489:
            return 14;
        case 490:
            return 8;
        case 491:
            return 18;
        case 492:
            return 2;
        case 493:
            return 85;
        case 494:
            return 5;
        case 495:
            return 28;
        case 496:
            return 18;
        case 501:
            return 2;
        case 502:
            return 6;
        case 503:
            return 15;
        case 504:
            return 16;
        case 505:
            return 18;
        case 506:
            return 3;
        case 507:
            return 5;
        case 508:
            return 7;
        case 509:
            return 2;
        case 510:
            return 85;
        case 601:
            return 2;
        case 701:
            return 7;
        case 702:
            return 28;
        case 703:
            return 2;
        case 704:
            return 11;
        case 705:
            return 18;
        case 706:
            return 35;
        case 707:
            return 57;
        case 708:
            return 60;
        case 709:
            return 85;
        case 710:
            return 14;
        case 711:
            return 3;
        case 712:
            return 4;
        case 801:
            return 2;
        case 802:
            return 3;
        case 803:
            return 4;
        case 804:
            return 6;
        case 805:
            return 7;
        case 806:
            return 9;
        case 807:
            return 11;
        case 808:
            return 12;
        case 809:
            return 16;
        case 810:
            return 17;
        case 811:
            return 19;
        case 812:
            return 21;
        case 813:
            return 22;
        case 814:
            return 26;
        case 815:
            return 27;
        case 816:
            return 28;
        case 817:
            return 29;
        case 818:
            return 32;
        case 819:
            return 33;
        case 820:
            return 35;
        case 821:
            return 37;
        case 822:
            return 1;
        case 823:
            return 13;
        case 824:
            return 18;
        case 825:
            return 8;
        case 826:
            return 85;
        case 827:
            return 57;
        case 828:
            return 60;
        case 829:
            return 14;
        case 830:
            return 3;
        case 831:
            return 28;
        case 832:
            return 5;
        case 833:
            return 6;
        case 834:
            return 14;
        case 835:
            return 2;
        case 836:
            return 4;
        case 837:
            return 85;
        case 838:
            return 7;
        case 839:
            return 1;
        case 9994:
            return 18;
        case 9995:
            return 18;
        case 9996:
            return 16;
        case 9997:
            return 6;
        case 9998:
            return 16;
        case 9999:
            return 18;
        default:
            return HeroId;
    }
}
Color teamIdColors[] =
        {
                Color::Red(),
                Color::Yellow(),
                Color::Orange(),
                Color::BlueDongker(),
                Color::Pink(),
                Color::Green(),
                Color::Grey(),
                Color::SilverDark(),
                Color::Blue()
        };

Color colorByDistance(int distance){
    Color _colorByDistance;
    if (distance < 450)
        _colorByDistance = Color(0,255,0);
    if (distance < 200)
        _colorByDistance = Color(255,255,0);
    if (distance < 120)
        _colorByDistance = Color(255,133,51);
    if (distance < 50)
        _colorByDistance = Color(255,0,0);
    return _colorByDistance;
}

uint64_t GetTickCount() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}
std::string MonsterToString(int m_id) {
    std::string strMonster;
    switch(m_id) {
    case 2002:
        strMonster += "Lord";
        break;
    case 2003:
        strMonster += "Turtle";
        break;
    case 2004:
        strMonster += "Fiend";
        break;
    case 2005:
        strMonster += "Serpent";
        break;
    case 2006:
        strMonster += "Scaled Lizard";
        break;
    case 2008:
        strMonster += "Crammer";
        break;
    case 2009:
        strMonster += "Rockursa";
        break;
    case 2011:
        strMonster += "Crab";
        break;
    case 2012:
        strMonster += "Serpent kids";
        break;
    case 2013:
        strMonster += "Crab";
        break;
    case 2056:
        strMonster += "Lithowanderer";
        break;
    case 2059:
        strMonster += "Crammer";
        break;
    case 2072:
        strMonster += "Lithowanderer";
        break;
    default:
        strMonster += ""/*to_string(m_id)*/;
    }
    return strMonster;
}
class Interval {
private:
    int initial_;

public:
    inline Interval() : initial_(GetTickCount()) {}

    virtual ~Interval() {}

    inline unsigned int value() const {
        return GetTickCount() - initial_;
    }
};

class FPS {
protected:
    int32_t m_fps;
    int32_t m_fpscount;
    Interval m_fpsinterval;

public:
    FPS() : m_fps(0), m_fpscount(0) {}

    void Update() {
        m_fpscount++;
        if (m_fpsinterval.value() > 1000) {
            m_fps = m_fpscount;
            m_fpscount = 0;
            m_fpsinterval = Interval();
        }
    }

    int32_t get() const {
        return m_fps;
    }
};
bool isValidPlayer(PlayerData data){
	return (data.Location != Vector2::Zero());
}

bool isOutsideScreen(Vector2 pos, Vector2 screen) {
    if (pos.y < 0) {
        return true;
    }
    if (pos.x > screen.x) {
        return true;
    }
    if (pos.y > screen.y) {
        return true;
    }
    return pos.x < 0;
}

void FindPoint(Vector2 origin, Vector2 &point, int screenwidth, int screenheight, int length)
{
    float halfScreenWidth = screenwidth / 2.0f;
    float halfScreenHeight = screenheight / 2.0f;
    float halfScreenWidth2 = (screenwidth - length) / 2.0f;
    float halfScreenHeight2 = (screenheight - length) / 2.0f;
    float dx = fabs(origin.x - halfScreenWidth);
    float dy = fabs(origin.y - halfScreenHeight);
    float rx = (dx != 0) ? halfScreenWidth2 / dx : 0;
    float ry = (dy != 0) ? halfScreenHeight2 / dy : 0;
    float r = fmin(rx, ry);
    point.x = origin.x + (halfScreenWidth - origin.x) * (1.0f - r);
    point.y = origin.y + (halfScreenHeight - origin.y) * (1.0f - r);
}

FPS m_FPS;
void DrawESP(ESP esp, int screenWidth, int screenHeight) {
   if (screenWidth < screenHeight) return;
   m_FPS.Update();
   std::string mFPS = "Putri FPS : ";
   mFPS += std::to_string(m_FPS.get());
   esp.DrawText(Color::White(), mFPS.c_str(), {120, 120}, screenWidth / 50);
   send((void*)&request,sizeof(request));
   receive((void*)&response);     
   Vector2 screen(screenWidth, screenHeight);
   float mScale = screenHeight / (float) 1080;      
   float textSize = screenHeight / 45;
   float textSize2 = screenHeight / 25;
   float InfoPos = screenHeight / 21.6;
   float lineSize = screenHeight / 432;
   float MinimapIconSize = request.IconSize;
   request.ScreenHeight=screenHeight;
   request.ScreenWidth=screenWidth;
   request.DroneView = Drone;
   request.DroneHeight = droneH;
   request.IconSize = iconSizee;
   request.MinimapSize = minimapSizee;
   request.MinimapPos = minimapPoss;
   if (response.Success){	    
       if (isShowTeamRoom) {
            float IconSize = screenWidth / 10.4;
            int32_t TeamRoomCount = response.TeamCount;
            for (int i = 0; i < TeamRoomCount; i++) {
                auto *TeamInfo = &response.teamInfo[i];
                Vector2 infoLoc{InfoPos, InfoPos + textSize2 * (i * 2)};
                Vector2 p_min = {infoLoc.x - ((IconSize / 3) / 2), infoLoc.y - ((IconSize / 3) / 2)};
                if (TeamInfo->heroID != 0) {
                    esp.DrawBase64Bitmap(TeamInfo->heroID, IconSize / 3, IconSize / 3, {p_min.x, p_min.y});
                }
                esp.DrawTextA(Color::Blue(), TeamInfo->Information, {infoLoc.x + ((IconSize / 3) / 2), infoLoc.y + ((IconSize / 3) / 4)}, textSize2, Align::LEFT);
            }
       }
	   if (isShowEnemyRoom) {
            float IconSize = screenWidth / 10.4;
            int32_t EnemyRoomCount = response.EnemyCount;
            for (int i = 0; i < EnemyRoomCount; i++) {
                auto *EnemyInfo = &response.enemyInfo[i];
                Vector2 infoLoc{screenWidth - InfoPos, InfoPos + textSize2 * (i * 2)};
                Vector2 p_min = {infoLoc.x - ((IconSize / 3) / 2), infoLoc.y - ((IconSize / 3) / 2)};
                if (EnemyInfo->heroID != 0) {
                    esp.DrawBase64Bitmap(EnemyInfo->heroID, IconSize / 3, IconSize / 3, {p_min.x, p_min.y});
                }
                esp.DrawTextA(Color::Red(), EnemyInfo->Information, {infoLoc.x - ((IconSize / 3) / 2), infoLoc.y + ((IconSize / 3) / 4)}, textSize2, Align::RIGHT);
            }
       }
       if (isDrawRadar) {
           esp.DrawRect2(Color::Green(), 2.f, {static_cast<float>(request.MinimapPos), 0}, {static_cast<float>(request.MinimapPos + request.MinimapSize), static_cast<float>(request.MinimapSize)});
       }
       int count = response.PlayerCount;
       if (count > 0){
           for (int i=0; i < count; i++){
                PlayerData player = response.Players[i];
				float w_dist = player.Distance;
                bool isOutScreen;
                float IconSize = screenWidth / 10.4;
				Vector2 HeroPos = {player.Location.x, player.Location.y};
                Vector2 Res;
                if (HeroPos.x < 0 || HeroPos.x > screenWidth || HeroPos.y < 0 ||
                    HeroPos.y > screenHeight) {
                    isOutScreen = true;
					IconSize = screenWidth / 15.6;
                    FindPoint(HeroPos, Res, screenWidth, screenHeight, (IconSize / 3));
                } else {
                    isOutScreen = false;
                    Res = HeroPos;
                }
				
                float x = player.Location.x;
                float y = player.Location.y;

				char buffer[32];
				sprintf(buffer, "%dm", (int)std::round(w_dist));
				auto sDistance = std::string(buffer);
					
                clrNew = Color(255, 0, 0);
                    
                Color _colorByDistance = colorByDistance((int) player.Distance);

				if (isPlayerLine) {
                    esp.DrawLine(colorByDistance((int) std::round(w_dist)), lineSize, response.localPos, {x, y});
                }
				 
				if (isPlayerName) {
                std::string name_str;
                if (isPlayerName) {
                    char buffer[10];
					sprintf(buffer, "(%d) ", player.HeroLevel);
					name_str += buffer;
                }

                if (isPlayerName) {
                    name_str += player.PlayerName;
                    if (player.isBot) {
                        name_str += " [Bot]";
                    }
                }

                auto name = name_str.c_str();
                Vector2 textPos = {Res.x - (((IconSize / 3) / 2) + 5), Res.y};
                Vector2 textPos2 = {Res.x + (((IconSize / 3) / 2) + 5), Res.y};
                if (!isOutScreen) {
                    textPos.y -= textSize;
                    textPos2.y -= textSize;
                }
                if (HeroPos.x > screenWidth) {
                    esp.DrawTextA(Color::White(), name, textPos, textSize, Align::RIGHT);
                } else {
                    esp.DrawTextA(Color::White(), name, textPos2, textSize, Align::LEFT);
                }
            }

            if (isPlayerDist) {
                char buffer[32];
			    sprintf(buffer, "%dm", (int)std::round(w_dist));
				auto dist_ = std::string(buffer);
                auto dist_str = dist_.c_str();
                Vector2 textPos = {Res.x - (((IconSize / 3) / 2) + 5), Res.y};
                Vector2 textPos2 = {Res.x + (((IconSize / 3) / 2) + 5), Res.y};
                if (isOutScreen) {
                    textPos.y += textSize;
                    textPos2.y += textSize;
                }
                if (HeroPos.x > screenWidth) {
                    esp.DrawTextA(Color::Yellow(), dist_str, textPos, textSize, Align::RIGHT);
                } else {
                    esp.DrawTextA(Color::Yellow(), dist_str, textPos2, textSize, Align::LEFT);
                }
            }

            if (isDrawCoolDown && !isOutScreen) {
                auto skill1 = player.coolDownData.skill1 == 0 ? "❤️" : std::to_string(player.coolDownData.skill1);
				auto skill2 = player.coolDownData.skill2 == 0 ? "❤️" : std::to_string(player.coolDownData.skill2);
				auto skill3 = player.coolDownData.skill3 == 0 ? "❤️" : std::to_string(player.coolDownData.skill3);
				auto skill4 = player.coolDownData.skill4 == 0 ? "" : (player.coolDownData.skill4 == -1 ? "❤️" : std::to_string(player.coolDownData.skill4));
				auto spell = player.coolDownData.spell == 0 ? "💛" : std::to_string(player.coolDownData.spell);

				auto cooldown_ = skill1 + " " + skill2 + " " + skill3 + " " + skill4 + " | " + spell;

                auto cooldown_str = cooldown_.c_str();
                if (HeroPos.x > screenWidth) {
                    esp.DrawTextA(Color::White(), cooldown_str, {Res.x - (((IconSize / 3) / 2) + 5), Res.y + textSize}, textSize, Align::RIGHT);
                } else {
                    esp.DrawTextA(Color::White(), cooldown_str, {Res.x + ((IconSize / 3) / 2) + 5, Res.y + textSize}, textSize, Align::LEFT);
                }
            }


            if (isPlayerHealth) {
                float max_health = player.HealthMax;
                float health = player.Health;
                float a_max = ((M_PI * 2.0f));
                esp.DrawCircularHealth(Color::Red(), health, max_health, Res, IconSize / 5.5);
            }

            if (isPlayerBox) {
                int HeroID = GetRealHeroID(player.HeroID);
                Vector2 p_min = {Res.x - ((IconSize / 3) / 2), Res.y - ((IconSize / 3) / 2)};
                esp.DrawBase64Bitmap(HeroID, IconSize / 3, IconSize / 3, {p_min.x, p_min.y});
            }
            Vector2 RadarLocation = {player.RadarPos.x, player.RadarPos.y};
            if (RadarLocation != Vector2::Zero()) {
                if (isDrawRadar) {
                    float max_health = player.HealthMax;
                    float health = player.Health;
                    esp.DrawCircularHealth(Color(254, 68, 138), health, max_health, RadarLocation, MinimapIconSize / 7);
                    int HeroID = GetRealHeroID(player.HeroID);
                    Vector2 p_min = {player.RadarPos.x - ((MinimapIconSize / 4) / 2), player.RadarPos.y- ((MinimapIconSize / 4) / 2)};
                    esp.DrawBase64Bitmap(HeroID, MinimapIconSize / 4, MinimapIconSize / 4, {p_min.x, p_min.y});
                }
            }  
		    if (isPlayer360) {
                Color _360warningColor = colorByDistance((int) player.Distance);
                float posX = x;
                float posY = y;
                float radius = screenHeight / 15;
                if (posX < -screenWidth / 10 ||
                    posX > screenWidth + screenWidth / 10) {
                    if (posY > screenHeight - screenHeight / 12)
                        posY = screenHeight - screenHeight / 12;
                    else if (posY < screenHeight / 12)
                        posY = screenHeight / 12;
                        if (posX > screenWidth / 2) {
                            esp.DrawCircle2(_360warningColor,
                                                            Vector2(screenWidth, posY), radius);
                            esp.DrawText2(Color::White(), sDistance.c_str(),
                                                           Vector2(screenWidth - screenWidth / 65,
                                                                   posY), 15);
                         } else {
                            esp.DrawCircle2(_360warningColor, Vector2(0, posY),
                                                            radius);
                            esp.DrawText2(Color::White(), sDistance.c_str(),
                                                           Vector2(screenWidth / 65, posY), 15);
					            
                         }				 
                     }
                  }
              }           
		  }
	 }
	 if (isNearEnemy) {            
	     int colors;
         std::string eCountStr;
         if (response.PlayerCount != 0) {
             colors = 2;
         if (response.PlayerCount > 2)
             colors = 2;
         if (response.PlayerCount > 4)
             colors = 3;
         if (response.PlayerCount > 5)
             colors = 4;
             eCountStr = std::to_string((int) response.PlayerCount);
         } else {
             colors = 1;
             eCountStr = "CLEAR";
         }
         esp.DrawEnemyCount(colors, Vector2(screenWidth /2, 20));
         esp.DrawText2(Color::Black(), eCountStr.c_str(), Vector2(screenWidth / 2, 110), 25);
     }	
}


#endif //HACKS_H
