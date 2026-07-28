#ifndef STRUCTSCOMM_H
#define STRUCTSCOMM_H
#include <iostream>
#include <string>
#include <unordered_set>
#include <numeric>
#include <vector>
#include <codecvt>
#include "utf8-cpp/utf8.h"
#include "kmods.h"
#include "Vector2.hpp"
using namespace std;

#define SIZE sizeof(uintptr_t)

struct monoArray {
    void *klas;
    void *obj;
    void *bounds;
    size_t max_length;
    uintptr_t m_Items;
};

struct monoList {
    void *klas;
    void *obj;
    uintptr_t monoArray;
    int pSize;
};

template<typename TKey, typename TValue>
struct monoDictionaryEntry {
    int hashCode, next;
    TKey key;
    TValue value;
};

struct monoDictionary {
    void *klas;
    void *obj;
    int32_t *buckets;
    uintptr_t arrayEntries;
    int count;
    int version;
    int freeList;
    int freeCount;
    uintptr_t comparer;
    uintptr_t keys;
    uintptr_t values;
};

template<typename T>
vector<T> get_mono_list_vector(monoList array) {
    vector<T> output(array.pSize);
    auto address = array.monoArray + offsetof(monoArray, m_Items);
    pvm(address, output.data(), array.pSize * sizeof(T));
    return output;
}

template<typename TKey, typename TValue>
vector<monoDictionaryEntry<TKey, TValue>> get_mono_dictionary_vector(monoDictionary dict) {
    vector<monoDictionaryEntry<TKey, TValue>> output(dict.count);
    auto address = dict.arrayEntries + offsetof(monoArray, m_Items);
    pvm(address, output.data(), dict.count * sizeof(monoDictionaryEntry<TKey, TValue>));
    return output;
}

vector<uintptr_t> get_entity_list(uintptr_t address) {
    monoList list;
    pvm(address, &list, sizeof(monoList));
    return get_mono_list_vector<uintptr_t>(list);
}
/*
monoList getMono(uintptr_t address) {
    monoList list;
    pvm(address, &list, sizeof(monoList));
    return list;
}*/

bool isValid(uintptr_t object) {
     return (object > 0);
}
/*
bool isValid(uintptr_t entity) {
    return entity != 0 && entity != NULL; // Periksa apakah alamat tidak null
}
*/
struct Vector3 {
	float X;
	float Y;
	float Z;

	Vector3() {
		this->X = 0;
		this->Y = 0;
		this->Z = 0;
	}

