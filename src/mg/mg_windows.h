#ifndef _MG_WINDOWS_H_
#define _MG_WINDOWS_H_

// TODO: add ASYNC I/O support

//////////////////////////////////////////////////////////////////////////////
// #define

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#ifndef NULL
#define NULL    0
#endif // NULL
#define TRUE    1
#define FALSE   0
#define MAX_PATH    260

#define DECLSPEC_IMPORT     __declspec(dllimport)
#define DECLSPEC_NORETURN   __declspec(noreturn)
#define WINBASEAPI          DECLSPEC_IMPORT
#define WINUSERAPI          DECLSPEC_IMPORT
#define WINABLEAPI          DECLSPEC_IMPORT
#define WINGDIAPI           DECLSPEC_IMPORT
#define CALLBACK            __stdcall
#define WINAPI              __stdcall
#define WINAPIV             __cdecl
#define APIENTRY            WINAPI
#define APIPRIVATE          __stdcall
#define PASCAL              __stdcall
#define CONST               const
#define UNALIGNED           __unaligned
#define UNALIGNED64         __unaligned

#ifndef _VCRTIMP
    #if defined _CRTIMP && !defined _VCRT_DEFINED_CRTIMP
        #define _VCRTIMP _CRTIMP
    #elif defined _VCRT_BUILD && defined CRTDLL
        #define _VCRTIMP __declspec(dllexport)
    #elif defined _CORECRT_BUILD
        #define _VCRTIMP
    #elif defined _DLL
        #define _VCRTIMP __declspec(dllimport)
    #else
        #define _VCRTIMP
    #endif
#endif

#define GENERIC_READ    (0x80000000L)
#define GENERIC_WRITE   (0x40000000L)
#define GENERIC_EXECUTE (0x20000000L)
#define GENERIC_ALL     (0x10000000L)

#define FILE_ATTRIBUTE_NORMAL   0x00000080

#define PAGE_NOACCESS               0x01
#define PAGE_READONLY               0x02
#define PAGE_READWRITE              0x04
#define PAGE_WRITECOPY              0x08
#define PAGE_EXECUTE                0x10
#define PAGE_EXECUTE_READ           0x20
#define PAGE_EXECUTE_READWRITE      0x40
#define PAGE_EXECUTE_WRITECOPY      0x80
#define MEM_COMMIT                  0x1000
#define MEM_RESERVE                 0x2000
#define MEM_DECOMMIT                0x4000
#define MEM_RELEASE                 0x8000
#define MEM_FREE                    0x10000
#define MEM_PRIVATE                 0x20000
#define MEM_MAPPED                  0x40000
#define MEM_RESET                   0x80000

#define SECTION_QUERY                0x0001
#define SECTION_MAP_WRITE            0x0002
#define SECTION_MAP_READ             0x0004
#define SECTION_MAP_EXECUTE          0x0008
#define SECTION_EXTEND_SIZE          0x0010
#define SECTION_MAP_EXECUTE_EXPLICIT 0x0020
#define STANDARD_RIGHTS_REQUIRED         (0x000F0000L)

#define SECTION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SECTION_QUERY|\
                        SECTION_MAP_WRITE |      \
                        SECTION_MAP_READ |       \
                        SECTION_MAP_EXECUTE |    \
                        SECTION_EXTEND_SIZE)

#define FILE_MAP_WRITE      SECTION_MAP_WRITE
#define FILE_MAP_READ       SECTION_MAP_READ
#define FILE_MAP_ALL_ACCESS SECTION_ALL_ACCESS

#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

//////////////////////////////////////////////////////////////////////////////
// Types

#ifndef VOID
#define VOID void
#endif // VOID

typedef void            *HANDLE;
typedef long            HRESULT;
typedef char            CHAR;
typedef short           SHORT, *PSHORT, *LPSHORT;
typedef int             INT, *PINT, *LPINT;
typedef int             BOOL, *PBOOL, *LPBOOL;
typedef unsigned char   BYTE, *PBYTE, *LPBYTE;
typedef unsigned short  WORD, *PWORD, *LPWORD;
typedef unsigned long   DWORD, *PDWORD, *LPDWORD;
typedef float           FLOAT, *PFLOAT, *LPFLOAT;
typedef void            *PVOID, *LPVOID;
typedef long            LONG, *PLONG, *LPLONG;
typedef CONST void      *PCVOID, *LPCVOID;
typedef unsigned int    UINT, *PUINT, *LPUINT;

