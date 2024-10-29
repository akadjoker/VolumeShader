#pragma once
#include "glm/glm.hpp"
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define MAX_TEXT_BUFFER_LENGTH              1024      




const char *strprbrk(const char *s, const char *charset);

    bool FileExists(const char *fileName);
    bool DirectoryExists(const char *dirPath);              
    bool IsFileExtension(const char *fileName, const char *ext); // Check file extension (including point: .png, .wav)
    int GetFileLength(const char *fileName);                    
    
    const char *GetFileExtension(const char *fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
    const char *GetFileName(const char *filePath);              // Get pointer to filename for a path string
    const char *GetFileNameWithoutExt(const char *filePath);    // Get filename string without extension (uses static string)
    const char *GetDirectoryPath(const char *filePath);         // Get full path for a given fileName with path (uses static string)
    const char *GetPrevDirectoryPath(const char *dirPath);      // Get previous directory path for a given path (uses static string)
    const char *GetWorkingDirectory(void);                      // Get current working directory (uses static string)
    const char *GetApplicationDirectory(void);                  // Get the directory if the running application (uses static string)


bool SaveDataFile(const char *fileName, void *data, unsigned int bytesToWrite);
char* LoadTextFile(const char *fileName);

unsigned char *LoadDataFile(const char *fileName, unsigned int *bytesRead);
 bool SaveTextFile(const char *fileName, char *text);

void LogError( const char *msg, ... );
void LogInfo( const char *msg, ... );
void LogWarning( const char *msg, ... );



template<typename T>
struct  Rectangle
{

T x;
T y;
T width;
T height;

Rectangle() : x(0), y(0), width(0), height(0) {}
Rectangle(T x, T y, T width, T height) : x(x), y(y), width(width), height(height) {}
Rectangle(const Rectangle &rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height) {}

void Set(T x, T y, T width, T height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void Merge(const Rectangle &rect)
{
	T right = x + width;
	T bottom = y + height;
	T rectRight = rect.x + rect.width;
	T rectBottom = rect.y + rect.height;
	x = Min(x, rect.x);
	y = Min(y, rect.y);
	right = Max(right, rectRight);
	bottom = Max(bottom, rectBottom);
	width = right - x;
	height = bottom - y;
}

void Merge(const glm::vec2 &point)
{
	T right = x + width;
	T bottom = y + height;
	x =Min(x, point.x);
	y = Min(y, point.y);
	right = Max(right, point.x);
	bottom = Max(bottom, point.y);
	width = right - x;
	height = bottom - y;
}

void Clear()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

Rectangle& operator=(const Rectangle &rect)
{
	if (this == &rect)
		return *this;
	x = rect.x;
	y = rect.y;
	width = rect.width;
	height = rect.height;
	return *this;
}

};

template<typename T>
struct  Size
{
T width;
T height;

Size() : width(0), height(0) {}
Size(T w, T h) : width(w), height(h) {}
Size(const Size &size) : width(size.width), height(size.height) {}

Size& operator=(const Size &size)
{
	if (this == &size)
		return *this;
	width = size.width;
	height = size.height;
	return *this;
}


};

typedef Rectangle<int> IntRect;
typedef Rectangle<float> FloatRect;
typedef Size<int> IntSize;
typedef Size<float> FloatSize;

inline float Clamp( float a, float min, float max ){	if ( a < min ) a = min;	else if ( a > max ) a = max;	return a;}
inline int   Clamp( int a, int min, int max ){	if ( a < min ) a = min;	else if ( a > max ) a = max;	return a;}