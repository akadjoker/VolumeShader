
#include "pch.h"



#if defined(PLATFORM_DESKTOP) && defined(_WIN32) && (defined(_MSC_VER) || defined(__TINYC__))

    #include "wdirent.h"    // Required for: DIR, opendir(), closedir() [Used in LoadDirectoryFiles()]
#else
    #include <dirent.h>             // Required for: DIR, opendir(), closedir() [Used in LoadDirectoryFiles()]
#endif

#if defined(_WIN32)
    #include <direct.h>             // Required for: _getch(), _chdir()
    #define GETCWD _getcwd          // NOTE: MSDN recommends not to use getcwd(), chdir()
    #define CHDIR _chdir
    #include <io.h>                 // Required for: _access() [Used in FileExists()]
#else
    #include <unistd.h>             // Required for: getch(), chdir() (POSIX), access()
    #define GETCWD getcwd
    #define CHDIR chdir
#endif

#ifdef PLATFORM_WIN

#define CONSOLE_COLOR_RESET ""
#define CONSOLE_COLOR_GREEN ""
#define CONSOLE_COLOR_RED ""
#define CONSOLE_COLOR_PURPLE ""



#else

#define CONSOLE_COLOR_RESET "\033[0m"
#define CONSOLE_COLOR_GREEN "\033[1;32m"
#define CONSOLE_COLOR_RED "\033[1;31m"
#define CONSOLE_COLOR_PURPLE "\033[1;35m"




#endif 



static void LogMessage( int level, const char *msg, va_list args )
{
	  


        char buffer[1024];
        vsnprintf( buffer, sizeof(buffer), msg, args );

    switch (level)
    {
        case 0:
            SDL_LogInfo(0, "%s %s%s", CONSOLE_COLOR_GREEN, buffer,CONSOLE_COLOR_RESET);
            break;
        case 1:
            SDL_LogError(0,  "%s %s%s", CONSOLE_COLOR_RED, buffer,CONSOLE_COLOR_RESET);
            break;
        case 2:
            SDL_LogWarn(0,  "%s %s%s", CONSOLE_COLOR_PURPLE, buffer,CONSOLE_COLOR_RESET);
            break;
 
    }

}


void LogError( const char *msg, ... )
{
	

	va_list args;
	va_start( args, msg );
	LogMessage( 1, msg, args );
	va_end( args );
}


void LogWarning( const char *msg, ... )
{
	
	va_list args;
	va_start( args, msg );
	LogMessage( 2, msg, args );
	va_end( args );
}


void LogInfo( const char *msg, ... )
{
	va_list args;
	va_start( args, msg );
	LogMessage( 0, msg, args );
	va_end( args );
}


 const char *strprbrk(const char *s, const char *charset)
{
    const char *latestMatch = NULL;
    for (; s = strpbrk(s, charset), s != NULL; latestMatch = s++) { }
    return latestMatch;
}

bool FileExists(const char *fileName)
{
   bool result = false;

#if defined(_WIN32)
    if (_access(fileName, 0) != -1) result = true;
#else
    if (access(fileName, F_OK) != -1) result = true;
#endif
    return result;
}

bool DirectoryExists(const char *dirPath)
{
    bool result = false;

    DIR *dir = opendir(dirPath);
    if (dir != NULL)
    {
        result = true;
        closedir(dir);
    }

    return result;
}

bool IsFileExtension(const char *fileName, const char *ext)
{
    bool result = false;

    const char *fileExt = std::strrchr(fileName, '.');

    if (fileExt != NULL)
    {
        if (strcmp(fileExt, ext) == 0) result = true;
    }

    return result;
}

int GetFileLength(const char *fileName)
{
    int length = 0;

    SDL_RWops *rw = SDL_RWFromFile(fileName, "rb");
    if (rw != NULL)
    {
        length = SDL_RWsize(rw);
        SDL_RWclose(rw);
    }


    return length;
}

const char *GetFileExtension(const char *fileName)
{
    const char *fileExt = std::strrchr(fileName, '.');

    if (fileExt != NULL) return fileExt;

    return NULL;
}

const char *GetFileName(const char *filePath)
{
    const char *fileName = std::strrchr(filePath, '/');

    if (fileName != NULL) return fileName + 1;

    return filePath;
}

const char *GetFileNameWithoutExt(const char *filePath)
{
    static char baseName[256];
    std::strcpy(baseName, GetFileName(filePath));

    char *dot = std::strrchr(baseName, '.');
    if (dot) *dot = '\0';

    return baseName;
}