typedef CHAR                    *PCHAR, *LPCH, *PCH;
typedef CONST CHAR              *LPCCH, *PCCH;
typedef CHAR                    *NPSTR, *LPSTR, *PSTR;
typedef PSTR                    *PZPSTR;
typedef CONST PSTR              *PCZPSTR;
typedef CONST CHAR              *LPCSTR, *PCSTR;
typedef PCSTR                   *PZPCSTR;
typedef CONST PCSTR             *PCZPCSTR;
typedef CHAR                    *PZZSTR;
typedef CONST CHAR              *PCZZSTR;
typedef CHAR                    *PNZCH;
typedef CONST CHAR              *PCNZCH;

typedef unsigned short          WCHAR;
typedef WCHAR                   *PWCHAR, *LPWCH, *PWCH;
typedef CONST WCHAR             *LPCWCH, *PCWCH;
typedef WCHAR                   *NWPSTR, *LPWSTR, *PWSTR;
typedef PWSTR                   *PZPWSTR;
typedef CONST PWSTR             *PCZPWSTR;
typedef WCHAR UNALIGNED         *LPUWSTR, *PUWSTR;
typedef CONST WCHAR             *LPCWSTR, *PCWSTR;
typedef PCWSTR                  *PZPCWSTR;
typedef CONST PCWSTR            *PCZPCWSTR;
typedef CONST WCHAR UNALIGNED   *LPCUWSTR, *PCUWSTR;
typedef WCHAR                   *PZZWSTR;
typedef CONST WCHAR             *PCZZWSTR;
typedef WCHAR UNALIGNED         *PUZZWSTR;
typedef CONST WCHAR UNALIGNED   *PCUZZWSTR;
typedef WCHAR                   *PNZWCH;
typedef CONST WCHAR             *PCNZWCH;
typedef WCHAR UNALIGNED         *PUNZWCH;
typedef CONST WCHAR UNALIGNED   *PCUNZWCH;

typedef __int64                 INT_PTR, *PINT_PTR, LONG_PTR, *PLONG_PTR, LONGLONG;
typedef unsigned __int64        UINT_PTR, *PUINT_PTR, ULONG_PTR, *PULONG_PTR, ULONGLONG;
typedef UINT_PTR                WPARAM;
typedef LONG_PTR                LPARAM, LRESULT, SSIZE_T, *PSSIZE_T;
typedef ULONG_PTR               SIZE_T, *PSIZE_T, DWORD_PTR, *PDWORD_PTR;

typedef WORD                    ATOM;
typedef DWORD                   COLORREF, *LPCOLORREF;

typedef INT_PTR (WINAPI *FARPROC)();
typedef INT_PTR (WINAPI *NEARPROC)();
typedef INT_PTR (WINAPI *PROC)();

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER, *LPLARGE_INTEGER;

DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HHOOK);
DECLARE_HANDLE(HACCEL);
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HBRUSH);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HGLRC);
DECLARE_HANDLE(HDESK);
DECLARE_HANDLE(HENHMETAFILE);
DECLARE_HANDLE(HFONT);
DECLARE_HANDLE(HICON);
DECLARE_HANDLE(HMENU);
DECLARE_HANDLE(HPALETTE);
DECLARE_HANDLE(HPEN);
DECLARE_HANDLE(HKEY);
DECLARE_HANDLE(HMETAFILE);
DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HRSRC);
DECLARE_HANDLE(HSPRITE);
DECLARE_HANDLE(HLSURF);
DECLARE_HANDLE(HSTR);
DECLARE_HANDLE(HTASK);
DECLARE_HANDLE(HWINSTA);
DECLARE_HANDLE(HKL);

typedef HANDLE      GDIOBJ;
typedef HKEY        *PHKEY;
typedef HINSTANCE   HMODULE;      /* HMODULEs can be used in place of HINSTANCEs */
typedef HICON       HCURSOR;      /* HICONs & HCURSORs are polymorphic */
typedef INT         HFILE;
#define HFILE_ERROR ((HFILE)-1)

//////////////////////////////////////////////////////////////////////////////
// GENERAL
//////////////////////////////////////////////////////////////////////////////

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
typedef struct _WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR   cFileName[ MAX_PATH ];
    CHAR   cAlternateFileName[ 14 ];
} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
typedef struct _WIN32_FIND_DATAW {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    WCHAR  cFileName[ MAX_PATH ];
    WCHAR  cAlternateFileName[ 14 ];
} WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;
typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        } DUMMYSTRUCTNAME;
        PVOID Pointer;
    } DUMMYUNIONNAME;

    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;
typedef struct _OVERLAPPED_ENTRY {
    ULONG_PTR lpCompletionKey;
    LPOVERLAPPED lpOverlapped;
    ULONG_PTR Internal;
    DWORD dwNumberOfBytesTransferred;
} OVERLAPPED_ENTRY, *LPOVERLAPPED_ENTRY;