	Vector3(float x, float y, float z) {
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

	static Vector3 Zero() {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	static Vector3 Up() {
		return Vector3(0.0f, 1.0f, 0.0f);
	}

	static Vector3 Down() {
		return Vector3(0.0f, -1.0f, 0.0f);
	}

	static Vector3 Back() {
		return Vector3(0.0f, 0.0f, -1.0f);
	}

	static Vector3 Forward() {
		return Vector3(0.0f, 0.0f, 1.0f);
	}

	static Vector3 Left() {
		return Vector3(-1.0f, 0.0f, 0.0f);
	}

	static Vector3 Right() {
		return Vector3(1.0f, 0.0f, 0.0f);
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}

	float operator[](int i) const {
		return ((float*)this)[i];
	}

	bool operator==(const Vector3& src) const {
		return (src.X == X) && (src.Y == Y) && (src.Z == Z);
	}

	bool operator!=(const Vector3& src) const {
		return (src.X != X) || (src.Y != Y) || (src.Z != Z);
	}

	Vector3& operator+=(const Vector3& v) {
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}

	Vector3& operator-=(const Vector3& v) {
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		return *this;
	}

	Vector3& operator*=(float fl) {
		X *= fl;
		Y *= fl;
		Z *= fl;
		return *this;
	}

	Vector3& operator*=(const Vector3& v) {
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;
		return *this;
	}

	Vector3& operator/=(const Vector3& v) {
		X /= v.X;
		Y /= v.Y;
		Z /= v.Z;
		return *this;
	}

	Vector3& operator+=(float fl) {
		X += fl;
		Y += fl;
		Z += fl;
		return *this;
	}

	Vector3& operator/=(float fl) {
		X /= fl;
		Y /= fl;
		Z /= fl;
		return *this;
	}

	Vector3& operator-=(float fl) {
		X -= fl;
		Y -= fl;
		Z -= fl;
		return *this;
	}

	Vector3& operator=(const Vector3& vOther) {
		X = vOther.X;
		Y = vOther.Y;
		Z = vOther.Z;
		return *this;
	}

	Vector3 operator-(void) const {
		return Vector3(-X, -Y, -Z);
	}

	Vector3 operator+(const Vector3& v) const {
		return Vector3(X + v.X, Y + v.Y, Z + v.Z);
	}

	Vector3 operator-(const Vector3& v) const {
		return Vector3(X - v.X, Y - v.Y, Z - v.Z);
	}

	Vector3 operator*(float fl) const {
		return Vector3(X * fl, Y * fl, Z * fl);
	}

	Vector3 operator*(const Vector3& v) const {
		return Vector3(X * v.X, Y * v.Y, Z * v.Z);
	}

	Vector3 operator/(float fl) const {
		return Vector3(X / fl, Y / fl, Z / fl);
	}

	Vector3 operator/(const Vector3& v) const {
		return Vector3(X / v.X, Y / v.Y, Z / v.Z);
	}

	static float Dot(Vector3 lhs, Vector3 rhs) {
		return (((lhs.X * rhs.X) + (lhs.Y * rhs.Y)) + (lhs.Z * rhs.Z));
	}

	float sqrMagnitude() const {
		return (X * X + Y * Y + Z * Z);
	}

	float Magnitude() const {
		return sqrt(sqrMagnitude());
	}

	static float Distance(Vector3 a, Vector3 b) {
		Vector3 vector = Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
		return sqrt(((vector.X * vector.X) + (vector.Y * vector.Y)) + (vector.Z * vector.Z));
	}
};

struct Rect {
	float x;
	float y;
	float width;
	float height;

	Rect() {
		this->x = 0;
		this->y = 0;
		this->width = 0;
		this->height = 0;
	}

	Rect(float x, float y, float width, float height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	bool operator==(const Rect& src) const {
		return (src.x == this->x && src.y == this->y && src.height == this->height &&
			src.width == this->width);
	}

	bool operator!=(const Rect& src) const {
		return (src.x != this->x && src.y != this->y && src.height != this->height &&
			src.width != this->width);
	}

	bool isZero() const {
		return (this->x == 0 && this->y == 0 && this->height == 0 && this->width == 0);
	}
};

float getDistance(struct Vector3 mxyz, struct Vector3 exyz) {
	return sqrt ((mxyz.X - exyz.X) * (mxyz.X - exyz.X) + (mxyz.Y - exyz.Y) * (mxyz.Y - exyz.Y) + (mxyz.Z - exyz.Z) * (mxyz.Z - exyz.Z)) / 100;
}

uintptr_t getA(uintptr_t address) {
    uintptr_t buff;
    pvm(address, &buff, 8);
    return buff;
}

struct Matrix4x4 {
    union {
        struct {
            float m00;
            float m10;
            float m20;
            float m30;
            float m01;
            float m11;
            float m21;
            float m31;
            float m02;
            float m12;
            float m22;
            float m32;
            float m03;
            float m13;
            float m23;
            float m33;
        };
        float m[16];
    };

    inline Matrix4x4();

    inline Matrix4x4 operator*(const Matrix4x4 &m2) const;

    inline Matrix4x4(float entry00, float entry01, float entry02, float entry03, float entry10,
                     float entry11, float entry12, float entry13, float entry20, float entry21,
                     float entry22, float entry23, float entry30, float entry31, float entry32,
                     float entry33);

    inline Vector3 MultiplyPoint(Vector3 point);
};

Matrix4x4::Matrix4x4() : m00(1.0f), m10(0.0f), m20(0.0f), m30(0.0f),
                         m01(0.0f), m11(1.0f), m21(0.0f), m31(0.0f),
                         m02(0.0f), m12(0.0f), m22(1.0f), m32(0.0f),
                         m03(0.0f), m13(0.0f), m23(0.0f), m33(1.0f) {}

Matrix4x4::Matrix4x4(float entry00, float entry01, float entry02, float entry03,
                     float entry10, float entry11, float entry12, float entry13,
                     float entry20, float entry21, float entry22, float entry23,
                     float entry30, float entry31, float entry32, float entry33)
        : m{entry00, entry01, entry02, entry03,
            entry10, entry11, entry12, entry13,
            entry20, entry21, entry22, entry23,
            entry30, entry31, entry32, entry33} {}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &rhs) const {
    Matrix4x4 res;
    res.m00 = this->m00 * rhs.m00 + this->m01 * rhs.m10 + this->m02 * rhs.m20 + this->m03 * rhs.m30;
    res.m01 = this->m00 * rhs.m01 + this->m01 * rhs.m11 + this->m02 * rhs.m21 + this->m03 * rhs.m31;
    res.m02 = this->m00 * rhs.m02 + this->m01 * rhs.m12 + this->m02 * rhs.m22 + this->m03 * rhs.m32;
    res.m03 = this->m00 * rhs.m03 + this->m01 * rhs.m13 + this->m02 * rhs.m23 + this->m03 * rhs.m33;

    res.m10 = this->m10 * rhs.m00 + this->m11 * rhs.m10 + this->m12 * rhs.m20 + this->m13 * rhs.m30;
    res.m11 = this->m10 * rhs.m01 + this->m11 * rhs.m11 + this->m12 * rhs.m21 + this->m13 * rhs.m31;
    res.m12 = this->m10 * rhs.m02 + this->m11 * rhs.m12 + this->m12 * rhs.m22 + this->m13 * rhs.m32;
    res.m13 = this->m10 * rhs.m03 + this->m11 * rhs.m13 + this->m12 * rhs.m23 + this->m13 * rhs.m33;

    res.m20 = this->m20 * rhs.m00 + this->m21 * rhs.m10 + this->m22 * rhs.m20 + this->m23 * rhs.m30;
    res.m21 = this->m20 * rhs.m01 + this->m21 * rhs.m11 + this->m22 * rhs.m21 + this->m23 * rhs.m31;
    res.m22 = this->m20 * rhs.m02 + this->m21 * rhs.m12 + this->m22 * rhs.m22 + this->m23 * rhs.m32;
    res.m23 = this->m20 * rhs.m03 + this->m21 * rhs.m13 + this->m22 * rhs.m23 + this->m23 * rhs.m33;

    res.m30 = this->m30 * rhs.m00 + this->m31 * rhs.m10 + this->m32 * rhs.m20 + this->m33 * rhs.m30;
    res.m31 = this->m30 * rhs.m01 + this->m31 * rhs.m11 + this->m32 * rhs.m21 + this->m33 * rhs.m31;
    res.m32 = this->m30 * rhs.m02 + this->m31 * rhs.m12 + this->m32 * rhs.m22 + this->m33 * rhs.m32;
    res.m33 = this->m30 * rhs.m03 + this->m31 * rhs.m13 + this->m32 * rhs.m23 + this->m33 * rhs.m33;

    return res;
}

Vector3 Matrix4x4::MultiplyPoint(Vector3 point) {
    Vector3 res;
    float w;
    res.X = this->m00 * point.X + this->m01 * point.Y + this->m02 * point.Z + this->m03;
    res.Y = this->m10 * point.X + this->m11 * point.Y + this->m12 * point.Z + this->m13;
    res.Z = this->m20 * point.X + this->m21 * point.Y + this->m22 * point.Z + this->m23;
    w = this->m30 * point.X + this->m31 * point.Y + this->m32 * point.Z + this->m33;
    if (w > 0.01f) {
        w = 1.0F * w;
        res.X /= w;
        res.Y /= w;
        res.Z /= w;
    }
    return res;
}

struct Camera {
    Matrix4x4 worldToCameraMatrix;
    Matrix4x4 projectionMatrix;
};

Matrix4x4 _vMatrix;

typedef struct Il2CppString {
    void *klass;
    void *obj;
    int32_t length;
    char chars[0];
} Il2CppString;

typedef unsigned short UTF16;
std::string getCharacterName(uintptr_t address) {
    int textLength = Read<int>(address + offsetof(Il2CppString, length));
    UTF16* buf16 = new UTF16[textLength];

    // Read the UTF-16 encoded characters directly based on the known length
    for (int i = 0; i < textLength; i++) {
        int classname = Read<int>(address + offsetof(Il2CppString, chars) + i * 4);
        buf16[i * 2] = (classname & 0xffff);
        buf16[i * 2 + 1] = (classname & 0xfffff000) >> 16;
    }

    std::string utf8String;
    utf8::unchecked::utf16to8(buf16, buf16 + textLength, std::back_inserter(utf8String));

    delete[] buf16;

    return utf8String;
}

bool WorldToScreen(Vector3 from, Vector2 *to) {
    auto viewMatrix = _vMatrix.MultiplyPoint(from);
    auto screenPos = Vector3(viewMatrix.X + 1.0f, viewMatrix.Y + 1.0f, viewMatrix.Z + 1.0f) / 2.0f;
    *to = Vector2(screenPos.X * Width, Height - (screenPos.Y * Height));
    return viewMatrix != Vector3::Zero();
}

uintptr_t GetBattleManagerFromInstance() {
    auto gameInstance = Read<uintptr_t>(libbase + Offsets::gameInstanceO);
    if (!gameInstance)
        return 0;

    auto gameInstance2 = Read<uintptr_t>(gameInstance + 0xB8);
    if (!gameInstance2)
        return 0;

    auto gameInstance3 = Read<uintptr_t>(gameInstance2);
    if (!gameInstance3)
        return 0;

    return gameInstance3;
}

uintptr_t GetBattleManagerFromBattleData() {
    auto gameInstance = Read<uintptr_t>(libbase + Offsets::gameInstance2O);
    if (!gameInstance)
        return 0;

    auto gameInstance2 = Read<uintptr_t>(gameInstance + 0xB8);
    if (!gameInstance2)
        return 0;

    auto gameInstance3 = Read<uintptr_t>(gameInstance2 + 0x0);
    if (!gameInstance3)
        return 0;

    return gameInstance3;
}

uintptr_t GetLogicBattleManagerInstance() {
    auto logicBattleManager = Read<uintptr_t>(libbase + 0x667e2a0);
    if (!logicBattleManager)
        return 0;

    auto logicBattleManager2 = Read<uintptr_t>(logicBattleManager);
    if (!logicBattleManager2)
        return 0;

    auto staticPtr = Read<uintptr_t>(logicBattleManager + 0xB8);
    if (!staticPtr)
        return 0;

    auto instancePtr = Read<uintptr_t>(staticPtr);
    if (!instancePtr)
        return 0;

    return instancePtr;
}

uintptr_t GetGameInstance() {
    auto ins = GetBattleManagerFromInstance();
    if (!ins) ins = GetBattleManagerFromBattleData();
    if (!ins) return 0;
    return ins;
}

uintptr_t GetMainCamera() {
    auto main_cam = Read<uintptr_t>(libbase + Offsets::main_camO);
    if (!main_cam)
        return 0;

    auto main_cam2 = Read<uintptr_t>(main_cam + 0xB8);
    if (!main_cam2)
        return 0;

    auto main_cam3 = Read<uintptr_t>(main_cam2 + 0x8);
    if (!main_cam3)
        return 0;

    return main_cam3;
}

std::string getSkillLevelText(int skillLevel) {
    switch (skillLevel) {
        case 1:
            return "❶";
            break;
        case 2:
            return "❷";
            break;
        case 3:
            return "❸";
            break;
        case 4:
            return "❹";
            break;
        default:
            return "";
            break;
    }
}
/*
uintptr_t GetSystemData() {
    auto system_data = Read<uintptr_t>(libbase + 0x668fe80);
    if (!system_data)
        return 0;

    auto system_data2 = Read<uintptr_t>(system_data + 0xB8);
    if (!system_data2)
        return 0;

    return system_data2;
}
*/
std::string strRank[] = {
        "Warrior III * 1",
        "Warrior III * 2",
        "Warrior III * 3",
        "Warrior II * 0",
        "Warrior II * 1",
        "Warrior II * 2",
        "Warrior II * 3",
        "Warrior I * 0",
        "Warrior I * 1",
        "Warrior I * 2",
        "Warrior I * 3",
        "Elite III * 0",
        "Elite III * 1",
        "Elite III * 2",
        "Elite III * 3",
        "Elite III * 4",
        "Elite II * 0",
        "Elite II * 1",
        "Elite II * 2",
        "Elite II * 3",
        "Elite II * 4",
        "Elite I * 0",
        "Elite I * 1",
        "Elite I * 2",
        "Elite I * 3",
        "Elite I * 4",
        "Master IV * 0",
        "Master IV * 1",
        "Master IV * 2",
        "Master IV * 3",
        "Master IV * 4",
        "Master III * 0",
        "Master III * 1",
        "Master III * 2",
        "Master III * 3",
        "Master III * 4",
        "Master II * 0",
        "Master II * 1",
        "Master II * 2",
        "Master II * 3",
        "Master II * 4",
        "Master I * 0",
        "Master I * 1",
        "Master I * 2",
        "Master I * 3",
        "Master I * 4",
        "Grandmaster V * 0",
        "Grandmaster V * 1",
        "Grandmaster V * 2",
        "Grandmaster V * 3",
        "Grandmaster V * 4",
        "Grandmaster V * 5",
        "Grandmaster IV * 0",
        "Grandmaster IV * 1",
        "Grandmaster IV * 2",
        "Grandmaster IV * 3",
        "Grandmaster IV * 4",
        "Grandmaster IV * 5",
        "Grandmaster III * 0",
        "Grandmaster III * 1",
        "Grandmaster III * 2",
        "Grandmaster III * 3",
        "Grandmaster III * 4",
        "Grandmaster III * 5",
        "Grandmaster II * 0",
        "Grandmaster II * 1",
        "Grandmaster II * 2",
        "Grandmaster II * 3",
        "Grandmaster II * 4",
        "Grandmaster II * 5",
        "Grandmaster I * 0",
        "Grandmaster I * 1",
        "Grandmaster I * 2",
        "Grandmaster I * 3",
        "Grandmaster I * 4",
        "Grandmaster I * 5",
        "Epic V * 0",
        "Epic V * 1",
        "Epic V * 2",
        "Epic V * 3",
        "Epic V * 4",
        "Epic V * 5",
        "Epic IV * 0",
        "Epic IV * 1",
        "Epic IV * 2",
        "Epic IV * 3",
        "Epic IV * 4",
        "Epic IV * 5",
        "Epic III * 0",
        "Epic III * 1",
        "Epic III * 2",
        "Epic III * 3",
        "Epic III * 4",
        "Epic III * 5",
        "Epic II * 0",
        "Epic II * 1",
        "Epic II * 2",
        "Epic II * 3",
        "Epic II * 4",
        "Epic II * 5",
        "Epic I * 0",
        "Epic I * 1",
        "Epic I * 2",
        "Epic I * 3",
        "Epic I * 4",
        "Epic I * 5",
        "Legend V * 0",
        "Legend V * 1",
        "Legend V * 2",
        "Legend V * 3",
        "Legend V * 4",
        "Legend V * 5",
        "Legend IV * 0",
        "Legend IV * 1",
        "Legend IV * 2",
        "Legend IV * 3",
        "Legend IV * 4",
        "Legend IV * 5",
        "Legend III * 0",
        "Legend III * 1",
        "Legend III * 2",
        "Legend III * 3",
        "Legend III * 4",
        "Legend III * 5",
        "Legend II * 0",
        "Legend II * 1",
        "Legend II * 2",
        "Legend II * 3",
        "Legend II * 4",
        "Legend II * 5",
        "Legend I * 0",
        "Legend I * 1",
        "Legend I * 2",
        "Legend I * 3",
        "Legend I * 4",
        "Legend I * 5"
};

std::string RankToString(int uiRankLevel, int iMythPoint) {
    std::string nameMythic = "Mythic * ";
    if (uiRankLevel > 136) {
        int Star = uiRankLevel - 136;
        if ((int) Star > 24) nameMythic = "Mythical Honor * ";
        if ((int) Star > 49) nameMythic = "Mythical Glory * ";
        if ((int) Star > 99) nameMythic = "Mythical Immortal * ";
        return nameMythic + std::to_string(Star);
    } else {
        return strRank[uiRankLevel];
    }
}

Vector2 WorldToMinimap(uint CampType, Vector3 HeroPosition, int Size, int Position) {
    float MapWidth = Size;
    float MapHeight = Size;
    float StartPos = Position;
    float EndPos = 0;
    float angle;
    angle = (CampType == 2 ? 314.60f : 134.76f) * 0.017453292519943295;
    auto angleCos = (float)std::cos(angle);
    auto angleSin = (float)std::sin(angle);
    Vector2 Res0;
    Res0.X = (angleCos * HeroPosition.X - angleSin * (HeroPosition.Z * -1)) / 74.11f;
    Res0.Y = (angleSin * HeroPosition.X + angleCos * (HeroPosition.Z * -1)) / 74.11f;
    return {(Res0.X * MapWidth) + StartPos + (MapWidth / 2.f),(Res0.Y * MapHeight) + EndPos + (MapHeight / 2.f)};
}

void SetFieldOfView(uintptr_t MainCamera, float size) {
    auto v7 = Read<float>(MainCamera + 0x15C);
    auto v8 = fminf(size, 179.0);
    if ( size < 0.00001 )
        v8 = 0.00001;
    if ( v7 != v8 )
    {
        Write<float>(MainCamera + 0x15C, v8);
    }
    auto v9 = Read<int32_t>(MainCamera + 0x4EC);
    Write<int16_t>(MainCamera + 0x4E8, 257);
    if ( v9 == 2 )
    {
        auto v10 = Read<float>(MainCamera + 0x44) * 0.5;
        auto v11 = tanf((float)(v7 * 0.017453) * 0.5);
        auto v12 = Read<int64_t>(MainCamera + 0x48);
        auto v13 = v10 / v11;
        Write<float>(MainCamera + 0x50, v13);
        Write<int64_t>(MainCamera + 0x8FC, v12);
        auto v14 = Read<float>(MainCamera + 0x4C8);
        auto v16 = Read<float>(MainCamera + 0x40);
        auto v15 = Read<float>(MainCamera + 0x44);
        auto v17 = Read<int32_t>(MainCamera + 0x54);
        auto v18 = (float)(v14 * v15) / v16;
        if ( v17 == 3 && v18 > 1.0 || v17 == 2 || v17 == 4 && v18 < 1.0 )
        {
            auto v19 = atanf((float)((float)(v16 / v14) * 0.5) / v13);
            auto v20 = v18 * Read<float>(MainCamera + 0x900);
            Write<float>(MainCamera + 0x8F8, v19 * 114.59);
            Write<float>(MainCamera + 0x900, v20);
        }
        else
        {
            Write<float>(MainCamera + 0x8FC, (float)(1.0 / v18) * *(float *)&v12);
            Write<float>(MainCamera + 0x8F8, atanf((float)(v15 * 0.5) / v13) * 114.59);
        }
        Write<uint8_t>(MainCamera + 0x4E9, 1);
    }
}

std::string getCoolDown(uintptr_t show_entity) {
    std::vector<std::string> result;
    auto cdComp = Read<uintptr_t>(show_entity + 0xf0);
    if (!cdComp) return "";

    auto dicCoolInfoPtr1 = Read<uintptr_t>(cdComp + 0x18);
    if (!dicCoolInfoPtr1) return "";

    auto dicCoolInfoPtr = Read<monoDictionary>(dicCoolInfoPtr1);
    auto items = get_mono_dictionary_vector<int, uintptr_t>(dicCoolInfoPtr);

    auto now = Read<uint>(GetLogicBattleManagerInstance() + 0x184);

    for (auto dictionaryEntry: items) {
        auto skillID = dictionaryEntry.key;
        auto coolDownData = dictionaryEntry.value;
        if (skillID < 0 || !coolDownData)
            continue;

        auto fSkillID = skillID % 100;

        auto startTime = Read<uint>(coolDownData + 0x1c);
        auto coolDownTime = Read<uint>(coolDownData + 0x14);
        auto coolTime = coolDownTime + startTime;
        if (coolTime < now)
            coolTime = now;

        auto isCoolDown = (coolTime - now) == 0;
        auto timeLeft = (coolTime - now) / 1000;

        if (skillID < 20000) {
            switch (fSkillID) {
                case 10:
                case 20:
                case 30:
                case 40: {
                    std::string coolDownStr = getSkillLevelText(fSkillID / 10) + ": ";

                    if (!isCoolDown) {
                        coolDownStr += std::to_string(timeLeft) + "s";
                    } else {
                        coolDownStr = "●";
                    }
                    result.push_back(coolDownStr);
                }
                    break;
                default:
                    break;
            }
        } else {
            if (fSkillID == 10) {
                std::string coolDownStr = "✚: ";
                if (!isCoolDown) {
                    coolDownStr += std::to_string(timeLeft) + "s";
                } else {
                    coolDownStr = "✚";
                }
                result.push_back(coolDownStr);
            } else if (skillID <= 20500 && (skillID % 10) == 0) {
                std::string coolDownStr = "✦: ";
                if (!isCoolDown) {
                    coolDownStr += std::to_string(timeLeft) + "s";
                } else {
                    coolDownStr = "✦";
                }
                result.push_back(coolDownStr);
            }
        }
    }

    std::sort(result.begin(), result.end(), [](const std::string& a, const std::string& b) {
        return a < b;
    });

    return std::accumulate(result.begin(), result.end(), std::string(""), [](std::string a, std::string b) {
        if (!a.empty() && !b.empty()) {
            return a + " | " + b;
        } else if (a.empty()) {
            return b;
        } else if (b.empty()) {
            return a;
        } else
            return std::string("");
    });
}

bool IsBuff(int Id) {
    switch(Id) {
        case 2002:
        case 2003:
        case 2110://Turtle Event
        case 2004:
        case 2005:
        case 2006:
        case 2008:
        case 2009:
        case 2011:
        case 2012:
        case 2013:
        case 2056:
        case 2059:
            return true;
            break;
        default:
            return false;
    }
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
bool getSpell(int id){
    switch(id){
        case 20150:
        case 20020:
        case 20030:
        case 20040:
        case 20050:
        case 20060:
        case 20070:
        case 20080:
        case 20140:
        case 20100:
        case 20160:
        case 20190:
            return true;
            break;
        default:
            return false;
    }
}

#endif
