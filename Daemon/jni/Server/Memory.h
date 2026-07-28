#ifndef MEMORY_H
#define MEMORY_H
#include <dirent.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <fcntl.h>
using namespace std;
#define halfShift	10
#define UNI_SUR_HIGH_START  (UTF32)0xD800
#define UNI_SUR_HIGH_END    (UTF32)0xDBFF
#define UNI_SUR_LOW_START   (UTF32)0xDC00
#define UNI_SUR_LOW_END     (UTF32)0xDFFF
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
// Unsigned base types.
typedef unsigned char uint8;
typedef unsigned short int	uint16;
typedef unsigned int uint32;
typedef unsigned long long	uint64;

// Signed base types.
typedef	signed char	int8;
typedef signed short int int16;
typedef signed int	int32;
typedef signed long long int64;

typedef unsigned char BYTE;
//typedef unsigned int uintptr_t;
typedef char UTF8;
typedef unsigned short UTF16;
typedef unsigned int UTF32;
static const UTF32 halfBase = 0x0010000UL;
static const UTF8 firstByteMark[7] = { 0x00, 0x00, static_cast<UTF8>(0xC0), static_cast<UTF8>(0xE0), static_cast<UTF8>(0xF0), static_cast<UTF8>(0xF8), static_cast<UTF8>(0xFC) };

typedef enum
{
    strictConversion = 0,
    lenientConversion
} ConversionFlags;

typedef enum
{
    conversionOK,				/* conversion successful */
    sourceExhausted,			/* partial character in source, but hit end */
    targetExhausted,			/* insuff. room in target for conversion */
    sourceIllegal,				/* source sequence is illegal/malformed */
    conversionFailed
} ConversionResult;