typedef VOID (WINAPI *LPOVERLAPPED_COMPLETION_ROUTINE)(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

//////////////////////////////////////////////////////////////////////////////
// string.h
//////////////////////////////////////////////////////////////////////////////

int __cdecl                 memcmp(void* const _Buf1, void const* _Buf2, size_t _Size);
void* __cdecl               memcpy(void* _Dst, void const* _Src, size_t _Size);
void* __cdecl               memset(void* _Dst, int _Val,size_t _Size);
_VCRTIMP void* __cdecl      memmove(void* _Dst, void const* _Src, size_t _Size);
size_t __cdecl              strlen(char const* _Str);

//////////////////////////////////////////////////////////////////////////////
// winnt.h
//////////////////////////////////////////////////////////////////////////////

#define RtlEqualMemory(Destination,Source,Length) (!memcmp((Destination),(Source),(Length)))
#define RtlMoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define RtlFillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))

#define MoveMemory RtlMoveMemory
#define CopyMemory RtlCopyMemory
#define FillMemory RtlFillMemory
#define ZeroMemory RtlZeroMemory

//////////////////////////////////////////////////////////////////////////////
// handleapi.h
//////////////////////////////////////////////////////////////////////////////

#define INVALID_HANDLE_VALUE    ((HANDLE)(LONG_PTR)-1)

WINBASEAPI BOOL WINAPI  CloseHandle(HANDLE hObject);

//////////////////////////////////////////////////////////////////////////////
// fileapi.h
//////////////////////////////////////////////////////////////////////////////

#define CREATE_NEW                  1
#define CREATE_ALWAYS               2
#define OPEN_EXISTING               3
#define OPEN_ALWAYS                 4
#define TRUNCATE_EXISTING           5
#define INVALID_FILE_SIZE           ((DWORD)0xFFFFFFFF)
#define INVALID_SET_FILE_POINTER    ((DWORD)-1)
#define INVALID_FILE_ATTRIBUTES     ((DWORD)-1)

WINBASEAPI LONG WINAPI      CompareFileTime(CONST FILETIME * lpFileTime1, CONST FILETIME * lpFileTime2);
WINBASEAPI BOOL WINAPI      CreateDirectoryA(LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
WINBASEAPI BOOL WINAPI      CreateDirectoryW(LPCWSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
WINBASEAPI HANDLE WINAPI    CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WINBASEAPI HANDLE WINAPI    CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WINBASEAPI BOOL WINAPI      DeleteFileA(LPCSTR lpFileName);
WINBASEAPI BOOL WINAPI      DeleteFileW(LPCWSTR lpFileName);
WINBASEAPI HANDLE WINAPI    FindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
WINBASEAPI HANDLE WINAPI    FindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
WINBASEAPI DWORD WINAPI     GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);
WINBASEAPI BOOL WINAPI      GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize);
WINBASEAPI BOOL WINAPI      ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
WINBASEAPI BOOL WINAPI      ReadFileEx(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
WINBASEAPI BOOL WINAPI      WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
WINBASEAPI BOOL WINAPI      WriteFileEx(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
WINBASEAPI DWORD WINAPI     SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
WINBASEAPI BOOL WINAPI      SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod);
WINBASEAPI BOOL WINAPI      LockFile(HANDLE hFile, DWORD dwFileOffsetLow, DWORD dwFileOffsetHigh, DWORD nNumberOfBytesToLockLow, DWORD nNumberOfBytesToLockHigh);
WINBASEAPI BOOL WINAPI      LockFileEx(HANDLE hFile, DWORD dwFlags, DWORD dwReserved, DWORD nNumberOfBytesToLockLow, DWORD nNumberOfBytesToLockHigh, LPOVERLAPPED lpOverlapped);
WINBASEAPI BOOL WINAPI      UnlockFile(HANDLE hFile, DWORD dwFileOffsetLow, DWORD dwFileOffsetHigh, DWORD nNumberOfBytesToUnlockLow, DWORD nNumberOfBytesToUnlockHigh);
WINBASEAPI BOOL WINAPI      UnlockFileEx(HANDLE hFile, DWORD dwReserved, DWORD nNumberOfBytesToUnlockLow, DWORD nNumberOfBytesToUnlockHigh, LPOVERLAPPED lpOverlapped);
WINBASEAPI BOOL WINAPI      RemoveDirectoryA(LPCSTR lpPathName);
WINBASEAPI BOOL WINAPI      RemoveDirectoryW(LPCWSTR lpPathName);

#ifdef UNICODE
    #define CreateDirectory     CreateDirectoryW
    #define CreateFile          CreateFileW
    #define DeleteFile          DeleteFileW
    #define FindFirstFile       FindFirstFileW
    #define RemoveDirectory     RemoveDirectoryW
#else
    #define CreateDirectory     CreateDirectoryA
    #define CreateFile          CreateFileA
    #define DeleteFile          DeleteFileA
    #define FindFirstFile       FindFirstFileA
    #define RemoveDirectory     RemoveDirectoryA
#endif // UNICODE

//////////////////////////////////////////////////////////////////////////////
// heapapi.h
//////////////////////////////////////////////////////////////////////////////

WINBASEAPI HANDLE WINAPI    HeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
WINBASEAPI BOOL WINAPI      HeapDestroy(HANDLE hHeap);
WINBASEAPI LPVOID WINAPI    HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
WINBASEAPI LPVOID WINAPI    HeapReAlloc(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes);
WINBASEAPI BOOL WINAPI      HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
WINBASEAPI HANDLE WINAPI    GetProcessHeap(VOID);
WINBASEAPI SIZE_T WINAPI    HeapCompact(HANDLE hHeap, DWORD dwFlags);
WINBASEAPI SIZE_T WINAPI    HeapSize(HANDLE hHeap, DWORD dwFlags, LPCVOID lpMem);

//////////////////////////////////////////////////////////////////////////////
// WinUser.h
//////////////////////////////////////////////////////////////////////////////

#define CS_VREDRAW          0x0001
#define CS_HREDRAW          0x0002
#define CS_OWNDC            0x0020
#define CW_USEDEFAULT       ((int)0x80000000)

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_TILED            WS_OVERLAPPED
#define WS_ICONIC           WS_MINIMIZE
#define WS_SIZEBOX          WS_THICKFRAME
#define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)
#define WS_POPUPWINDOW      (WS_POPUP          | \
                             WS_BORDER         | \
                             WS_SYSMENU)
#define WS_CHILDWINDOW      (WS_CHILD)

#define WM_NULL             0x0000
#define WM_CREATE           0x0001
#define WM_DESTROY          0x0002
#define WM_MOVE             0x0003
#define WM_SIZE             0x0005
#define WM_QUIT             0x0012
#define WM_CLOSE            0x0010
#define WM_PAINT            0x000F
#define WM_KEYDOWN          0x0100
#define WM_KEYUP            0x0101

#define VK_ESCAPE           0x1B

#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L
#define MB_ICONERROR                MB_ICONHAND

#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
#define SW_SHOWDEFAULT      10
#define SW_FORCEMINIMIZE    11
#define SW_MAX              11

#define PM_NOREMOVE         0x0000
#define PM_REMOVE           0x0001
#define PM_NOYIELD          0x0002

typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;
typedef struct tagWNDCLASSA {
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCSTR      lpszMenuName;
    LPCSTR      lpszClassName;
} WNDCLASSA, *PWNDCLASSA, *NPWNDCLASSA, *LPWNDCLASSA;
typedef struct tagWNDCLASSW {
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCWSTR     lpszMenuName;
    LPCWSTR     lpszClassName;
} WNDCLASSW, *PWNDCLASSW, *NPWNDCLASSW, *LPWNDCLASSW;
typedef struct tagWNDCLASSEXA {
    UINT        cbSize;
    /* Win 3.x */
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCSTR      lpszMenuName;
    LPCSTR      lpszClassName;
    /* Win 4.0 */
    HICON       hIconSm;
} WNDCLASSEXA, *PWNDCLASSEXA, *NPWNDCLASSEXA, *LPWNDCLASSEXA;
typedef struct tagWNDCLASSEXW {
    UINT        cbSize;
    /* Win 3.x */
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCWSTR     lpszMenuName;
    LPCWSTR     lpszClassName;
    /* Win 4.0 */
    HICON       hIconSm;
} WNDCLASSEXW, *PWNDCLASSEXW, *NPWNDCLASSEXW, *LPWNDCLASSEXW;
typedef struct tagMSG {
    HWND        hwnd;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
    DWORD       time;
    POINT       pt;
} MSG, *PMSG, *NPMSG, *LPMSG;
typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;
typedef struct tagPAINTSTRUCT {
    HDC         hdc;
    BOOL        fErase;
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;

// Class
WINUSERAPI ATOM WINAPI      RegisterClassA(CONST WNDCLASSA *lpWndClass);
WINUSERAPI ATOM WINAPI      RegisterClassW(CONST WNDCLASSW *lpWndClass);
WINUSERAPI ATOM WINAPI      RegisterClassExA(CONST WNDCLASSEXA *);
WINUSERAPI ATOM WINAPI      RegisterClassExW(CONST WNDCLASSEXW *);
// Window
WINUSERAPI HWND WINAPI      CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
WINUSERAPI HWND WINAPI      CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
WINUSERAPI BOOL WINAPI      DestroyWindow(HWND hWnd);
WINUSERAPI BOOL WINAPI      ShowWindow(HWND hWnd, int nCmdShow);
WINUSERAPI BOOL WINAPI      UpdateWindow(HWND hWnd);
WINUSERAPI BOOL WINAPI      GetClientRect(HWND hWnd, LPRECT lpRect);
WINUSERAPI BOOL WINAPI      GetWindowRect( HWND hWnd, LPRECT lpRect);
WINUSERAPI BOOL WINAPI      AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);
WINUSERAPI BOOL WINAPI      AdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);
WINUSERAPI int WINAPI       MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
WINUSERAPI int WINAPI       MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);
// Message
WINUSERAPI BOOL WINAPI      PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
WINUSERAPI BOOL WINAPI      PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
WINUSERAPI BOOL WINAPI      TranslateMessage(CONST MSG *lpMsg);
WINUSERAPI LRESULT WINAPI   DispatchMessageA(CONST MSG *lpMsg);
WINUSERAPI LRESULT WINAPI   DispatchMessageW(CONST MSG *lpMsg);
WINUSERAPI BOOL WINAPI      GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
WINUSERAPI BOOL WINAPI      GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
WINUSERAPI LRESULT WINAPI   DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WINUSERAPI LRESULT WINAPI   DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WINUSERAPI VOID WINAPI      PostQuitMessage(int nExitCode);
WINUSERAPI BOOL WINAPI      PostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WINUSERAPI BOOL WINAPI      PostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
// DC / Paint