const char *GetDirectoryPath(const char *filePath)
{
    static char dirPath[256];
    std::strcpy(dirPath, filePath);

    char *lastSlash = std::strrchr(dirPath, '/');

    if (lastSlash != NULL) lastSlash[1] = '\0';
    else dirPath[0] = '\0';

    return dirPath;
}

const char *GetPrevDirectoryPath(const char *dirPath)
{
    static char prevDirPath[256];
    std::strcpy(prevDirPath, dirPath);

    char *lastSlash = std::strrchr(prevDirPath, '/');

    if (lastSlash != NULL)
    {
        lastSlash[0] = '\0';
        lastSlash = std::strrchr(prevDirPath, '/');
        if (lastSlash != NULL) lastSlash[1] = '\0';
        else prevDirPath[0] = '\0';
    }
    else prevDirPath[0] = '\0';

    return prevDirPath;
}

const char *GetWorkingDirectory()
{
    static char currentDir[256];
    GETCWD(currentDir, 256);
    return currentDir;
}

const char *GetApplicationDirectory()
{
    static char appDir[256];
    GETCWD(appDir, 256);
    return appDir;
}

bool ChangeDirectory(const char *dir)
{
    return CHDIR(dir) == 0;
}

bool IsPathFile(const char *path)
{
    bool result = false;

    DIR *dir = opendir(path);
    if (dir == NULL) result = true;

    return result;
}


unsigned char* LoadDataFile(const char* fileName, unsigned int* bytesRead)
{
    unsigned char* data = NULL;
    *bytesRead = 0;

 
        SDL_RWops* file = SDL_RWFromFile(fileName, "rb");


        if (file != NULL)
        {
            unsigned int size = (int)SDL_RWsize(file);

            if (size > 0)
            {
                data = (unsigned char*)malloc(size * sizeof(unsigned char));

                unsigned int count = (unsigned int)SDL_RWread(file, data, sizeof(unsigned char), size);
                *bytesRead = count;

                 SDL_LogInfo(0, "FILEIO: [%s] File loaded successfully", fileName);

            }
            else SDL_LogError(0, "FILEIO: [%s] Failed to read file", fileName);
            SDL_RWclose(file);
        }
        else SDL_LogError(0, "FILEIO: [%s] Failed to open file", fileName);

    return data;
}

bool SaveDataFile(const char* fileName, void* data, unsigned int bytesToWrite)
{
    bool success = false;

   
        SDL_RWops* file = SDL_RWFromFile(fileName, "wb");
        if (file != NULL)
        {
            unsigned int count = (unsigned int)SDL_RWwrite(file, data, sizeof(unsigned char), bytesToWrite);
            if (count == 0)SDL_LogError(0, "FILEIO: [%s] Failed to write file", fileName);

            else  SDL_LogInfo(0, "FILEIO: [%s] File saved successfully", fileName);

            int result = SDL_RWclose(file);
            if (result == 0) success = true;
        }
        else SDL_LogError(0, "FILEIO: [%s] Failed to open file", fileName);
 
    return success;
}

char* LoadTextFile(const char* fileName)
{
    char* text = NULL;

  
        SDL_RWops* textFile = SDL_RWFromFile(fileName, "rt");
        if (textFile != NULL)
        {
            unsigned  int size = (int)SDL_RWsize(textFile);
            if (size > 0)
            {
                text = (char*)malloc((size + 1) * sizeof(char));
                unsigned int count = (unsigned int)SDL_RWread(textFile, text, sizeof(char), size);
                if (count < size) text = (char*)realloc(text, count + 1);
                text[count] = '\0';

                SDL_LogInfo(0, "FILEIO: [%s] Text file loaded successfully", fileName);
            }
            else SDL_LogError(0, "FILEIO: [%s] Failed to read text file", fileName);

            SDL_RWclose(textFile);
        }
        else SDL_LogError(0, "FILEIO: [%s] Failed to open text file", fileName);
    

    return text;
}

bool SaveTextFile(const char* fileName, char* text)
{
    bool success = false;

        SDL_RWops* file = SDL_RWFromFile(fileName, "wt");
        if (file != NULL)
        {
            size_t strLen = SDL_strlen(text);
            int count = SDL_RWwrite(file, text, 1, strLen);
            if (count < 0) SDL_LogError(0, "FILEIO: [%s] Failed to write text file", fileName);
            else SDL_LogInfo(0,"FILEIO: [%s] Text file saved successfully", fileName);

            int result = SDL_RWclose(file);
            if (result == 0) success = true;
        }
        else SDL_LogError(0, "FILEIO: [%s] Failed to open text file", fileName);

    return success;
}