int Utf16_To_Utf8(const UTF16 * sourceStart, UTF8 * targetStart, size_t outLen,
                  ConversionFlags flags)
{
    int result = 0;
    const UTF16 *source = sourceStart;
    UTF8 *target = targetStart;
    UTF8 *targetEnd = targetStart + outLen;

    if ((NULL == source) || (NULL == targetStart))
    {
        printf("ERR, Utf16_To_Utf8: source=%p, targetStart=%p\n", source, targetStart);
        return conversionFailed;
    }

    while (*source)
    {
        UTF32 ch;
        unsigned short bytesToWrite = 0;
        const UTF32 byteMask = 0xBF;
        const UTF32 byteMark = 0x80;
        const UTF16 *oldSource = source;	/* In case we have to back up
											   because of target overflow. */
        ch = *source++;
        /* If we have a surrogate pair, convert to UTF32 first. */
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
        {
            /* If the 16 bits following the high surrogate are in the source
               buffer... */
            if (*source)
            {
                UTF32 ch2 = *source;
                /* If it's a low surrogate, convert to UTF32. */
                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
                {
                    ch = ((ch - UNI_SUR_HIGH_START) << halfShift) + (ch2 - UNI_SUR_LOW_START) +
                         halfBase;
                    ++source;
                }
                else if (flags == strictConversion)
                {				/* it's an unpaired high surrogate */
                    --source;	/* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                }
            }
            else
            {					/* We don't have the 16 bits following the
								   high surrogate. */
                --source;		/* return to the high surrogate */
                result = sourceExhausted;
                break;
            }
        }
        else if (flags == strictConversion)
        {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END)
            {
                --source;		/* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        /* Figure out how many bytes the result will require */
        if (ch < (UTF32) 0x80)
        {
            bytesToWrite = 1;
        }
        else if (ch < (UTF32) 0x800)
        {
            bytesToWrite = 2;
        }
        else if (ch < (UTF32) 0x10000)
        {
            bytesToWrite = 3;
        }
        else if (ch < (UTF32) 0x110000)
        {
            bytesToWrite = 4;
        }
        else
        {
            bytesToWrite = 3;
            ch = UNI_REPLACEMENT_CHAR;
        }

        target += bytesToWrite;
        if (target > targetEnd)
        {
            source = oldSource;	/* Back up source pointer! */
            target -= bytesToWrite;
            result = targetExhausted;
            break;
        }
        switch (bytesToWrite)
        {						/* note: everything falls through. */
            case 4:
                *--target = (UTF8) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 3:
                *--target = (UTF8) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 2:
                *--target = (UTF8) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 1:
                *--target = (UTF8) (ch | firstByteMark[bytesToWrite]);
        }
        target += bytesToWrite;
    }
    return result;
}
uint32 MAX_SIZE = 100;
static uintptr_t libbase = 0;

pid_t find_pid(char * name)
{
    char text[69];
    pid_t pid = 0;
    sprintf(text,"pidof %s",name);
    FILE *chkRun = popen(text, "r");
    if (chkRun)
    {
        char output[10];
        fgets(output ,10,chkRun);
        pclose(chkRun);
        pid= atoi(output);
    }
    if (pid < 10)
    {
        DIR* dir = NULL;
        struct dirent* ptr = NULL;
        FILE* fp = NULL;
        char filepath[256];
        char filetext[128];
        dir = opendir("/proc");
        if (NULL != dir)
        {
            while ((ptr = readdir(dir)) != NULL)
            {
                if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                    continue;
                if (ptr->d_type != DT_DIR)
                    continue;
                sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
                fp = fopen(filepath, "r");
                if (NULL != fp)
                {
                    fgets(filetext, sizeof(filetext), fp);
                    if (strcmp(filetext, name) == 0)
                    {
                        fclose(fp);
                        break;
                    }
                    fclose(fp);
                }
            }
        }
        if (readdir(dir) == NULL)
        {
            closedir(dir);
            return 0;
        }
        closedir(dir);
        pid= atoi(ptr->d_name);
    }
    return pid;
}

uintptr_t getBase(const char *module_name) {
    FILE *fp;
    uintptr_t addr = 0;
    char filename[40], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", target_pid);
    fp = fopen(filename, "rt");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, module_name)) {
                addr = (uintptr_t)strtoull(buffer, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

uintptr_t get_module_base(const char *module_name) {
    FILE *fp;
    uintptr_t addr = 0;
    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", target_pid);
    fp = fopen(filename, "rt");
    if (fp != nullptr) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, module_name)) {
#if defined(__LP64__)
                sscanf(buffer, "%lx-%*s", &addr);
#else
                sscanf(buffer, "%x-%*s", &addr);
#endif
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

uintptr_t getRealOffset(uintptr_t offset) {
	if (libbase == 0) {
		LOGW("Error: Can't Find Base Addr for Real Offset");
		return 0;
	}
	return (libbase + offset);
}

template <typename T>
T Read(uintptr_t address) {
	T data;
	vm_readv(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&data), sizeof(T));
	return data;
}

template <typename T>
void Write(uintptr_t address, T data) {
	vm_writev(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&data), sizeof(T));
}

template <typename T>
T* ReadArr(uintptr_t address, unsigned int size) {
	T data[size];
	T* ptr = data;
	vm_readv(reinterpret_cast<void*>(address), reinterpret_cast<void*>(ptr), (sizeof(T) * size));
	return ptr;
}

std::string ReadStr2(uintptr_t address, unsigned int size) {
    std::string name(size, '\0');
    vm_readv((void *) address, (void *) name.data(), size * sizeof(char));
    name.shrink_to_fit();
    return name;
}

struct mfloat {
    float value;
};

mfloat getF(uintptr_t address) {
    mfloat result;
    pvm(address, &result.value, 8);
    return result;
}

int getI(uintptr_t address) {
    int buff;
    pvm(address, &buff, 8);
    return buff;
}

char* ReadStr(uintptr_t address, unsigned int size) {
	char* data = new char[size];
	for(int i=0; i < size; i++){
		vm_readv(reinterpret_cast<void*>(address + (sizeof(char)*i)), reinterpret_cast<void*>(data + i), sizeof(char));
		if(data[i] == 0x0){
			break;
		}
	}
	return data;
}

uintptr_t getPtr(uintptr_t address) {
	return Read<uintptr_t>(address);
}

#endif //MEMORY_H