WINUSERAPI HDC WINAPI 		GetDC(HWND hWnd);
WINUSERAPI HDC WINAPI       GetWindowDC(HWND hWnd);
WINUSERAPI int WINAPI       ReleaseDC(HWND hWnd, HDC hDC);
WINUSERAPI HDC WINAPI       BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);
WINUSERAPI BOOL WINAPI      EndPaint(HWND hWnd, CONST PAINTSTRUCT *lpPaint);

#ifdef UNICODE
    typedef WNDCLASSW           WNDCLASS;
    typedef PWNDCLASSW          PWNDCLASS;
    typedef NPWNDCLASSW         NPWNDCLASS;
    typedef LPWNDCLASSW         LPWNDCLASS;
    typedef WNDCLASSEXW         WNDCLASSEX;
    typedef PWNDCLASSEXW        PWNDCLASSEX;
    typedef NPWNDCLASSEXW       NPWNDCLASSEX;
    typedef LPWNDCLASSEXW       LPWNDCLASSEX;
    #define RegisterClass       RegisterClassW
    #define RegisterClassEx     RegisterClassExW
    #define CreateWindowEx      CreateWindowExW
    #define PeekMessage         PeekMessageW
    #define MessageBox          MessageBoxW
    #define DispatchMessage     DispatchMessageW
    #define GetMessage          GetMessageExW
    #define DefWindowProc       DefWindowProcW
    #define PostMessage         PostMessageW
