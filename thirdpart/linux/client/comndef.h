//
//  Common definitions
//
//

#ifndef _V100_COMMON_DEFINE_H
#define _V100_COMMON_DEFINE_H

#include <sys/types.h>
#include <pthread.h>
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#if defined(USE_NEW_MINIGUI)
typedef unsigned long	ULONG;

#include <minigui/common.h>
#else/*defined(USE_NEW_MINIGUI)*/
typedef unsigned long	ULONG;
typedef long            LONG;

/* upon added for dialogbox by Terry */

//COMMON TYPES.
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned int	DWORD;
typedef DWORD			UINT;
typedef void			*PVOID;
//typedef int				BOOL;

typedef long long				Sint64;
typedef unsigned long long		Uint64;


typedef ULONG	GHANDLE;

typedef ULONG	HWND;

typedef ULONG	HDC;

typedef ULONG	HCURSOR;

typedef ULONG	HICON;

typedef ULONG	HMENU;

typedef ULONG	HACCEL;

typedef ULONG	HDLG;

typedef ULONG	HHOOK;

typedef ULONG	WPARAM;

typedef ULONG	LPARAM;



#define LOBYTE(w)           ((BYTE)(w))
/**
* \def HIBYTE(w)
* \brief Returns the high byte of the word \a w.
*
* \sa MAKEWORD
*/
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

/**
* \def MAKEWORD(low, high)
* \brief Makes a word from \a low byte and \a high byte.
*/
#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))

/**
* \def LOWORD(l)
* \brief Returns the low word of the double word \a l
*
* \sa MAKELONG
*/
#define LOWORD(l)           ((WORD)(DWORD)(l))
/**
* \def HIWORD(l)
* \brief Returns the high word of the double word \a l
*
* \sa MAKELONG
*/
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))

/**
* \def LOshort(l)
* \brief Returns the low signed word of the double word \a l
*
* \sa MAKELONG
*/
#define LOSWORD(l)          ((short)(DWORD)(l))
/**
* \def HIshort(l)
* \brief Returns the high signed word of the double word \a l
*
* \sa MAKELONG
*/
#define HISWORD(l)          ((short)((((DWORD)(l)) >> 16) & 0xFFFF))


/**
* \def MAKELONG(low, high)
* \brief Makes a double word from \a low word and \a high word.
*/
#define MAKELONG(low, high) ((DWORD)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))



//table size.
#define TABLESIZE(table)    (sizeof((table))/sizeof((table)[0]))




typedef union {
    struct {
        /**
        * the red component of a RGBA quarter.
        */
        BYTE r;
        /**
        * the green component of a RGBA quarter.
        */
        BYTE g;
        /**
        * the blue component of a RGBA quarter.
        */
        BYTE b;
        /**
        * the alpha component of a RGBA quarter.
        */
        BYTE a;
    };

    DWORD rgba;

} RGB;


#define GetRValue(rgb)      ((BYTE)(rgb))
/**
* \def GetGValue(rgb)
* \brief Gets the green component from a RGB triple value \a rgb.
*
* You can make a RGB triple by using MakeRGB.
*
* \sa MakeRGB
*/
#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
/**
* \def GetBValue(rgb)
* \brief Gets the blue component from a RGB triple value \a rgb.
*
* You can make a RGB triple by using MakeRGB.
*
* \sa MakeRGB
*/
#define GetBValue(rgb)      ((BYTE)((rgb) >> 16))

/**
* \def MakeRGB(r, g, b)
* \brief Makes a RGB triple value from red \a r, green \a g, and blue \a b components.
*
* \note The red, green, and blue components are all ranged from 0 to 255,
* and the returned value will be a double word.
*
* \sa GetRValue, GetGValue, GetBValue
*/
#define MakeRGB(r, g, b)    (((DWORD)((BYTE)(r))) | ((DWORD)((BYTE)(g)) << 8) \
                             | ((DWORD)((BYTE)(b)) << 16))


typedef UINT gal_pixel;
typedef unsigned char gal_uint8;


typedef struct {
    /**
    * the x coordinate of the point.
    */
    int x;
    /**
    * the y coordinate of the point.
    */
    int y;
} POINT;


typedef struct {
    /**
    * the x coordinate of the upper-left corner of the rectangle.
    */
    int left;
    /**
    * the y coordinate of the upper-left corner of the rectangle.
    */
    int top;
    /**
    * the x coordinate of the lower-right corner of the rectangle.
    */
    int right;
    /**
    * the y coordinate of the lower-right corner of the rectangle.
    */
    int bottom;
} RECT, *PRECT;


#define RECTWP(prc)  (prc->right - prc->left)
/**
* \def RECTHP(prc)
* \brief Gets the height of a RECT object by using the pointer to it.
*/
#define RECTHP(prc)  (prc->bottom - prc->top)
/**
* \def RECTW(rc)
* \brief Gets the width of a RECT object.
*/
#define RECTW(rc)    (rc.right - rc.left)
/**
* \def RECTH(rc)
* \brief Gets the height of a RECT object.
*/
#define RECTH(rc)    (rc.bottom - rc.top)


typedef struct {
    /**
    * the extent in x coordinate of a 2D object.
    */
    int cx;
    /**
    * the extent in y coordinate of a 2D object.
    */
    int cy;
} SIZE, *PSIZE;



/** The stipple bitmap structure. */
typedef struct {
    /** The width of the stipple bitmap. */
    int width;
    /** The height of the stipple bitmap. */
    int height;

    /** The pitch of the stipple bitmap. */
    int pitch;

    /** The size of the stipple bitmap. */
    size_t size;

    /** The pointer to the buffer of glyph bitmap bits. */
    const unsigned char *bits;
} STIPPLE;


//TRUE or FALSE.

#ifndef FALSE
#define FALSE       0
#endif

#ifndef TRUE
#define TRUE        1
#endif

#ifndef NULL
#define NULL			((void*)0)
#endif



#define MAX_PATH	(255)
#define MAX_NAME	(64)


#ifndef MAX
#define MAX(x, y)           ((x > y)?x:y)
#endif

#ifndef MIN
#define MIN(x, y)           ((x < y)?x:y)
#endif

#define GUIAPI

#endif/*defined(USE_NEW_MINIGUI)*/
extern void *veex_memset(void *s, int c, size_t n);
#define WIFI_AUTO_CONNECT 1
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_V100_COMMON_DEFINE_H 