#else
    typedef WNDCLASSA           WNDCLASS;
    typedef PWNDCLASSA          PWNDCLASS;
    typedef NPWNDCLASSA         NPWNDCLASS;
    typedef LPWNDCLASSA         LPWNDCLASS;
    typedef WNDCLASSEXA         WNDCLASSEX;
    typedef PWNDCLASSEXA        PWNDCLASSEX;
    typedef NPWNDCLASSEXA       NPWNDCLASSEX;
    typedef LPWNDCLASSEXA       LPWNDCLASSEX;
    #define RegisterClass       RegisterClassA
    #define RegisterClassEx     RegisterClassExA
    #define CreateWindowEx      CreateWindowExA
    #define PeekMessage         PeekMessageA
    #define MessageBox          MessageBoxA
    #define DispatchMessage     DispatchMessageA
    #define GetMessage          GetMessageExA
    #define DefWindowProc       DefWindowProcA
    #define PostMessage         PostMessageA
#endif // UNICODE

/////////////////////////////////////////////////////////////////////////////
// wingdi.h
//////////////////////////////////////////////////////////////////////////////

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L
#define DIB_RGB_COLORS      0 /* color table in RGBs */
#define SRCCOPY             (DWORD)0x00CC0020 /* dest = source                   */
#define SRCPAINT            (DWORD)0x00EE0086 /* dest = source OR dest           */
#define SRCAND              (DWORD)0x008800C6 /* dest = source AND dest          */
#define SRCINVERT           (DWORD)0x00660046 /* dest = source XOR dest          */
#define SRCERASE            (DWORD)0x00440328 /* dest = source AND (NOT dest )   */
#define NOTSRCCOPY          (DWORD)0x00330008 /* dest = (NOT source)             */
#define NOTSRCERASE         (DWORD)0x001100A6 /* dest = (NOT src) AND (NOT dest) */
#define MERGECOPY           (DWORD)0x00C000CA /* dest = (source AND pattern)     */
#define MERGEPAINT          (DWORD)0x00BB0226 /* dest = (NOT source) OR dest     */
#define PATCOPY             (DWORD)0x00F00021 /* dest = pattern                  */
#define PATPAINT            (DWORD)0x00FB0A09 /* dest = DPSnoo                   */
#define PATINVERT           (DWORD)0x005A0049 /* dest = pattern XOR dest         */
#define DSTINVERT           (DWORD)0x00550009 /* dest = (NOT dest)               */
#define BLACKNESS           (DWORD)0x00000042 /* dest = BLACK                    */
#define WHITENESS           (DWORD)0x00FF0062 /* dest = WHITE                    */

typedef struct tagBITMAPCOREHEADER {
        DWORD   bcSize;                 /* used to get to color table */
        WORD    bcWidth;
        WORD    bcHeight;
        WORD    bcPlanes;
        WORD    bcBitCount;
} BITMAPCOREHEADER, *LPBITMAPCOREHEADER, *PBITMAPCOREHEADER;
typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;
typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;
typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

WINGDIAPI int WINAPI    StretchDIBits(HDC hdc, int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight, CONST VOID *lpBits, CONST BITMAPINFO *lpbmi, UINT iUsage, DWORD rop);

//////////////////////////////////////////////////////////////////////////////
// memoryapi.h
//////////////////////////////////////////////////////////////////////////////

typedef struct _MEMORY_BASIC_INFORMATION {
    PVOID BaseAddress;
    PVOID AllocationBase;
    DWORD AllocationProtect;
    SIZE_T RegionSize;
    DWORD State;
    DWORD Protect;
    DWORD Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

// Virtual
WINBASEAPI LPVOID WINAPI    VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
WINBASEAPI BOOL WINAPI      VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
WINBASEAPI BOOL WINAPI      VirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
WINBASEAPI SIZE_T WINAPI    VirtualQuery(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);
WINBASEAPI LPVOID WINAPI    VirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
WINBASEAPI BOOL WINAPI      VirtualFreeEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
WINBASEAPI BOOL WINAPI      VirtualProtectEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
WINBASEAPI SIZE_T WINAPI    VirtualQueryEx(HANDLE hProcess, LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);
WINBASEAPI BOOL WINAPI      VirtualLock(LPVOID lpAddress, SIZE_T dwSize);
WINBASEAPI BOOL WINAPI      VirtualUnlock(LPVOID lpAddress, SIZE_T dwSize);
// Filemap
WINBASEAPI HANDLE WINAPI    CreateFileMappingW(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWSTR lpName);
WINBASEAPI LPVOID WINAPI    MapViewOfFile(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap);
WINBASEAPI LPVOID WINAPI    MapViewOfFileEx(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap, LPVOID lpBaseAddress);
WINBASEAPI BOOL WINAPI      FlushViewOfFile(LPCVOID lpBaseAddress, SIZE_T dwNumberOfBytesToFlush);
WINBASEAPI BOOL WINAPI      UnmapViewOfFile(LPCVOID lpBaseAddress);
// Memory
WINBASEAPI BOOL WINAPI      ReadProcessMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesRead);
WINBASEAPI BOOL WINAPI      WriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesWritten);

//////////////////////////////////////////////////////////////////////////////
// processthreadsapi.h
//////////////////////////////////////////////////////////////////////////////

typedef DWORD (WINAPI *PTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;
typedef struct _STARTUPINFOA {
    DWORD   cb;
    LPSTR   lpReserved;
    LPSTR   lpDesktop;
    LPSTR   lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;
typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;
typedef struct _PROC_THREAD_ATTRIBUTE_LIST *PPROC_THREAD_ATTRIBUTE_LIST, *LPPROC_THREAD_ATTRIBUTE_LIST;

// Processes
WINBASEAPI HANDLE WINAPI                    GetCurrentProcess(VOID);
WINBASEAPI DWORD WINAPI                     GetCurrentProcessId(VOID);
WINBASEAPI DECLSPEC_NORETURN VOID WINAPI    ExitProcess(UINT uExitCode);
WINBASEAPI BOOL WINAPI                      TerminateProcess(HANDLE hProcess, UINT uExitCode);
WINBASEAPI BOOL WINAPI                      GetExitCodeProcess(HANDLE hProcess, LPDWORD lpExitCode);
WINBASEAPI BOOL WINAPI                      CreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
WINBASEAPI BOOL WINAPI                      CreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
WINBASEAPI DWORD WINAPI                     GetProcessId(HANDLE Process);
// TLS
WINBASEAPI DWORD WINAPI                     TlsAlloc(VOID);
WINBASEAPI LPVOID WINAPI                    TlsGetValue(DWORD dwTlsIndex);
WINBASEAPI BOOL WINAPI                      TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);
WINBASEAPI BOOL WINAPI                      TlsFree(DWORD dwTlsIndex);
// Threads
WINBASEAPI HANDLE WINAPI                    CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
WINBASEAPI HANDLE WINAPI                    CreateRemoteThread(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
WINBASEAPI HANDLE WINAPI                    OpenThread(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);
WINBASEAPI DECLSPEC_NORETURN VOID WINAPI    ExitThread(DWORD dwExitCode);
WINBASEAPI BOOL WINAPI                      TerminateThread(HANDLE hThread, DWORD dwExitCode);
WINBASEAPI BOOL WINAPI                      GetExitCodeThread(HANDLE hThread, LPDWORD lpExitCode);
WINBASEAPI DWORD WINAPI                     SuspendThread(HANDLE hThread);
WINBASEAPI DWORD WINAPI                     ResumeThread(HANDLE hThread);
WINBASEAPI DWORD WINAPI                     GetThreadId(HANDLE Thread);
WINBASEAPI HANDLE WINAPI                    CreateRemoteThreadEx(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, LPDWORD lpThreadId);

//////////////////////////////////////////////////////////////////////////////
// libloaderapi.h
//////////////////////////////////////////////////////////////////////////////

WINBASEAPI FARPROC WINAPI   GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
WINBASEAPI HMODULE WINAPI   LoadLibraryExA(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
WINBASEAPI HMODULE WINAPI   LoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
WINBASEAPI BOOL WINAPI      FreeLibrary(HMODULE hLibModule);
WINBASEAPI HMODULE WINAPI   GetModuleHandleA(LPCSTR lpModuleName);
WINBASEAPI HMODULE WINAPI   GetModuleHandleW(LPCWSTR lpModuleName);
WINBASEAPI BOOL WINAPI      GetModuleHandleExA(DWORD dwFlags, LPCSTR lpModuleName, HMODULE *phModule);
WINBASEAPI BOOL WINAPI      GetModuleHandleExW(DWORD dwFlags, LPCWSTR lpModuleName, HMODULE *phModule);

#ifndef UNICODE
    #define LoadLibraryEx       LoadLibraryExW
    #define GetModuleHandle     GetModuleHandleW
    #define GetModuleHandleEx   GetModuleHandleExW
#else
    #define LoadLibraryEx       LoadLibraryExA
    #define GetModuleHandle     GetModuleHandleA
    #define GetModuleHandleEx   GetModuleHandleExA
#endif // UNICODE

//////////////////////////////////////////////////////////////////////////////
// WinBase.h
//////////////////////////////////////////////////////////////////////////////

#define STD_INPUT_HANDLE    ((DWORD)-10)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)
#define STD_ERROR_HANDLE    ((DWORD)-12)

#define IGNORE              0       // Ignore signal
#define INFINITE            0xFFFFFFFF  // Infinite timeout

#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

WINBASEAPI HANDLE WINAPI    CreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName);
WINBASEAPI HMODULE WINAPI   LoadLibraryA(LPCSTR lpLibFileName);
WINBASEAPI HMODULE WINAPI   LoadLibraryW(LPCWSTR lpLibFileName);
WINBASEAPI DWORD WINAPI     WaitForMultipleObjects(DWORD nCount, CONST HANDLE *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds);
WINBASEAPI HANDLE WINAPI    OpenFileMappingA(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
WINBASEAPI HANDLE WINAPI    OpenFileMappingW(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCWSTR lpName);
WINBASEAPI HANDLE WINAPI    CreateSemaphoreA(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName);
WINBASEAPI HANDLE WINAPI    CreateSemaphoreW(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCWSTR lpName);
WINBASEAPI HANDLE WINAPI    CreateSemaphoreExA(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);

#ifdef UNICODE
    #define CreateFileMapping   CreateFileMappingW
    #define LoadLibrary         LoadLibraryW
    #define OpenFileMapping     OpenFileMappingW
    #define CreateSemaphore     CreateSemaphoreW
#else
    #define CreateFileMapping   CreateFileMappingA
    #define LoadLibrary         LoadLibraryA
    #define OpenFileMapping     OpenFileMappingA
    #define CreateSemaphore     CreateSemaphoreA
    #define CreateSemaphoreEx   CreateSemaphoreExA
#endif // UNICODE

//////////////////////////////////////////////////////////////////////////////
// syncapi.h
//////////////////////////////////////////////////////////////////////////////

typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, *PRLIST_ENTRY;
typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD   Type;
    WORD   CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Flags;
    WORD   CreatorBackTraceIndexHigh;
    WORD   SpareWORD  ;
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG, RTL_RESOURCE_DEBUG, *PRTL_RESOURCE_DEBUG;
typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;

    //
    //  The following three fields control entering and exiting the critical
    //  section for the resource
    //

    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;        // from the thread's ClientId->UniqueThread
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;        // force size on 64-bit systems when packed
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION, *LPCRITICAL_SECTION;

// Mutexes
WINBASEAPI HANDLE WINAPI    CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName);
WINBASEAPI HANDLE WINAPI    CreateMutexW(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCWSTR lpName);
WINBASEAPI HANDLE WINAPI    CreateMutexExA(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
WINBASEAPI HANDLE WINAPI    CreateMutexExW(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCWSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
WINBASEAPI BOOL WINAPI      ReleaseMutex(HANDLE hMutex);
// Semaphores
WINBASEAPI HANDLE WINAPI    CreateSemaphoreExW(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCWSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
WINBASEAPI BOOL WINAPI      ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount);
// Events
WINBASEAPI HANDLE WINAPI    CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);
WINBASEAPI HANDLE WINAPI    CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCWSTR lpName);
WINBASEAPI HANDLE WINAPI    CreateEventExA(LPSECURITY_ATTRIBUTES lpEventAttributes, LPCSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
WINBASEAPI HANDLE WINAPI    CreateEventExW(LPSECURITY_ATTRIBUTES lpEventAttributes, LPCWSTR lpName, DWORD dwFlags, DWORD dwDesiredAccess);
WINBASEAPI BOOL WINAPI      SetEvent(HANDLE hEvent);
WINBASEAPI BOOL WINAPI      ResetEvent(HANDLE hEvent);
// Critical sections
WINBASEAPI VOID WINAPI      InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
WINBASEAPI VOID WINAPI      EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
WINBASEAPI VOID WINAPI      LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
WINBASEAPI BOOL WINAPI      InitializeCriticalSectionAndSpinCount(LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount);
WINBASEAPI BOOL WINAPI      InitializeCriticalSectionEx(LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount, DWORD Flags);
WINBASEAPI DWORD WINAPI     SetCriticalSectionSpinCount(LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount);
WINBASEAPI BOOL WINAPI      TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
WINBASEAPI VOID WINAPI      DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
// Wait / sleep
WINBASEAPI DWORD WINAPI     WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
WINBASEAPI DWORD WINAPI     WaitForSingleObjectEx(HANDLE hHandle, DWORD dwMilliseconds, BOOL bAlertable);
WINBASEAPI DWORD WINAPI     WaitForMultipleObjectsEx(DWORD nCount, CONST HANDLE *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds, BOOL bAlertable);
WINBASEAPI DWORD WINAPI     SleepEx(DWORD dwMilliseconds, BOOL bAlertable);

#ifdef UNICODE
    #define CreateMutex     CreateMutexW
    #define CreateMutexEx   CreateMutexExW
    #define CreateEvent     CreateEventW
    #define CreateEventEx   CreateEventExW
#else
    #define CreateMutex     CreateMutexA
    #define CreateMutexEx   CreateMutexExA
    #define CreateEvent     CreateEventA
    #define CreateEventEx   CreateEventExA
#endif // UNICODE

//////////////////////////////////////////////////////////////////////////////
// syncapi.h
//////////////////////////////////////////////////////////////////////////////

WINBASEAPI DWORD WINAPI     GetLastError(VOID);
WINBASEAPI VOID WINAPI      SetLastError(DWORD dwErrCode);

//////////////////////////////////////////////////////////////////////////////
// processenv.h
//////////////////////////////////////////////////////////////////////////////

WINBASEAPI LPSTR WINAPI			GetCommandLineA(VOID);
WINBASEAPI LPWSTR WINAPI		GetCommandLineW(VOID);
WINBASEAPI DWORD WINAPI			GetCurrentDirectoryA(DWORD nBufferLength, LPSTR lpBuffer);
WINBASEAPI DWORD WINAPI			GetCurrentDirectoryW(DWORD nBufferLength, LPWSTR lpBuffer);
WINBASEAPI HANDLE WINAPI 		GetStdHandle(DWORD nStdHandle);
WINBASEAPI BOOL WINAPI 			SetStdHandle(DWORD nStdHandle, HANDLE hHandle);

#ifdef UNICODE
#define GetCommandLine  	GetCommandLineW
#define GetCurrentDirectory	GetCurrentDirectoryW
#else
#define GetCommandLine  	GetCommandLineA
#define GetCurrentDirectory	GetCurrentDirectoryA
#endif // UNICODE

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _MG_WINDOWS_H_

