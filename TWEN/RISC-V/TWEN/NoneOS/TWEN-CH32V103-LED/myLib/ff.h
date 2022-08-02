/*---------------------------------------------------------------------------/
/  FatFs - FAT file system module include R0.11a    (C)ChaN, 2015
/----------------------------------------------------------------------------/
/ FatFs module is a free software that opened under license policy of
/ following conditions.
/
/ Copyright (C) 2015, ChaN, all right reserved.
/
/ 1. Redistributions of source code must retain the above copyright notice,
/    this condition and the following disclaimer.
/
/ This software is provided by the copyright holder and contributors "AS IS"
/ and any warranties related to this software are DISCLAIMED.
/ The copyright owner or contributors be NOT LIABLE for any damages caused
/ by use of this software.
/---------------------------------------------------------------------------*/


#ifndef _FATFS
#define _FATFS  64180   /* Revision ID */

#ifdef __cplusplus
extern "C" {
#endif

#include "diskio.h"     /* Declarations of disk I/O functions */
#include "integer.h"    /* Basic integer types */
#include "ffconf.h"     /* FatFs configuration options */
#if _FATFS != _FFCONF
#error Wrong configuration file (ffconf.h).
#endif

/* Definitions of volume management */
#if _MULTI_PARTITION        /* Multiple partition configuration */
typedef struct {
    UCHAR pd;    /* Physical drive number */
    UCHAR pt;    /* Partition: 0:Auto detect, 1-4:Forced partition) */
} PARTITION;
extern PARTITION VolToPart[];   /* Volume - Partition resolution table */
#define LD2PD(vol) (VolToPart[vol].pd)  /* Get physical drive number */
#define LD2PT(vol) (VolToPart[vol].pt)  /* Get partition index */
#else                           /* Single partition configuration */
#define LD2PD(vol) (UCHAR)(vol)  /* Each logical drive is bound to the same physical drive number */
#define LD2PT(vol) 0            /* Find first valid partition or in SFD */
#endif

/* Type of path name strings on FatFs API */
#if _LFN_UNICODE            /* Unicode string */
#if !_USE_LFN
#error _LFN_UNICODE must be 0 at non-LFN cfg.
#endif
#ifndef _INC_TCHAR
typedef WCHAR TCHAR;
#define _T(x) L ## x
#define _TEXT(x) L ## x
#endif
#else                       /* ANSI/OEM string */
#ifndef _INC_TCHAR
typedef char TCHAR;
#define _T(x) x
#define _TEXT(x) x
#endif

#endif

/* File system object structure (FATFS) */
typedef struct {
    UCHAR    fs_type;        /* FAT sub-type (0:Not mounted) */
    UCHAR    drv;            /* Physical drive number */
    UCHAR    csize;          /* Sectors per cluster (1,2,4...128) */
    UCHAR    n_fats;         /* Number of FAT copies (1 or 2) */
    UCHAR    wflag;          /* win[] flag (b0:dirty) */
    UCHAR    fsi_flag;       /* FSINFO flags (b7:disabled, b0:dirty) */
    WORD    id;             /* File system mount ID */
    WORD    n_rootdir;      /* Number of root directory entries (FAT12/16) */
#if _MAX_SS != _MIN_SS
    WORD    ssize;          /* Bytes per sector (512, 1024, 2048 or 4096) */
#endif
#if _FS_REENTRANT
    _SYNC_t sobj;           /* Identifier of sync object */
#endif
#if !_FS_READONLY
    DWORD   last_clust;     /* Last allocated cluster */
    DWORD   free_clust;     /* Number of free clusters */
#endif
#if _FS_RPATH
    DWORD   cdir;           /* Current directory start cluster (0:root) */
#endif
    DWORD   n_fatent;       /* Number of FAT entries, = number of clusters + 2 */
    DWORD   fsize;          /* Sectors per FAT */
    DWORD   volbase;        /* Volume start sector */
    DWORD   fatbase;        /* FAT start sector */
    DWORD   dirbase;        /* Root directory start sector (FAT32:Cluster#) */
    DWORD   database;       /* Data start sector */
    DWORD   winsect;        /* Current sector appearing in the win[] */
    UCHAR    win[_MAX_SS];   /* Disk access window for Directory, FAT (and file data at tiny cfg) */
} FATFS;

/* File object structure (FIL) */
typedef struct {
    FATFS*  fs;             /* Pointer to the related file system object (**do not change order**) */
    WORD    id;             /* Owner file system mount ID (**do not change order**) */
    UCHAR    flag;           /* Status flags */
    UCHAR    err;            /* Abort flag (error code) */
    DWORD   fptr;           /* File read/write pointer (Zeroed on file open) */
    DWORD   fsize;          /* File size */
    DWORD   sclust;         /* File start cluster (0:no cluster chain, always 0 when fsize is 0) */
    DWORD   clust;          /* Current cluster of fpter (not valid when fprt is 0) */
    DWORD   dsect;          /* Sector number appearing in buf[] (0:invalid) */
#if !_FS_READONLY
    DWORD   dir_sect;       /* Sector number containing the directory entry */
    UCHAR*   dir_ptr;        /* Pointer to the directory entry in the win[] */
#endif
#if _USE_FASTSEEK
    DWORD*  cltbl;          /* Pointer to the cluster link map table (Nulled on file open) */
#endif
#if _FS_LOCK
    UINT    lockid;         /* File lock ID origin from 1 (index of file semaphore table Files[]) */
#endif
#if !_FS_TINY
    UCHAR    buf[_MAX_SS];   /* File private data read/write window */
#endif
} FIL;

/* Directory object structure (DIR) */
typedef struct {
    FATFS*  fs;             /* Pointer to the owner file system object (**do not change order**) */
    WORD    id;             /* Owner file system mount ID (**do not change order**) */
    WORD    index;          /* Current read/write index number */
    DWORD   sclust;         /* Table start cluster (0:Root dir) */
    DWORD   clust;          /* Current cluster */
    DWORD   sect;           /* Current sector */
    UCHAR*   dir;            /* Pointer to the current SFN entry in the win[] */
    UCHAR*   fn;             /* Pointer to the SFN (in/out) {file[8],ext[3],status[1]} */
#if _FS_LOCK
    UINT    lockid;         /* File lock ID (index of file semaphore table Files[]) */
#endif
#if _USE_LFN
    WCHAR*  lfn;            /* Pointer to the LFN working buffer */
    WORD    lfn_idx;        /* Last matched LFN index number (0xFFFF:No LFN) */
#endif
#if _USE_FIND
    const TCHAR*    pat;    /* Pointer to the name matching pattern */
#endif
} DIR;

/* File information structure (FILINFO) */
typedef struct {
    DWORD   fsize;          /* File size */
    WORD    fdate;          /* Last modified date */
    WORD    ftime;          /* Last modified time */
    UCHAR    fattrib;        /* Attribute */
    TCHAR   fname[13];      /* Short file name (8.3 format) */
#if _USE_LFN
    TCHAR*  lfname;         /* Pointer to the LFN buffer */
    UINT    lfsize;         /* Size of LFN buffer in TCHAR */
#endif
} FILINFO;

/* File function return code (FRESULT) */
typedef enum {
    FR_OK = 0,              /* (0) Succeeded */
    FR_DISK_ERR,            /* (1) A hard error occurred in the low level disk I/O layer */
    FR_INT_ERR,             /* (2) Assertion failed */
    FR_NOT_READY,           /* (3) The physical drive cannot work */
    FR_NO_FILE,             /* (4) Could not find the file */
    FR_NO_PATH,             /* (5) Could not find the path */
    FR_INVALID_NAME,        /* (6) The path name format is invalid */
    FR_DENIED,              /* (7) Access denied due to prohibited access or directory full */
    FR_EXIST,               /* (8) Access denied due to prohibited access */
    FR_INVALID_OBJECT,      /* (9) The file/directory object is invalid */
    FR_WRITE_PROTECTED,     /* (10) The physical drive is write protected */
    FR_INVALID_DRIVE,       /* (11) The logical drive number is invalid */
    FR_NOT_ENABLED,         /* (12) The volume has no work area */
    FR_NO_FILESYSTEM,       /* (13) There is no valid FAT volume */
    FR_MKFS_ABORTED,        /* (14) The f_mkfs() aborted due to any parameter error */
    FR_TIMEOUT,             /* (15) Could not get a grant to access the volume within defined period */
    FR_LOCKED,              /* (16) The operation is rejected according to the file sharing policy */
    FR_NOT_ENOUGH_CORE,     /* (17) LFN working buffer could not be allocated */
    FR_TOO_MANY_OPEN_FILES, /* (18) Number of open files > _FS_LOCK */
    FR_INVALID_PARAMETER    /* (19) Given parameter is invalid */
} FRESULT;

/*--------------------------------------------------------------*/
/* FatFs module application interface                           */
FRESULT f_open (FIL* fp, const TCHAR* path, UCHAR mode);             /* Open or create a file */
FRESULT f_close (FIL* fp);                                          /* Close an open file object */
FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);           /* Read data from a file */
FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);    /* Write data to a file */
FRESULT f_forward (FIL* fp, UINT(*func)(const UCHAR*,UINT), UINT btf, UINT* bf); /* Forward data to the stream */
FRESULT f_lseek (FIL* fp, DWORD ofs);                               /* Move file pointer of a file object */
FRESULT f_truncate (FIL* fp);                                       /* Truncate file */
FRESULT f_sync (FIL* fp);                                           /* Flush cached data of a writing file */
FRESULT f_opendir (DIR* dp, const TCHAR* path);                     /* Open a directory */
FRESULT f_closedir (DIR* dp);                                       /* Close an open directory */
FRESULT f_readdir (DIR* dp, FILINFO* fno);                          /* Read a directory item */
FRESULT f_findfirst (DIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern);   /* Find first file */
FRESULT f_findnext (DIR* dp, FILINFO* fno);                         /* Find next file */
FRESULT f_mkdir (const TCHAR* path);                                /* Create a sub directory */
FRESULT f_unlink (const TCHAR* path);                               /* Delete an existing file or directory */
FRESULT f_rename (const TCHAR* path_old, const TCHAR* path_new);    /* Rename/Move a file or directory */
FRESULT f_stat (const TCHAR* path, FILINFO* fno);                   /* Get file status */
FRESULT f_chmod (const TCHAR* path, UCHAR attr, UCHAR mask);          /* Change attribute of the file/dir */
FRESULT f_utime (const TCHAR* path, const FILINFO* fno);            /* Change times-tamp of the file/dir */
FRESULT f_chdir (const TCHAR* path);                                /* Change current directory */
FRESULT f_chdrive (const TCHAR* path);                              /* Change current drive */
FRESULT f_getcwd (TCHAR* buff, UINT len);                           /* Get current directory */
FRESULT f_getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs); /* Get number of free clusters on the drive */
FRESULT f_getlabel (const TCHAR* path, TCHAR* label, DWORD* vsn);   /* Get volume label */
FRESULT f_setlabel (const TCHAR* label);                            /* Set volume label */
FRESULT f_mount (FATFS* fs, const TCHAR* path, UCHAR opt);           /* Mount/Unmount a logical drive */
FRESULT f_mkfs (const TCHAR* path, UCHAR sfd, UINT au);              /* Create a file system on the volume */
FRESULT f_fdisk (UCHAR pdrv, const DWORD szt[], void* work);         /* Divide a physical drive into some partitions */
int f_putc (TCHAR c, FIL* fp);                                      /* Put a character to the file */
int f_puts (const TCHAR* str, FIL* cp);                             /* Put a string to the file */
int f_printf (FIL* fp, const TCHAR* str, ...);                      /* Put a formatted string to the file */
TCHAR* f_gets (TCHAR* buff, int len, FIL* fp);                      /* Get a string from the file */

#define f_eof(fp) ((int)((fp)->fptr == (fp)->fsize))
#define f_error(fp) ((fp)->err)
#define f_tell(fp) ((fp)->fptr)
#define f_size(fp) ((fp)->fsize)
#define f_rewind(fp) f_lseek((fp), 0)
#define f_rewinddir(dp) f_readdir((dp), 0)

#ifndef EOF
#define EOF (-1)
#endif

/*--------------------------------------------------------------*/
/* Additional user defined functions                            */
/* RTC function */
#if !_FS_READONLY && !_FS_NORTC
DWORD get_fattime (void);
#endif

/* Unicode support functions */
#if _USE_LFN                            /* Unicode - OEM code conversion */
WCHAR ff_convert (WCHAR chr, UINT dir); /* OEM-Unicode bidirectional conversion */
WCHAR ff_wtoupper (WCHAR chr);          /* Unicode upper-case conversion */
#if _USE_LFN == 3                       /* Memory functions */
void* ff_memalloc (UINT msize);         /* Allocate memory block */
void ff_memfree (void* mblock);         /* Free memory block */
#endif
#endif

/* Sync functions */
#if _FS_REENTRANT
int ff_cre_syncobj (UCHAR vol, _SYNC_t* sobj);   /* Create a sync object */
int ff_req_grant (_SYNC_t sobj);                /* Lock sync object */
void ff_rel_grant (_SYNC_t sobj);               /* Unlock sync object */
int ff_del_syncobj (_SYNC_t sobj);              /* Delete a sync object */
#endif

/*--------------------------------------------------------------*/
/* Flags and offset address                                     */
/* File access control and file status flags (FIL.flag) */

#define FA_READ             0x01
#define FA_OPEN_EXISTING    0x00

#if !_FS_READONLY
#define FA_WRITE            0x02
#define FA_CREATE_NEW       0x04
#define FA_CREATE_ALWAYS    0x08
#define FA_OPEN_ALWAYS      0x10
#define FA__WRITTEN         0x20
#define FA__DIRTY           0x40
#endif

/* FAT sub type (FATFS.fs_type) */
#define FS_FAT12    1
#define FS_FAT16    2
#define FS_FAT32    3

/* File attribute bits for directory entry */
#define AM_RDO  0x01    /* Read only */
#define AM_HID  0x02    /* Hidden */
#define AM_SYS  0x04    /* System */
#define AM_VOL  0x08    /* Volume label */
#define AM_LFN  0x0F    /* LFN entry */
#define AM_DIR  0x10    /* Directory */
#define AM_ARC  0x20    /* Archive */
#define AM_MASK 0x3F    /* Mask of defined bits */

/* Fast seek feature */
#define CREATE_LINKMAP  0xFFFFFFFF

/*--------------------------------*/
/* Multi-byte word access macros  */
#if _WORD_ACCESS == 1   /* Enable word access to the FAT structure */
#define LD_WORD(ptr)        (WORD)(*(WORD*)(UCHAR*)(ptr))
#define LD_DWORD(ptr)       (DWORD)(*(DWORD*)(UCHAR*)(ptr))
#define ST_WORD(ptr,val)    *(WORD*)(UCHAR*)(ptr)=(WORD)(val)
#define ST_DWORD(ptr,val)   *(DWORD*)(UCHAR*)(ptr)=(DWORD)(val)
#else                   /* Use byte-by-byte access to the FAT structure */
#define LD_WORD(ptr)        (WORD)(((WORD)*((UCHAR*)(ptr)+1)<<8)|(WORD)*(UCHAR*)(ptr))
#define LD_DWORD(ptr)       (DWORD)(((DWORD)*((UCHAR*)(ptr)+3)<<24)|((DWORD)*((UCHAR*)(ptr)+2)<<16)|((WORD)*((UCHAR*)(ptr)+1)<<8)|*(UCHAR*)(ptr))
#define ST_WORD(ptr,val)    *(UCHAR*)(ptr)=(UCHAR)(val); *((UCHAR*)(ptr)+1)=(UCHAR)((WORD)(val)>>8)
#define ST_DWORD(ptr,val)   *(UCHAR*)(ptr)=(UCHAR)(val); *((UCHAR*)(ptr)+1)=(UCHAR)((WORD)(val)>>8); *((UCHAR*)(ptr)+2)=(UCHAR)((DWORD)(val)>>16); *((UCHAR*)(ptr)+3)=(UCHAR)((DWORD)(val)>>24)
#endif

#ifdef __cplusplus
}
#endif

/****************************************************** ccsbcs.c ************************************************************/
#if _CODE_PAGE == 437
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP437(0x80-0xFF) to Unicode conversion table */
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, 0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, 0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 720
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP720(0x80-0xFF) to Unicode conversion table */
    0x0000, 0x0000, 0x00E9, 0x00E2, 0x0000, 0x00E0, 0x0000, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0651, 0x0652, 0x00F4, 0x00A4, 0x0640, 0x00FB, 0x00F9, 0x0621, 0x0622, 0x0623, 0x0624, 0x00A3, 0x0625, 0x0626, 0x0627,
    0x0628, 0x0629, 0x062A, 0x062B, 0x062C, 0x062D, 0x062E, 0x062F, 0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x0636, 0x0637, 0x0638, 0x0639, 0x063A, 0x0641, 0x00B5, 0x0642, 0x0643, 0x0644, 0x0645, 0x0646, 0x0647, 0x0648, 0x0649, 0x064A,
    0x2261, 0x064B, 0x064C, 0x064D, 0x064E, 0x064F, 0x0650, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 737
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP737(0x80-0xFF) to Unicode conversion table */
    0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x0398, 0x0399, 0x039A, 0x039B, 0x039C, 0x039D, 0x039E, 0x039F, 0x03A0,
    0x03A1, 0x03A3, 0x03A4, 0x03A5, 0x03A6, 0x03A7, 0x03A8, 0x03A9, 0x03B1, 0x03B2, 0x03B3, 0x03B4, 0x03B5, 0x03B6, 0x03B7, 0x03B8,
    0x03B9, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BE, 0x03BF, 0x03C0, 0x03C1, 0x03C3, 0x03C2, 0x03C4, 0x03C5, 0x03C6, 0x03C7, 0x03C8,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03C9, 0x03AC, 0x03AD, 0x03AE, 0x03CA, 0x03AF, 0x03CC, 0x03CD, 0x03CB, 0x03CE, 0x0386, 0x0388, 0x0389, 0x038A, 0x038C, 0x038E,
    0x038F, 0x00B1, 0x2265, 0x2264, 0x03AA, 0x03AB, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 771
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP771(0x80-0xFF) to Unicode conversion table */
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x2558, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x0104, 0x0105, 0x010C, 0x010D,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F,
    0x0118, 0x0119, 0x0116, 0x0117, 0x012E, 0x012F, 0x0160, 0x0161, 0x0172, 0x0173, 0x016A, 0x016B, 0x017D, 0x017E, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 775
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP775(0x80-0xFF) to Unicode conversion table */
    0x0106, 0x00FC, 0x00E9, 0x0101, 0x00E4, 0x0123, 0x00E5, 0x0107, 0x0142, 0x0113, 0x0156, 0x0157, 0x012B, 0x0179, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x014D, 0x00F6, 0x0122, 0x00A2, 0x015A, 0x015B, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x00A4,
    0x0100, 0x012A, 0x00F3, 0x017B, 0x017C, 0x017A, 0x201D, 0x00A6, 0x00A9, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x0141, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x0104, 0x010C, 0x0118, 0x0116, 0x2563, 0x2551, 0x2557, 0x255D, 0x012E, 0x0160, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x0172, 0x016A, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x017D,
    0x0105, 0x010D, 0x0119, 0x0117, 0x012F, 0x0161, 0x0173, 0x016B, 0x017E, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x00D3, 0x00DF, 0x014C, 0x0143, 0x00F5, 0x00D5, 0x00B5, 0x0144, 0x0136, 0x0137, 0x013B, 0x013C, 0x0146, 0x0112, 0x0145, 0x2019,
    0x00AD, 0x00B1, 0x201C, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x201E, 0x00B0, 0x2219, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 850
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP850(0x80-0xFF) to Unicode conversion table */
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, 0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0, 0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
    0x00F0, 0x00D0, 0x00CA, 0x00CB, 0x00C8, 0x0131, 0x00CD, 0x00CE, 0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580,
    0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x00FE, 0x00DE, 0x00DA, 0x00DB, 0x00D9, 0x00FD, 0x00DD, 0x00AF, 0x00B4,
    0x00AD, 0x00B1, 0x2017, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8, 0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 852
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP852(0x80-0xFF) to Unicode conversion table */
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x016F, 0x0107, 0x00E7, 0x0142, 0x00EB, 0x0150, 0x0151, 0x00EE, 0x0179, 0x00C4, 0x0106,
    0x00C9, 0x0139, 0x013A, 0x00F4, 0x00F6, 0x013D, 0x013E, 0x015A, 0x015B, 0x00D6, 0x00DC, 0x0164, 0x0165, 0x0141, 0x00D7, 0x010D,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x0104, 0x0105, 0x017D, 0x017E, 0x0118, 0x0119, 0x00AC, 0x017A, 0x010C, 0x015F, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x011A, 0x015E, 0x2563, 0x2551, 0x2557, 0x255D, 0x017B, 0x017C, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x0102, 0x0103, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
    0x0111, 0x0110, 0x010E, 0x00CB, 0x010F, 0x0147, 0x00CD, 0x00CE, 0x011B, 0x2518, 0x250C, 0x2588, 0x2584, 0x0162, 0x016E, 0x2580,
    0x00D3, 0x00DF, 0x00D4, 0x0143, 0x0144, 0x0148, 0x0160, 0x0161, 0x0154, 0x00DA, 0x0155, 0x0170, 0x00FD, 0x00DD, 0x0163, 0x00B4,
    0x00AD, 0x02DD, 0x02DB, 0x02C7, 0x02D8, 0x00A7, 0x00F7, 0x00B8, 0x00B0, 0x00A8, 0x02D9, 0x0171, 0x0158, 0x0159, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 855
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP855(0x80-0xFF) to Unicode conversion table */
    0x0452, 0x0402, 0x0453, 0x0403, 0x0451, 0x0401, 0x0454, 0x0404, 0x0455, 0x0405, 0x0456, 0x0406, 0x0457, 0x0407, 0x0458, 0x0408,
    0x0459, 0x0409, 0x045A, 0x040A, 0x045B, 0x040B, 0x045C, 0x040C, 0x045E, 0x040E, 0x045F, 0x040F, 0x044E, 0x042E, 0x044A, 0x042A,
    0x0430, 0x0410, 0x0431, 0x0411, 0x0446, 0x0426, 0x0434, 0x0414, 0x0435, 0x0415, 0x0444, 0x0424, 0x0433, 0x0413, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x0445, 0x0425, 0x0438, 0x0418, 0x2563, 0x2551, 0x2557, 0x255D, 0x0439, 0x0419, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x043A, 0x041A, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
    0x043B, 0x041B, 0x043C, 0x041C, 0x043D, 0x041D, 0x043E, 0x041E, 0x043F, 0x2518, 0x250C, 0x2588, 0x2584, 0x041F, 0x044F, 0x2580,
    0x042F, 0x0440, 0x0420, 0x0441, 0x0421, 0x0442, 0x0422, 0x0443, 0x0423, 0x0436, 0x0416, 0x0432, 0x0412, 0x044C, 0x042C, 0x2116,
    0x00AD, 0x044B, 0x042B, 0x0437, 0x0417, 0x0448, 0x0428, 0x044D, 0x042D, 0x0449, 0x0429, 0x0447, 0x0427, 0x00A7, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 857
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP857(0x80-0xFF) to Unicode conversion table */
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x0131, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, 0x0130, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x015E, 0x015F,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x011E, 0x011F, 0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0, 0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
    0x00BA, 0x00AA, 0x00CA, 0x00CB, 0x00C8, 0x0000, 0x00CD, 0x00CE, 0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580,
    0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x0000, 0x00D7, 0x00DA, 0x00DB, 0x00D9, 0x00EC, 0x00FF, 0x00AF, 0x00B4,
    0x00AD, 0x00B1, 0x0000, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8, 0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 860
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP860(0x80-0xFF) to Unicode conversion table */
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E3, 0x00E0, 0x00C1, 0x00E7, 0x00EA, 0x00CA, 0x00E8, 0x00CD, 0x00D4, 0x00EC, 0x00C3, 0x00C2,
    0x00C9, 0x00C0, 0x00C8, 0x00F4, 0x00F5, 0x00F2, 0x00DA, 0x00F9, 0x00CC, 0x00D5, 0x00DC, 0x00A2, 0x00A3, 0x00D9, 0x20A7, 0x00D3,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x00D2, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x2558, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, 0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 861
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP861(0x80-0xFF) to Unicode conversion table */
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E6, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00D0, 0x00F0, 0x00DE, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00FE, 0x00FB, 0x00DD, 0x00FD, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x20A7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00C1, 0x00CD, 0x00D3, 0x00DA, 0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, 0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 862
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP862(0x80-0xFF) to Unicode conversion table */
    0x05D0, 0x05D1, 0x05D2, 0x05D3, 0x05D4, 0x05D5, 0x05D6, 0x05D7, 0x05D8, 0x05D9, 0x05DA, 0x05DB, 0x05DC, 0x05DD, 0x05DE, 0x05DF,
    0x05E0, 0x05E1, 0x05E2, 0x05E3, 0x05E4, 0x05E5, 0x05E6, 0x05E7, 0x05E8, 0x05E9, 0x05EA, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, 0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 863
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP863(0x80-0xFF) to Unicode conversion table */
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00C2, 0x00E0, 0x00B6, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x2017, 0x00C0,
    0x00C9, 0x00C8, 0x00CA, 0x00F4, 0x00CB, 0x00CF, 0x00FB, 0x00F9, 0x00A4, 0x00D4, 0x00DC, 0x00A2, 0x00A3, 0x00D9, 0x00DB, 0x0192,
    0x00A6, 0x00B4, 0x00F3, 0x00FA, 0x00A8, 0x00BB, 0x00B3, 0x00AF, 0x00CE, 0x3210, 0x00AC, 0x00BD, 0x00BC, 0x00BE, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, 0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2219,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 864
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP864(0x80-0xFF) to Unicode conversion table */
    0x00B0, 0x00B7, 0x2219, 0x221A, 0x2592, 0x2500, 0x2502, 0x253C, 0x2524, 0x252C, 0x251C, 0x2534, 0x2510, 0x250C, 0x2514, 0x2518,
    0x03B2, 0x221E, 0x03C6, 0x00B1, 0x00BD, 0x00BC, 0x2248, 0x00AB, 0x00BB, 0xFEF7, 0xFEF8, 0x0000, 0x0000, 0xFEFB, 0xFEFC, 0x0000,
    0x00A0, 0x00AD, 0xFE82, 0x00A3, 0x00A4, 0xFE84, 0x0000, 0x20AC, 0xFE8E, 0xFE8F, 0xFE95, 0xFE99, 0x060C, 0xFE9D, 0xFEA1, 0xFEA5,
    0x0660, 0x0661, 0x0662, 0x0663, 0x0664, 0x0665, 0x0666, 0x0667, 0x0668, 0x0669, 0xFED1, 0x061B, 0xFEB1, 0xFEB5, 0xFEB9, 0x061F,
    0x00A2, 0xFE80, 0xFE81, 0xFE83, 0xFE85, 0xFECA, 0xFE8B, 0xFE8D, 0xFE91, 0xFE93, 0xFE97, 0xFE9B, 0xFE9F, 0xFEA3, 0xFEA7, 0xFEA9,
    0xFEAB, 0xFEAD, 0xFEAF, 0xFEB3, 0xFEB7, 0xFEBB, 0xFEBF, 0xFEC1, 0xFEC5, 0xFECB, 0xFECF, 0x00A6, 0x00AC, 0x00F7, 0x00D7, 0xFEC9,
    0x0640, 0xFED3, 0xFED7, 0xFEDB, 0xFEDF, 0xFEE3, 0xFEE7, 0xFEEB, 0xFEED, 0xFEEF, 0xFEF3, 0xFEBD, 0xFECC, 0xFECE, 0xFECD, 0xFEE1,
    0xFE7D, 0x0651, 0xFEE5, 0xFEE9, 0xFEEC, 0xFEF0, 0xFEF2, 0xFED0, 0xFED5, 0xFEF5, 0xFEF6, 0xFEDD, 0xFED9, 0xFEF1, 0x25A0, 0x0000
};

#elif _CODE_PAGE == 865
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP865(0x80-0xFF) to Unicode conversion table */
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C5, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, 0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x20A7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00A4,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x2558, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, 0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 866
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP866(0x80-0xFF) to Unicode conversion table */
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F,
    0x0401, 0x0451, 0x0404, 0x0454, 0x0407, 0x0457, 0x040E, 0x045E, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x2116, 0x00A4, 0x25A0, 0x00A0
};

#elif _CODE_PAGE == 869
#define _TBLDEF 1
static
const WCHAR Tbl[] = {   /*  CP869(0x80-0xFF) to Unicode conversion table */
    0x00B7, 0x00B7, 0x00B7, 0x00B7, 0x00B7, 0x00B7, 0x0386, 0x00B7, 0x00B7, 0x00AC, 0x00A6, 0x2018, 0x2019, 0x0388, 0x2015, 0x0389,
    0x038A, 0x03AA, 0x038C, 0x00B7, 0x00B7, 0x038E, 0x03AB, 0x00A9, 0x038F, 0x00B2, 0x00B3, 0x03AC, 0x00A3, 0x03AD, 0x03AE, 0x03AF,
    0x03CA, 0x0390, 0x03CC, 0x03CD, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x00BD, 0x0398, 0x0399, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x039A, 0x039B, 0x039C, 0x039D, 0x2563, 0x2551, 0x2557, 0x255D, 0x039E, 0x039F, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x0A30, 0x03A1, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x03A3,
    0x03A4, 0x03A5, 0x03A6, 0x03A7, 0x03A8, 0x03A9, 0x03B1, 0x03B2, 0x03B3, 0x2518, 0x250C, 0x2588, 0x2584, 0x03B4, 0x03B5, 0x2580,
    0x03B6, 0x03B7, 0x03B8, 0x03B9, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BE, 0x03BF, 0x03C0, 0x03C1, 0x03C3, 0x03C2, 0x03C4, 0x0384,
    0x00AD, 0x00B1, 0x03C5, 0x03C6, 0x03C7, 0x00A7, 0x03C8, 0x0385, 0x00B0, 0x00A8, 0x03C9, 0x03CB, 0x03B0, 0x03CE, 0x25A0, 0x00A0
};

#endif

#if !_TBLDEF || !_USE_LFN
#error This file is not needed at current configuration. Remove from the project.
#endif

WCHAR ff_convert (  /* Converted character, Returns zero on error */
    WCHAR   chr,    /* Character code to be converted */
    UINT    dir     /* 0: Unicode to OEM code, 1: OEM code to Unicode */
)
{
    WCHAR c;


    if (chr < 0x80) {   /* ASCII */
        c = chr;

    } else {
        if (dir) {      /* OEM code to Unicode */
            c = (chr >= 0x100) ? 0 : Tbl[chr - 0x80];

        } else {        /* Unicode to OEM code */
            for (c = 0; c < 0x80; c++) {
                if (chr == Tbl[c]) break;
            }
            c = (c + 0x80) & 0xFF;
        }
    }
    return c;
}

WCHAR ff_wtoupper ( /* Returns upper converted character */
    WCHAR chr       /* Unicode character to be upper converted */
)
{
    static const WCHAR lower[] = {  /* Lower case characters to be converted */
    /* Latin Supplement */          0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
    /* Latin Extended-A */          0x101,0x103,0x105,0x107,0x109,0x10B,0x10D,0x10F,0x111,0x113,0x115,0x117,0x119,0x11B,0x11D,0x11F,0x121,0x123,0x125,0x127,0x129,0x12B,0x12D,0x12F,0x131,0x133,0x135,0x137,0x13A,0x13C,0x13E,0x140,0x142,0x144,0x146,0x148,0x14B,0x14D,0x14F,0x151,0x153,0x155,0x157,0x159,0x15B,0x15D,0x15F,0x161,0x163,0x165,0x167,0x169,0x16B,0x16D,0x16F,0x171,0x173,0x175,0x177,0x17A,0x17C,0x17E,
    /* Latin Extended-B */          0x183,0x185,0x188,0x18C,0x192,0x199,0x1A1,0x1A3,0x1A8,0x1AD,0x1B0,0x1B4,0x1B6,0x1B9,0x1BD,0x1C6,0x1C9,0x1CC,0x1CE,0x1D0,0x1D2,0x1D4,0x1D6,0x1D8,0x1DA,0x1DC,0x1DD,0x1DF,0x1E1,0x1E3,0x1E5,0x1E7,0x1E9,0x1EB,0x1ED,0x1EF,0x1F3,0x1F5,0x1FB,0x1FD,0x1FF,0x201,0x203,0x205,0x207,0x209,0x20B,0x20D,0x20F,0x211,0x213,0x215,0x217,
    /* Greek, Coptic */             0x3B1,0x3B2,0x3B3,0x3B4,0x3B5,0x3B6,0x3B7,0x3B8,0x3B9,0x3BA,0x3BB,0x3BC,0x3BD,0x3BE,0x3BF,0x3C0,0x3C1,0x3C3,0x3C4,0x3C5,0x3C6,0x3C7,0x3C8,0x3C9,0x3CA,0x3CB,0x3CC,0x3CD,0x3CE,0x3E3,0x3E5,0x3E7,0x3E9,0x3EB,
    /* Cyrillic */                  0x430,0x431,0x432,0x433,0x434,0x435,0x436,0x437,0x438,0x439,0x43A,0x43B,0x43C,0x43D,0x43E,0x43F,0x440,0x441,0x442,0x443,0x444,0x445,0x446,0x447,0x448,0x449,0x44A,0x44B,0x44C,0x44D,0x44E,0x44F,0x452,0x453,0x454,0x455,0x456,0x457,0x458,0x459,0x45A,0x45B,0x45C,0x45E,0x45F,0x461,0x463,0x465,0x467,0x469,0x46B,0x46D,0x46F,0x471,0x473,0x475,0x477,0x479,0x47B,0x47D,0x47F,0x481,0x491,0x493,0x495,0x497,0x499,0x49B,0x49D,0x49F,0x4A1,0x4A3,0x4A5,0x4A7,0x4A9,0x4AB,0x4AD,0x4AF,0x4B1,0x4B3,0x4B5,0x4B7,0x4B9,0x4BB,0x4BD,0x4BF,0x4C2,0x4C4,0x4C8,0x4D1,0x4D3,0x4D5,0x4D7,0x4D9,0x4DB,0x4DD,0x4DF,0x4E1,0x4E3,0x4E5,0x4E7,0x4E9,0x4EB,0x4ED,0x4EF,0x4F1,0x4F3,0x4F5,0x4F9,
    /* Armenian */                  0x561,0x562,0x563,0x564,0x565,0x566,0x567,0x568,0x569,0x56A,0x56B,0x56C,0x56D,0x56E,0x56F,0x570,0x571,0x572,0x573,0x574,0x575,0x576,0x577,0x578,0x579,0x57A,0x57B,0x57C,0x57D,0x57E,0x57F,0x580,0x581,0x582,0x583,0x584,0x585,0x586,
    /* Latin Extended Additional */ 0x1E01,0x1E03,0x1E05,0x1E07,0x1E09,0x1E0B,0x1E0D,0x1E0F,0x1E11,0x1E13,0x1E15,0x1E17,0x1E19,0x1E1B,0x1E1D,0x1E1F,0x1E21,0x1E23,0x1E25,0x1E27,0x1E29,0x1E2B,0x1E2D,0x1E2F,0x1E31,0x1E33,0x1E35,0x1E37,0x1E39,0x1E3B,0x1E3D,0x1E3F,0x1E41,0x1E43,0x1E45,0x1E47,0x1E49,0x1E4B,0x1E4D,0x1E4F,0x1E51,0x1E53,0x1E55,0x1E57,0x1E59,0x1E5B,0x1E5D,0x1E5F,0x1E61,0x1E63,0x1E65,0x1E67,0x1E69,0x1E6B,0x1E6D,0x1E6F,0x1E71,0x1E73,0x1E75,0x1E77,0x1E79,0x1E7B,0x1E7D,0x1E7F,0x1E81,0x1E83,0x1E85,0x1E87,0x1E89,0x1E8B,0x1E8D,0x1E8F,0x1E91,0x1E93,0x1E95,0x1E97,0x1E99,0x1E9B,0x1E9D,0x1E9F,0x1EA1,0x1EA3,0x1EA5,0x1EA7,0x1EA9,0x1EAB,0x1EAD,0x1EAF,0x1EB1,0x1EB3,0x1EB5,0x1EB7,0x1EB9,0x1EBB,0x1EBD,0x1EBF,0x1EC1,0x1EC3,0x1EC5,0x1EC7,0x1EC9,0x1ECB,0x1ECD,0x1ECF,0x1ED1,0x1ED3,0x1ED5,0x1ED7,0x1ED9,0x1EDB,0x1EDD,0x1EDF,0x1EE1,0x1EE3,0x1EE5,0x1EE7,0x1EE9,0x1EEB,0x1EED,0x1EEF,0x1EF1,0x1EF3,0x1EF5,0x1EF7,0x1EF9,
    /* Number forms */              0x2170,0x2171,0x2172,0x2173,0x2174,0x2175,0x2176,0x2177,0x2178,0x2179,0x217A,0x217B,0x217C,0x217D,0x217E,0x217F,
    /* Full-width */                0xFF41,0xFF42,0xFF43,0xFF44,0xFF45,0xFF46,0xFF47,0xFF48,0xFF49,0xFF4A,0xFF4B,0xFF4C,0xFF4D,0xFF4E,0xFF4F,0xFF50,0xFF51,0xFF52,0xFF53,0xFF54,0xFF55,0xFF56,0xFF57,0xFF58,0xFF59,0xFF5A
    };
    static const WCHAR upper[] = {  /* Upper case characters correspond to lower[] */
                                    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0x178,
                                    0x100,0x102,0x104,0x106,0x108,0x10A,0x10C,0x10E,0x110,0x112,0x114,0x116,0x118,0x11A,0x11C,0x11E,0x120,0x122,0x124,0x126,0x128,0x12A,0x12C,0x12E,0x130,0x132,0x134,0x136,0x139,0x13B,0x13D,0x13F,0x141,0x143,0x145,0x147,0x14A,0x14C,0x14E,0x150,0x152,0x154,0x156,0x158,0x15A,0x15C,0x15E,0x160,0x162,0x164,0x166,0x168,0x16A,0x16C,0x16E,0x170,0x172,0x174,0x176,0x179,0x17B,0x17D,
                                    0x182,0x184,0x187,0x18B,0x191,0x198,0x1A0,0x1A2,0x1A7,0x1AC,0x1AF,0x1B3,0x1B5,0x1B8,0x1BC,0x1C4,0x1C7,0x1CA,0x1CD,0x1CF,0x1D1,0x1D3,0x1D5,0x1D7,0x1D9,0x1DB,0x18E,0x1DE,0x1E0,0x1E2,0x1E4,0x1E6,0x1E8,0x1EA,0x1EC,0x1EE,0x1F1,0x1F4,0x1FA,0x1FC,0x1FE,0x200,0x202,0x204,0x206,0x208,0x20A,0x20C,0x20E,0x210,0x212,0x214,0x216,
                                    0x391,0x392,0x393,0x394,0x395,0x396,0x397,0x398,0x399,0x39A,0x39B,0x39C,0x39D,0x39E,0x39F,0x3A0,0x3A1,0x3A3,0x3A4,0x3A5,0x3A6,0x3A7,0x3A8,0x3A9,0x3AA,0x3AB,0x38C,0x38E,0x38F,0x3E2,0x3E4,0x3E6,0x3E8,0x3EA,
                                    0x410,0x411,0x412,0x413,0x414,0x415,0x416,0x417,0x418,0x419,0x41A,0x41B,0x41C,0x41D,0x41E,0x41F,0x420,0x421,0x422,0x423,0x424,0x425,0x426,0x427,0x428,0x429,0x42A,0x42B,0x42C,0x42D,0x42E,0x42F,0x402,0x403,0x404,0x405,0x406,0x407,0x408,0x409,0x40A,0x40B,0x40C,0x40E,0x40F,0x460,0x462,0x464,0x466,0x468,0x46A,0x46C,0x46E,0x470,0x472,0x474,0x476,0x478,0x47A,0x47C,0x47E,0x480,0x490,0x492,0x494,0x496,0x498,0x49A,0x49C,0x49E,0x4A0,0x4A2,0x4A4,0x4A6,0x4A8,0x4AA,0x4AC,0x4AE,0x4B0,0x4B2,0x4B4,0x4B6,0x4B8,0x4BA,0x4BC,0x4BE,0x4C1,0x4C3,0x5C7,0x4D0,0x4D2,0x4D4,0x4D6,0x4D8,0x4DA,0x4DC,0x4DE,0x4E0,0x4E2,0x4E4,0x4E6,0x4E8,0x4EA,0x4EC,0x4EE,0x4F0,0x4F2,0x4F4,0x4F8,
                                    0x531,0x532,0x533,0x534,0x535,0x536,0x537,0x538,0x539,0x53A,0x53B,0x53C,0x53D,0x53E,0x53F,0x540,0x541,0x542,0x543,0x544,0x545,0x546,0x547,0x548,0x549,0x54A,0x54B,0x54C,0x54D,0x54E,0x54F,0x550,0x551,0x552,0x553,0x554,0x555,0x556,
                                    0x1E00,0x1E02,0x1E04,0x1E06,0x1E08,0x1E0A,0x1E0C,0x1E0E,0x1E10,0x1E12,0x1E14,0x1E16,0x1E18,0x1E1A,0x1E1C,0x1E1E,0x1E20,0x1E22,0x1E24,0x1E26,0x1E28,0x1E2A,0x1E2C,0x1E2E,0x1E30,0x1E32,0x1E34,0x1E36,0x1E38,0x1E3A,0x1E3C,0x1E3E,0x1E40,0x1E42,0x1E44,0x1E46,0x1E48,0x1E4A,0x1E4C,0x1E4E,0x1E50,0x1E52,0x1E54,0x1E56,0x1E58,0x1E5A,0x1E5C,0x1E5E,0x1E60,0x1E62,0x1E64,0x1E66,0x1E68,0x1E6A,0x1E6C,0x1E6E,0x1E70,0x1E72,0x1E74,0x1E76,0x1E78,0x1E7A,0x1E7C,0x1E7E,0x1E80,0x1E82,0x1E84,0x1E86,0x1E88,0x1E8A,0x1E8C,0x1E8E,0x1E90,0x1E92,0x1E94,0x1E96,0x1E98,0x1E9A,0x1E9C,0x1E9E,0x1EA0,0x1EA2,0x1EA4,0x1EA6,0x1EA8,0x1EAA,0x1EAC,0x1EAE,0x1EB0,0x1EB2,0x1EB4,0x1EB6,0x1EB8,0x1EBA,0x1EBC,0x1EBE,0x1EC0,0x1EC2,0x1EC4,0x1EC6,0x1EC8,0x1ECA,0x1ECC,0x1ECE,0x1ED0,0x1ED2,0x1ED4,0x1ED6,0x1ED8,0x1EDA,0x1EDC,0x1EDE,0x1EE0,0x1EE2,0x1EE4,0x1EE6,0x1EE8,0x1EEA,0x1EEC,0x1EEE,0x1EF0,0x1EF2,0x1EF4,0x1EF6,0x1EF8,
                                    0x2160,0x2161,0x2162,0x2163,0x2164,0x2165,0x2166,0x2167,0x2168,0x2169,0x216A,0x216B,0x216C,0x216D,0x216E,0x216F,
                                    0xFF21,0xFF22,0xFF23,0xFF24,0xFF25,0xFF26,0xFF27,0xFF28,0xFF29,0xFF2A,0xFF2B,0xFF2C,0xFF2D,0xFF2E,0xFF2F,0xFF30,0xFF31,0xFF32,0xFF33,0xFF34,0xFF35,0xFF36,0xFF37,0xFF38,0xFF39,0xFF3A
    };
    UINT i, n, hi, li;

    if (chr < 0x80) {   /* ASCII characters (acceleration) */
        if (chr >= 0x61 && chr <= 0x7A) chr -= 0x20;

    } else {            /* Non ASCII characters (table search) */
        n = 12; li = 0; hi = sizeof lower / sizeof lower[0];
        do {
            i = li + (hi - li) / 2;
            if (chr == lower[i]) break;
            if (chr > lower[i]) li = i; else hi = i;
        } while (--n);
        if (n) chr = upper[i];
    }
    return chr;
}


/****************************************************** ff.c ************************************************************/
// #include "ff.h"         /* Declarations of FatFs API */
// #include "diskio.h"

/*--------------------------------------------------------------------------
   Module Private Definitions
---------------------------------------------------------------------------*/
#if _FATFS != 64180 /* Revision ID */
#error Wrong include file (ff.h).
#endif

/* Reentrancy related */
#if _FS_REENTRANT
#if _USE_LFN == 1
#error Static LFN work area cannot be used at thread-safe configuration
#endif
#define ENTER_FF(fs)        { if (!lock_fs(fs)) return FR_TIMEOUT; }
#define LEAVE_FF(fs, res)   { unlock_fs(fs, res); return res; }
#else
#define ENTER_FF(fs)
#define LEAVE_FF(fs, res)   return res
#endif

#define ABORT(fs, res)      { fp->err = (UCHAR)(res); LEAVE_FF(fs, res); }

/* Definitions of sector size */
#if (_MAX_SS < _MIN_SS) || (_MAX_SS != 512 && _MAX_SS != 1024 && _MAX_SS != 2048 && _MAX_SS != 4096) || (_MIN_SS != 512 && _MIN_SS != 1024 && _MIN_SS != 2048 && _MIN_SS != 4096)
#error Wrong sector size configuration
#endif
#if _MAX_SS == _MIN_SS
#define SS(fs)  ((UINT)_MAX_SS) /* Fixed sector size */
#else
#define SS(fs)  ((fs)->ssize)   /* Variable sector size */
#endif

/* Timestamp feature */
#if _FS_NORTC == 1
#if _NORTC_YEAR < 1980 || _NORTC_YEAR > 2107 || _NORTC_MON < 1 || _NORTC_MON > 12 || _NORTC_MDAY < 1 || _NORTC_MDAY > 31
#error Invalid _FS_NORTC settings
#endif
#define GET_FATTIME()   ((DWORD)(_NORTC_YEAR - 1980) << 25 | (DWORD)_NORTC_MON << 21 | (DWORD)_NORTC_MDAY << 16)
#else
#define GET_FATTIME()   get_fattime()
#endif

/* File access control feature */
#if _FS_LOCK
#if _FS_READONLY
#error _FS_LOCK must be 0 at read-only configuration
#endif
typedef struct {
    FATFS *fs;      /* Object ID 1, volume (NULL:blank entry) */
    DWORD clu;      /* Object ID 2, directory (0:root) */
    WORD idx;       /* Object ID 3, directory index */
    WORD ctr;       /* Object open counter, 0:none, 0x01..0xFF:read mode open count, 0x100:write mode */
} FILESEM;
#endif

/* DBCS code ranges and SBCS upper conversion tables */
#if _CODE_PAGE == 932   /* Japanese Shift-JIS */
#define _DF1S   0x81    /* DBC 1st byte range 1 start */
#define _DF1E   0x9F    /* DBC 1st byte range 1 end */
#define _DF2S   0xE0    /* DBC 1st byte range 2 start */
#define _DF2E   0xFC    /* DBC 1st byte range 2 end */
#define _DS1S   0x40    /* DBC 2nd byte range 1 start */
#define _DS1E   0x7E    /* DBC 2nd byte range 1 end */
#define _DS2S   0x80    /* DBC 2nd byte range 2 start */
#define _DS2E   0xFC    /* DBC 2nd byte range 2 end */

#elif _CODE_PAGE == 936 /* Simplified Chinese GBK */
#define _DF1S   0x81
#define _DF1E   0xFE
#define _DS1S   0x40
#define _DS1E   0x7E
#define _DS2S   0x80
#define _DS2E   0xFE

#elif _CODE_PAGE == 949 /* Korean */
#define _DF1S   0x81
#define _DF1E   0xFE
#define _DS1S   0x41
#define _DS1E   0x5A
#define _DS2S   0x61
#define _DS2E   0x7A
#define _DS3S   0x81
#define _DS3E   0xFE

#elif _CODE_PAGE == 950 /* Traditional Chinese Big5 */
#define _DF1S   0x81
#define _DF1E   0xFE
#define _DS1S   0x40
#define _DS1E   0x7E
#define _DS2S   0xA1
#define _DS2E   0xFE

#elif _CODE_PAGE == 437 /* U.S. */
#define _DF1S   0
#define _EXCVT {0x80,0x9A,0x45,0x41,0x8E,0x41,0x8F,0x80,0x45,0x45,0x45,0x49,0x49,0x49,0x8E,0x8F, \
                0x90,0x92,0x92,0x4F,0x99,0x4F,0x55,0x55,0x59,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0x41,0x49,0x4F,0x55,0xA5,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 720 /* Arabic */
#define _DF1S   0
#define _EXCVT {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F, \
                0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 737 /* Greek */
#define _DF1S   0
#define _EXCVT {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F, \
                0x90,0x92,0x92,0x93,0x94,0x95,0x96,0x97,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87, \
                0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,0x90,0x91,0xAA,0x92,0x93,0x94,0x95,0x96, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0x97,0xEA,0xEB,0xEC,0xE4,0xED,0xEE,0xEF,0xF5,0xF0,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 771 /* KBL */
#define _DF1S   0
#define _EXCVT {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F, \
                0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDC,0xDE,0xDE, \
                0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0xF0,0xF0,0xF2,0xF2,0xF4,0xF4,0xF6,0xF6,0xF8,0xF8,0xFA,0xFA,0xFC,0xFC,0xFE,0xFF}

#elif _CODE_PAGE == 775 /* Baltic */
#define _DF1S   0
#define _EXCVT {0x80,0x9A,0x91,0xA0,0x8E,0x95,0x8F,0x80,0xAD,0xED,0x8A,0x8A,0xA1,0x8D,0x8E,0x8F, \
                0x90,0x92,0x92,0xE2,0x99,0x95,0x96,0x97,0x97,0x99,0x9A,0x9D,0x9C,0x9D,0x9E,0x9F, \
                0xA0,0xA1,0xE0,0xA3,0xA3,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xB5,0xB6,0xB7,0xB8,0xBD,0xBE,0xC6,0xC7,0xA5,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE5,0xE5,0xE6,0xE3,0xE8,0xE8,0xEA,0xEA,0xEE,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 850 /* Latin 1 */
#define _DF1S   0
#define _EXCVT {0x43,0x55,0x45,0x41,0x41,0x41,0x41,0x43,0x45,0x45,0x45,0x49,0x49,0x49,0x41,0x41, \
                0x45,0x92,0x92,0x4F,0x4F,0x4F,0x55,0x55,0x59,0x4F,0x55,0x4F,0x9C,0x4F,0x9E,0x9F, \
                0x41,0x49,0x4F,0x55,0xA5,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0x41,0x41,0x41,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0x41,0x41,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD1,0xD1,0x45,0x45,0x45,0x49,0x49,0x49,0x49,0xD9,0xDA,0xDB,0xDC,0xDD,0x49,0xDF, \
                0x4F,0xE1,0x4F,0x4F,0x4F,0x4F,0xE6,0xE8,0xE8,0x55,0x55,0x55,0x59,0x59,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 852 /* Latin 2 */
#define _DF1S   0
#define _EXCVT {0x80,0x9A,0x90,0xB6,0x8E,0xDE,0x8F,0x80,0x9D,0xD3,0x8A,0x8A,0xD7,0x8D,0x8E,0x8F, \
                0x90,0x91,0x91,0xE2,0x99,0x95,0x95,0x97,0x97,0x99,0x9A,0x9B,0x9B,0x9D,0x9E,0xAC, \
                0xB5,0xD6,0xE0,0xE9,0xA4,0xA4,0xA6,0xA6,0xA8,0xA8,0xAA,0x8D,0xAC,0xB8,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBD,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC6,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD1,0xD1,0xD2,0xD3,0xD2,0xD5,0xD6,0xD7,0xB7,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE3,0xD5,0xE6,0xE6,0xE8,0xE9,0xE8,0xEB,0xED,0xED,0xDD,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xEB,0xFC,0xFC,0xFE,0xFF}

#elif _CODE_PAGE == 855 /* Cyrillic */
#define _DF1S   0
#define _EXCVT {0x81,0x81,0x83,0x83,0x85,0x85,0x87,0x87,0x89,0x89,0x8B,0x8B,0x8D,0x8D,0x8F,0x8F, \
                0x91,0x91,0x93,0x93,0x95,0x95,0x97,0x97,0x99,0x99,0x9B,0x9B,0x9D,0x9D,0x9F,0x9F, \
                0xA1,0xA1,0xA3,0xA3,0xA5,0xA5,0xA7,0xA7,0xA9,0xA9,0xAB,0xAB,0xAD,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB6,0xB6,0xB8,0xB8,0xB9,0xBA,0xBB,0xBC,0xBE,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC7,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD1,0xD1,0xD3,0xD3,0xD5,0xD5,0xD7,0xD7,0xDD,0xD9,0xDA,0xDB,0xDC,0xDD,0xE0,0xDF, \
                0xE0,0xE2,0xE2,0xE4,0xE4,0xE6,0xE6,0xE8,0xE8,0xEA,0xEA,0xEC,0xEC,0xEE,0xEE,0xEF, \
                0xF0,0xF2,0xF2,0xF4,0xF4,0xF6,0xF6,0xF8,0xF8,0xFA,0xFA,0xFC,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 857 /* Turkish */
#define _DF1S   0
#define _EXCVT {0x80,0x9A,0x90,0xB6,0x8E,0xB7,0x8F,0x80,0xD2,0xD3,0xD4,0xD8,0xD7,0x49,0x8E,0x8F, \
                0x90,0x92,0x92,0xE2,0x99,0xE3,0xEA,0xEB,0x98,0x99,0x9A,0x9D,0x9C,0x9D,0x9E,0x9E, \
                0xB5,0xD6,0xE0,0xE9,0xA5,0xA5,0xA6,0xA6,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC7,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0x49,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE5,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xDE,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 860 /* Portuguese */
#define _DF1S   0
#define _EXCVT {0x80,0x9A,0x90,0x8F,0x8E,0x91,0x86,0x80,0x89,0x89,0x92,0x8B,0x8C,0x98,0x8E,0x8F, \
                0x90,0x91,0x92,0x8C,0x99,0xA9,0x96,0x9D,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0x86,0x8B,0x9F,0x96,0xA5,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 861 /* Icelandic */
#define _DF1S   0
#define _EXCVT {0x80,0x9A,0x90,0x41,0x8E,0x41,0x8F,0x80,0x45,0x45,0x45,0x8B,0x8B,0x8D,0x8E,0x8F, \
                0x90,0x92,0x92,0x4F,0x99,0x8D,0x55,0x97,0x97,0x99,0x9A,0x9D,0x9C,0x9D,0x9E,0x9F, \
                0xA4,0xA5,0xA6,0xA7,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 862 /* Hebrew */
#define _DF1S   0
#define _EXCVT {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F, \
                0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0x41,0x49,0x4F,0x55,0xA5,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 863 /* Canadian-French */
#define _DF1S   0
#define _EXCVT {0x43,0x55,0x45,0x41,0x41,0x41,0x86,0x43,0x45,0x45,0x45,0x49,0x49,0x8D,0x41,0x8F, \
                0x45,0x45,0x45,0x4F,0x45,0x49,0x55,0x55,0x98,0x4F,0x55,0x9B,0x9C,0x55,0x55,0x9F, \
                0xA0,0xA1,0x4F,0x55,0xA4,0xA5,0xA6,0xA7,0x49,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 864 /* Arabic */
#define _DF1S   0
#define _EXCVT {0x80,0x9A,0x45,0x41,0x8E,0x41,0x8F,0x80,0x45,0x45,0x45,0x49,0x49,0x49,0x8E,0x8F, \
                0x90,0x92,0x92,0x4F,0x99,0x4F,0x55,0x55,0x59,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0x41,0x49,0x4F,0x55,0xA5,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 865 /* Nordic */
#define _DF1S   0
#define _EXCVT {0x80,0x9A,0x90,0x41,0x8E,0x41,0x8F,0x80,0x45,0x45,0x45,0x49,0x49,0x49,0x8E,0x8F, \
                0x90,0x92,0x92,0x4F,0x99,0x4F,0x55,0x55,0x59,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0x41,0x49,0x4F,0x55,0xA5,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, \
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 866 /* Russian */
#define _DF1S   0
#define _EXCVT {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F, \
                0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, \
                0x90,0x91,0x92,0x93,0x9d,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, \
                0xF0,0xF0,0xF2,0xF2,0xF4,0xF4,0xF6,0xF6,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF}

#elif _CODE_PAGE == 869 /* Greek 2 */
#define _DF1S   0
#define _EXCVT {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F, \
                0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x86,0x9C,0x8D,0x8F,0x90, \
                0x91,0x90,0x92,0x95,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, \
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, \
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, \
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xA4,0xA5,0xA6,0xD9,0xDA,0xDB,0xDC,0xA7,0xA8,0xDF, \
                0xA9,0xAA,0xAC,0xAD,0xB5,0xB6,0xB7,0xB8,0xBD,0xBE,0xC6,0xC7,0xCF,0xCF,0xD0,0xEF, \
                0xF0,0xF1,0xD1,0xD2,0xD3,0xF5,0xD4,0xF7,0xF8,0xF9,0xD5,0x96,0x95,0x98,0xFE,0xFF}

#elif _CODE_PAGE == 1   /* ASCII (for only non-LFN cfg) */
#if _USE_LFN
#error Cannot use LFN feature without valid code page.
#endif
#define _DF1S   0

#else
#error Unknown code page

#endif


/* Character code support macros */
#define IsUpper(c)  (((c)>='A')&&((c)<='Z'))
#define IsLower(c)  (((c)>='a')&&((c)<='z'))
#define IsDigit(c)  (((c)>='0')&&((c)<='9'))

#if _DF1S       /* Code page is DBCS */

#ifdef _DF2S    /* Two 1st byte areas */
#define IsDBCS1(c)  (((UCHAR)(c) >= _DF1S && (UCHAR)(c) <= _DF1E) || ((UCHAR)(c) >= _DF2S && (UCHAR)(c) <= _DF2E))
#else           /* One 1st byte area */
#define IsDBCS1(c)  ((UCHAR)(c) >= _DF1S && (UCHAR)(c) <= _DF1E)
#endif

#ifdef _DS3S    /* Three 2nd byte areas */
#define IsDBCS2(c)  (((UCHAR)(c) >= _DS1S && (UCHAR)(c) <= _DS1E) || ((UCHAR)(c) >= _DS2S && (UCHAR)(c) <= _DS2E) || ((UCHAR)(c) >= _DS3S && (UCHAR)(c) <= _DS3E))
#else           /* Two 2nd byte areas */
#define IsDBCS2(c)  (((UCHAR)(c) >= _DS1S && (UCHAR)(c) <= _DS1E) || ((UCHAR)(c) >= _DS2S && (UCHAR)(c) <= _DS2E))
#endif

#else           /* Code page is SBCS */

#define IsDBCS1(c)  0
#define IsDBCS2(c)  0

#endif /* _DF1S */

/* Name status flags */
#define NSFLAG      11      /* Index of name status byte in fn[] */
#define NS_LOSS     0x01    /* Out of 8.3 format */
#define NS_LFN      0x02    /* Force to create LFN entry */
#define NS_LAST     0x04    /* Last segment */
#define NS_BODY     0x08    /* Lower case flag (body) */
#define NS_EXT      0x10    /* Lower case flag (ext) */
#define NS_DOT      0x20    /* Dot entry */

/* FAT sub-type boundaries (Differ from specs but correct for real DOS/Windows) */
#define MIN_FAT16   4086U   /* Minimum number of clusters of FAT16 */
#define MIN_FAT32   65526U  /* Minimum number of clusters of FAT32 */

/* FatFs refers the members in the FAT structures as byte array instead of
/ structure members because the structure is not binary compatible between
/ different platforms */
#define BS_jmpBoot          0       /* x86 jump instruction (3) */
#define BS_OEMName          3       /* OEM name (8) */
#define BPB_BytsPerSec      11      /* Sector size [byte] (2) */
#define BPB_SecPerClus      13      /* Cluster size [sector] (1) */
#define BPB_RsvdSecCnt      14      /* Size of reserved area [sector] (2) */
#define BPB_NumFATs         16      /* Number of FAT copies (1) */
#define BPB_RootEntCnt      17      /* Number of root directory entries for FAT12/16 (2) */
#define BPB_TotSec16        19      /* Volume size [sector] (2) */
#define BPB_Media           21      /* Media descriptor (1) */
#define BPB_FATSz16         22      /* FAT size [sector] (2) */
#define BPB_SecPerTrk       24      /* Track size [sector] (2) */
#define BPB_NumHeads        26      /* Number of heads (2) */
#define BPB_HiddSec         28      /* Number of special hidden sectors (4) */
#define BPB_TotSec32        32      /* Volume size [sector] (4) */
#define BS_DrvNum           36      /* Physical drive number (1) */
#define BS_NTres            37      /* Error flag (1) */
#define BS_BootSig          38      /* Extended boot signature (1) */
#define BS_VolID            39      /* Volume serial number (4) */
#define BS_VolLab           43      /* Volume label (8) */
#define BS_FilSysType       54      /* File system type (1) */
#define BPB_FATSz32         36      /* FAT size [sector] (4) */
#define BPB_ExtFlags        40      /* Extended flags (2) */
#define BPB_FSVer           42      /* File system version (2) */
#define BPB_RootClus        44      /* Root directory first cluster (4) */
#define BPB_FSInfo          48      /* Offset of FSINFO sector (2) */
#define BPB_BkBootSec       50      /* Offset of backup boot sector (2) */
#define BS_DrvNum32         64      /* Physical drive number (1) */
#define BS_NTres32          65      /* Error flag (1) */
#define BS_BootSig32        66      /* Extended boot signature (1) */
#define BS_VolID32          67      /* Volume serial number (4) */
#define BS_VolLab32         71      /* Volume label (8) */
#define BS_FilSysType32     82      /* File system type (1) */
#define FSI_LeadSig         0       /* FSI: Leading signature (4) */
#define FSI_StrucSig        484     /* FSI: Structure signature (4) */
#define FSI_Free_Count      488     /* FSI: Number of free clusters (4) */
#define FSI_Nxt_Free        492     /* FSI: Last allocated cluster (4) */
#define MBR_Table           446     /* MBR: Partition table offset (2) */
#define SZ_PTE              16      /* MBR: Size of a partition table entry */
#define BS_55AA             510     /* Signature word (2) */

#define DIR_Name            0       /* Short file name (11) */
#define DIR_Attr            11      /* Attribute (1) */
#define DIR_NTres           12      /* Lower case flag (1) */
#define DIR_CrtTimeTenth    13      /* Created time sub-second (1) */
#define DIR_CrtTime         14      /* Created time (2) */
#define DIR_CrtDate         16      /* Created date (2) */
#define DIR_LstAccDate      18      /* Last accessed date (2) */
#define DIR_FstClusHI       20      /* Higher 16-bit of first cluster (2) */
#define DIR_WrtTime         22      /* Modified time (2) */
#define DIR_WrtDate         24      /* Modified date (2) */
#define DIR_FstClusLO       26      /* Lower 16-bit of first cluster (2) */
#define DIR_FileSize        28      /* File size (4) */
#define LDIR_Ord            0       /* LFN entry order and LLE flag (1) */
#define LDIR_Attr           11      /* LFN attribute (1) */
#define LDIR_Type           12      /* LFN type (1) */
#define LDIR_Chksum         13      /* Checksum of corresponding SFN entry */
#define LDIR_FstClusLO      26      /* Must be zero (0) */
#define SZ_DIRE             32      /* Size of a directory entry */
#define LLEF                0x40    /* Last long entry flag in LDIR_Ord */
#define DDEM                0xE5    /* Deleted directory entry mark at DIR_Name[0] */
#define RDDEM               0x05    /* Replacement of the character collides with DDEM */

/*--------------------------------------------------------------------------
   Module Private Work Area
---------------------------------------------------------------------------*/
/* Remark: Uninitialized variables with static duration are guaranteed
/  zero/null at start-up. If not, either the linker or start-up routine
/  being used is not compliance with ANSI-C standard.
*/
#if _VOLUMES < 1 || _VOLUMES > 9
#error Wrong _VOLUMES setting
#endif
static FATFS *FatFs[_VOLUMES];  /* Pointer to the file system objects (logical drives) */
static WORD Fsid;               /* File system mount ID */

#if _FS_RPATH && _VOLUMES >= 2
static UCHAR CurrVol;            /* Current drive */
#endif

#if _FS_LOCK
static FILESEM Files[_FS_LOCK]; /* Open object lock semaphores */
#endif

#if _USE_LFN == 0           /* Non LFN feature */
#define DEFINE_NAMEBUF      UCHAR sfn[12]
#define INIT_BUF(dobj)      (dobj).fn = sfn
#define FREE_BUF()
#else
#if _MAX_LFN < 12 || _MAX_LFN > 255
#error Wrong _MAX_LFN setting
#endif
#if _USE_LFN == 1           /* LFN feature with static working buffer */
static WCHAR LfnBuf[_MAX_LFN + 1];
#define DEFINE_NAMEBUF      UCHAR sfn[12]
#define INIT_BUF(dobj)      { (dobj).fn = sfn; (dobj).lfn = LfnBuf; }
#define FREE_BUF()
#elif _USE_LFN == 2         /* LFN feature with dynamic working buffer on the stack */
#define DEFINE_NAMEBUF      UCHAR sfn[12]; WCHAR lbuf[_MAX_LFN + 1]
#define INIT_BUF(dobj)      { (dobj).fn = sfn; (dobj).lfn = lbuf; }
#define FREE_BUF()
#elif _USE_LFN == 3         /* LFN feature with dynamic working buffer on the heap */
#define DEFINE_NAMEBUF      UCHAR sfn[12]; WCHAR *lfn
#define INIT_BUF(dobj)      { lfn = ff_memalloc((_MAX_LFN + 1) * 2); if (!lfn) LEAVE_FF((dobj).fs, FR_NOT_ENOUGH_CORE); (dobj).lfn = lfn; (dobj).fn = sfn; }
#define FREE_BUF()          ff_memfree(lfn)
#else
#error Wrong _USE_LFN setting
#endif
#endif

#ifdef _EXCVT
static const UCHAR ExCvt[] = _EXCVT; /* Upper conversion table for SBCS extended characters */
#endif

/*--------------------------------------------------------------------------
   Module Private Functions
---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* String functions                                                      */
/*-----------------------------------------------------------------------*/

/* Copy memory to memory */
static
void mem_cpy (void* dst, const void* src, UINT cnt) {
    UCHAR *d = (UCHAR*)dst;
    const UCHAR *s = (const UCHAR*)src;

#if _WORD_ACCESS == 1
    while (cnt >= sizeof (int)) {
        *(int*)d = *(int*)s;
        d += sizeof (int); s += sizeof (int);
        cnt -= sizeof (int);
    }
#endif
    while (cnt--)
        *d++ = *s++;
}

/* Fill memory */
static
void mem_set (void* dst, int val, UINT cnt) {
    UCHAR *d = (UCHAR*)dst;

    while (cnt--)
        *d++ = (UCHAR)val;
}

/* Compare memory to memory */
static
int mem_cmp (const void* dst, const void* src, UINT cnt) {
    const UCHAR *d = (const UCHAR *)dst, *s = (const UCHAR *)src;
    int r = 0;

    while (cnt-- && (r = *d++ - *s++) == 0) ;
    return r;
}

/* Check if chr is contained in the string */
static
int chk_chr (const char* str, int chr) {
    while (*str && *str != chr) str++;
    return *str;
}

/*-----------------------------------------------------------------------*/
/* Request/Release grant to access the volume                            */
/*-----------------------------------------------------------------------*/
#if _FS_REENTRANT
static
int lock_fs (
    FATFS* fs       /* File system object */
)
{
    return ff_req_grant(fs->sobj);
}

static
void unlock_fs (
    FATFS* fs,      /* File system object */
    FRESULT res     /* Result code to be returned */
)
{
    if (fs &&
        res != FR_NOT_ENABLED &&
        res != FR_INVALID_DRIVE &&
        res != FR_INVALID_OBJECT &&
        res != FR_TIMEOUT) {
        ff_rel_grant(fs->sobj);
    }
}
#endif

/*-----------------------------------------------------------------------*/
/* File lock control functions                                           */
/*-----------------------------------------------------------------------*/
#if _FS_LOCK

static
FRESULT chk_lock (  /* Check if the file can be accessed */
    DIR* dp,        /* Directory object pointing the file to be checked */
    int acc         /* Desired access type (0:Read, 1:Write, 2:Delete/Rename) */
)
{
    UINT i, be;

    /* Search file semaphore table */
    for (i = be = 0; i < _FS_LOCK; i++) {
        if (Files[i].fs) {  /* Existing entry */
            if (Files[i].fs == dp->fs &&        /* Check if the object matched with an open object */
                Files[i].clu == dp->sclust &&
                Files[i].idx == dp->index) break;
        } else {            /* Blank entry */
            be = 1;
        }
    }
    if (i == _FS_LOCK)  /* The object is not opened */
        return (be || acc == 2) ? FR_OK : FR_TOO_MANY_OPEN_FILES;   /* Is there a blank entry for new object? */

    /* The object has been opened. Reject any open against writing file and all write mode open */
    return (acc || Files[i].ctr == 0x100) ? FR_LOCKED : FR_OK;
}

static
int enq_lock (void) /* Check if an entry is available for a new object */
{
    UINT i;

    for (i = 0; i < _FS_LOCK && Files[i].fs; i++) ;
    return (i == _FS_LOCK) ? 0 : 1;
}

static
UINT inc_lock ( /* Increment object open counter and returns its index (0:Internal error) */
    DIR* dp,    /* Directory object pointing the file to register or increment */
    int acc     /* Desired access (0:Read, 1:Write, 2:Delete/Rename) */
)
{
    UINT i;

    for (i = 0; i < _FS_LOCK; i++) {    /* Find the object */
        if (Files[i].fs == dp->fs &&
            Files[i].clu == dp->sclust &&
            Files[i].idx == dp->index) break;
    }

    if (i == _FS_LOCK) {                /* Not opened. Register it as new. */
        for (i = 0; i < _FS_LOCK && Files[i].fs; i++) ;
        if (i == _FS_LOCK) return 0;    /* No free entry to register (int err) */
        Files[i].fs = dp->fs;
        Files[i].clu = dp->sclust;
        Files[i].idx = dp->index;
        Files[i].ctr = 0;
    }

    if (acc && Files[i].ctr) return 0;  /* Access violation (int err) */

    Files[i].ctr = acc ? 0x100 : Files[i].ctr + 1;  /* Set semaphore value */

    return i + 1;
}


static
FRESULT dec_lock (  /* Decrement object open counter */
    UINT i          /* Semaphore index (1..) */
)
{
    WORD n;
    FRESULT res;


    if (--i < _FS_LOCK) {   /* Shift index number origin from 0 */
        n = Files[i].ctr;
        if (n == 0x100) n = 0;      /* If write mode open, delete the entry */
        if (n) n--;                 /* Decrement read mode open count */
        Files[i].ctr = n;
        if (!n) Files[i].fs = 0;    /* Delete the entry if open count gets zero */
        res = FR_OK;
    } else {
        res = FR_INT_ERR;           /* Invalid index nunber */
    }
    return res;
}


static
void clear_lock (   /* Clear lock entries of the volume */
    FATFS *fs
)
{
    UINT i;

    for (i = 0; i < _FS_LOCK; i++) {
        if (Files[i].fs == fs) Files[i].fs = 0;
    }
}
#endif

/*-----------------------------------------------------------------------*/
/* Move/Flush disk access window in the file system object               */
/*-----------------------------------------------------------------------*/
#if !_FS_READONLY
static
FRESULT sync_window (   /* FR_OK:succeeded, !=0:error */
    FATFS* fs       /* File system object */
)
{
    DWORD wsect;
    UINT nf;
    FRESULT res = FR_OK;


    if (fs->wflag) {    /* Write back the sector if it is dirty */
        wsect = fs->winsect;    /* Current sector number */
        if (disk_write(fs->drv, fs->win, wsect, 1) != RES_OK) {
            res = FR_DISK_ERR;
        } else {
            fs->wflag = 0;
            if (wsect - fs->fatbase < fs->fsize) {      /* Is it in the FAT area? */
                for (nf = fs->n_fats; nf >= 2; nf--) {  /* Reflect the change to all FAT copies */
                    wsect += fs->fsize;
                    disk_write(fs->drv, fs->win, wsect, 1);
                }
            }
        }
    }
    return res;
}
#endif

static
FRESULT move_window (   /* FR_OK(0):succeeded, !=0:error */
    FATFS* fs,      /* File system object */
    DWORD sector    /* Sector number to make appearance in the fs->win[] */
)
{
    FRESULT res = FR_OK;


    if (sector != fs->winsect) {    /* Window offset changed? */
#if !_FS_READONLY
        res = sync_window(fs);      /* Write-back changes */
#endif
        if (res == FR_OK) {         /* Fill sector window with new data */
            if (disk_read(fs->drv, fs->win, sector, 1) != RES_OK) {
                sector = 0xFFFFFFFF;    /* Invalidate window if data is not reliable */
                res = FR_DISK_ERR;
            }
            fs->winsect = sector;
        }
    }
    return res;
}

/*-----------------------------------------------------------------------*/
/* Synchronize file system and strage device                             */
/*-----------------------------------------------------------------------*/
#if !_FS_READONLY
static
FRESULT sync_fs (   /* FR_OK:succeeded, !=0:error */
    FATFS* fs       /* File system object */
)
{
    FRESULT res;
    res = sync_window(fs);
    if (res == FR_OK) {
        /* Update FSInfo sector if needed */
        if (fs->fs_type == FS_FAT32 && fs->fsi_flag == 1) {
            /* Create FSInfo structure */
            mem_set(fs->win, 0, SS(fs));
            ST_WORD(fs->win + BS_55AA, 0xAA55);
            ST_DWORD(fs->win + FSI_LeadSig, 0x41615252);
            ST_DWORD(fs->win + FSI_StrucSig, 0x61417272);
            ST_DWORD(fs->win + FSI_Free_Count, fs->free_clust);
            ST_DWORD(fs->win + FSI_Nxt_Free, fs->last_clust);
            /* Write it into the FSInfo sector */
            fs->winsect = fs->volbase + 1;
            disk_write(fs->drv, fs->win, fs->winsect, 1);
            fs->fsi_flag = 0;
        }
        /* Make sure that no pending write process in the physical drive */
        if (disk_ioctl(fs->drv, CTRL_SYNC, 0) != RES_OK)
            res = FR_DISK_ERR;
    }

    return res;
}
#endif

/*-----------------------------------------------------------------------*/
/* Get sector# from cluster#                                             */
/*-----------------------------------------------------------------------*/
/* Hidden API for hacks and disk tools */
DWORD clust2sect (  /* !=0:Sector number, 0:Failed (invalid cluster#) */
    FATFS* fs,      /* File system object */
    DWORD clst      /* Cluster# to be converted */
)
{
    clst -= 2;
    if (clst >= fs->n_fatent - 2) return 0;     /* Invalid cluster# */
    return clst * fs->csize + fs->database;
}

/*-----------------------------------------------------------------------*/
/* FAT access - Read value of a FAT entry                                */
/*-----------------------------------------------------------------------*/
/* Hidden API for hacks and disk tools */
DWORD get_fat ( /* 0xFFFFFFFF:Disk error, 1:Internal error, 2..0x0FFFFFFF:Cluster status */
    FATFS* fs,  /* File system object */
    DWORD clst  /* FAT index number (cluster number) to get the value */
)
{
    UINT wc, bc;
    UCHAR *p;
    DWORD val;

    if (clst < 2 || clst >= fs->n_fatent) { /* Check if in valid range */
        val = 1;    /* Internal error */

    } else {
        val = 0xFFFFFFFF;   /* Default value falls on disk error */

        switch (fs->fs_type) {
        case FS_FAT12 :
            bc = (UINT)clst; bc += bc / 2;
            if (move_window(fs, fs->fatbase + (bc / SS(fs))) != FR_OK) break;
            wc = fs->win[bc++ % SS(fs)];
            if (move_window(fs, fs->fatbase + (bc / SS(fs))) != FR_OK) break;
            wc |= fs->win[bc % SS(fs)] << 8;
            val = clst & 1 ? wc >> 4 : (wc & 0xFFF);
            break;

        case FS_FAT16 :
            if (move_window(fs, fs->fatbase + (clst / (SS(fs) / 2))) != FR_OK) break;
            p = &fs->win[clst * 2 % SS(fs)];
            val = LD_WORD(p);
            break;

        case FS_FAT32 :
            if (move_window(fs, fs->fatbase + (clst / (SS(fs) / 4))) != FR_OK) break;
            p = &fs->win[clst * 4 % SS(fs)];
            val = LD_DWORD(p) & 0x0FFFFFFF;
            break;

        default:
            val = 1;    /* Internal error */
        }
    }
    return val;
}

/*-----------------------------------------------------------------------*/
/* FAT access - Change value of a FAT entry                              */
/*-----------------------------------------------------------------------*/
/* Hidden API for hacks and disk tools */

#if !_FS_READONLY
FRESULT put_fat (   /* FR_OK(0):succeeded, !=0:error */
    FATFS* fs,      /* File system object */
    DWORD clst,     /* FAT index number (cluster number) to be changed */
    DWORD val       /* New value to be set to the entry */
)
{
    UINT bc;
    UCHAR *p;
    FRESULT res;

    if (clst < 2 || clst >= fs->n_fatent) { /* Check if in valid range */
        res = FR_INT_ERR;

    } else {
        switch (fs->fs_type) {
        case FS_FAT12 :
            bc = (UINT)clst; bc += bc / 2;
            res = move_window(fs, fs->fatbase + (bc / SS(fs)));
            if (res != FR_OK) break;
            p = &fs->win[bc++ % SS(fs)];
            *p = (clst & 1) ? ((*p & 0x0F) | ((UCHAR)val << 4)) : (UCHAR)val;
            fs->wflag = 1;
            res = move_window(fs, fs->fatbase + (bc / SS(fs)));
            if (res != FR_OK) break;
            p = &fs->win[bc % SS(fs)];
            *p = (clst & 1) ? (UCHAR)(val >> 4) : ((*p & 0xF0) | ((UCHAR)(val >> 8) & 0x0F));
            fs->wflag = 1;
            break;

        case FS_FAT16 :
            res = move_window(fs, fs->fatbase + (clst / (SS(fs) / 2)));
            if (res != FR_OK) break;
            p = &fs->win[clst * 2 % SS(fs)];
            ST_WORD(p, (WORD)val);
            fs->wflag = 1;
            break;

        case FS_FAT32 :
            res = move_window(fs, fs->fatbase + (clst / (SS(fs) / 4)));
            if (res != FR_OK) break;
            p = &fs->win[clst * 4 % SS(fs)];
            val |= LD_DWORD(p) & 0xF0000000;
            ST_DWORD(p, val);
            fs->wflag = 1;
            break;

        default :
            res = FR_INT_ERR;
        }
    }

    return res;
}
#endif /* !_FS_READONLY */

/*-----------------------------------------------------------------------*/
/* FAT handling - Remove a cluster chain                                 */
/*-----------------------------------------------------------------------*/
#if !_FS_READONLY
static
FRESULT remove_chain (  /* FR_OK(0):succeeded, !=0:error */
    FATFS* fs,          /* File system object */
    DWORD clst          /* Cluster# to remove a chain from */
)
{
    FRESULT res;
    DWORD nxt;
#if _USE_TRIM
    DWORD scl = clst, ecl = clst, rt[2];
#endif

    if (clst < 2 || clst >= fs->n_fatent) { /* Check if in valid range */
        res = FR_INT_ERR;

    } else {
        res = FR_OK;
        while (clst < fs->n_fatent) {           /* Not a last link? */
            nxt = get_fat(fs, clst);            /* Get cluster status */
            if (nxt == 0) break;                /* Empty cluster? */
            if (nxt == 1) { res = FR_INT_ERR; break; }  /* Internal error? */
            if (nxt == 0xFFFFFFFF) { res = FR_DISK_ERR; break; }    /* Disk error? */
            res = put_fat(fs, clst, 0);         /* Mark the cluster "empty" */
            if (res != FR_OK) break;
            if (fs->free_clust != 0xFFFFFFFF) { /* Update FSINFO */
                fs->free_clust++;
                fs->fsi_flag |= 1;
            }
#if _USE_TRIM
            if (ecl + 1 == nxt) {   /* Is next cluster contiguous? */
                ecl = nxt;
            } else {                /* End of contiguous clusters */
                rt[0] = clust2sect(fs, scl);                    /* Start sector */
                rt[1] = clust2sect(fs, ecl) + fs->csize - 1;    /* End sector */
                disk_ioctl(fs->drv, CTRL_TRIM, rt);             /* Erase the block */
                scl = ecl = nxt;
            }
#endif
            clst = nxt; /* Next cluster */
        }
    }

    return res;
}
#endif

/*-----------------------------------------------------------------------*/
/* FAT handling - Stretch or Create a cluster chain                      */
/*-----------------------------------------------------------------------*/
#if !_FS_READONLY
static
DWORD create_chain (    /* 0:No free cluster, 1:Internal error, 0xFFFFFFFF:Disk error, >=2:New cluster# */
    FATFS* fs,          /* File system object */
    DWORD clst          /* Cluster# to stretch, 0:Create a new chain */
)
{
    DWORD cs, ncl, scl;
    FRESULT res;

    if (clst == 0) {        /* Create a new chain */
        scl = fs->last_clust;           /* Get suggested start point */
        if (!scl || scl >= fs->n_fatent) scl = 1;
    }
    else {                  /* Stretch the current chain */
        cs = get_fat(fs, clst);         /* Check the cluster status */
        if (cs < 2) return 1;           /* Invalid value */
        if (cs == 0xFFFFFFFF) return cs;    /* A disk error occurred */
        if (cs < fs->n_fatent) return cs;   /* It is already followed by next cluster */
        scl = clst;
    }

    ncl = scl;              /* Start cluster */
    for (;;) {
        ncl++;                          /* Next cluster */
        if (ncl >= fs->n_fatent) {      /* Check wrap around */
            ncl = 2;
            if (ncl > scl) return 0;    /* No free cluster */
        }
        cs = get_fat(fs, ncl);          /* Get the cluster status */
        if (cs == 0) break;             /* Found a free cluster */
        if (cs == 0xFFFFFFFF || cs == 1)/* An error occurred */
            return cs;
        if (ncl == scl) return 0;       /* No free cluster */
    }

    res = put_fat(fs, ncl, 0x0FFFFFFF); /* Mark the new cluster "last link" */
    if (res == FR_OK && clst != 0) {
        res = put_fat(fs, clst, ncl);   /* Link it to the previous one if needed */
    }
    if (res == FR_OK) {
        fs->last_clust = ncl;           /* Update FSINFO */
        if (fs->free_clust != 0xFFFFFFFF) {
            fs->free_clust--;
            fs->fsi_flag |= 1;
        }
    } else {
        ncl = (res == FR_DISK_ERR) ? 0xFFFFFFFF : 1;
    }

    return ncl;     /* Return new cluster number or error code */
}
#endif /* !_FS_READONLY */

/*-----------------------------------------------------------------------*/
/* FAT handling - Convert offset into cluster with link map table        */
/*-----------------------------------------------------------------------*/
#if _USE_FASTSEEK
static
DWORD clmt_clust (  /* <2:Error, >=2:Cluster number */
    FIL* fp,        /* Pointer to the file object */
    DWORD ofs       /* File offset to be converted to cluster# */
)
{
    DWORD cl, ncl, *tbl;


    tbl = fp->cltbl + 1;    /* Top of CLMT */
    cl = ofs / SS(fp->fs) / fp->fs->csize;  /* Cluster order from top of the file */
    for (;;) {
        ncl = *tbl++;           /* Number of cluters in the fragment */
        if (!ncl) return 0;     /* End of table? (error) */
        if (cl < ncl) break;    /* In this fragment? */
        cl -= ncl; tbl++;       /* Next fragment */
    }
    return cl + *tbl;   /* Return the cluster number */
}
#endif  /* _USE_FASTSEEK */

/*-----------------------------------------------------------------------*/
/* Directory handling - Set directory index                              */
/*-----------------------------------------------------------------------*/
static
FRESULT dir_sdi (   /* FR_OK(0):succeeded, !=0:error */
    DIR* dp,        /* Pointer to directory object */
    UINT idx        /* Index of directory table */
)
{
    DWORD clst, sect;
    UINT ic;

    dp->index = (WORD)idx;  /* Current index */
    clst = dp->sclust;      /* Table start cluster (0:root) */
    if (clst == 1 || clst >= dp->fs->n_fatent)  /* Check start cluster range */
        return FR_INT_ERR;
    if (!clst && dp->fs->fs_type == FS_FAT32)   /* Replace cluster# 0 with root cluster# if in FAT32 */
        clst = dp->fs->dirbase;

    if (clst == 0) {    /* Static table (root-directory in FAT12/16) */
        if (idx >= dp->fs->n_rootdir)   /* Is index out of range? */
            return FR_INT_ERR;
        sect = dp->fs->dirbase;
    }
    else {              /* Dynamic table (root-directory in FAT32 or sub-directory) */
        ic = SS(dp->fs) / SZ_DIRE * dp->fs->csize;  /* Entries per cluster */
        while (idx >= ic) { /* Follow cluster chain */
            clst = get_fat(dp->fs, clst);               /* Get next cluster */
            if (clst == 0xFFFFFFFF) return FR_DISK_ERR; /* Disk error */
            if (clst < 2 || clst >= dp->fs->n_fatent)   /* Reached to end of table or internal error */
                return FR_INT_ERR;
            idx -= ic;
        }
        sect = clust2sect(dp->fs, clst);
    }
    dp->clust = clst;   /* Current cluster# */
    if (!sect) return FR_INT_ERR;
    dp->sect = sect + idx / (SS(dp->fs) / SZ_DIRE);                 /* Sector# of the directory entry */
    dp->dir = dp->fs->win + (idx % (SS(dp->fs) / SZ_DIRE)) * SZ_DIRE;   /* Ptr to the entry in the sector */

    return FR_OK;
}

/*-----------------------------------------------------------------------*/
/* Directory handling - Move directory table index next                  */
/*-----------------------------------------------------------------------*/
static
FRESULT dir_next (  /* FR_OK(0):succeeded, FR_NO_FILE:End of table, FR_DENIED:Could not stretch */
    DIR* dp,        /* Pointer to the directory object */
    int stretch     /* 0: Do not stretch table, 1: Stretch table if needed */
)
{
    DWORD clst;
    UINT i;
#if !_FS_READONLY
    UINT c;
#endif

    i = dp->index + 1;
    if (!(i & 0xFFFF) || !dp->sect) /* Report EOT when index has reached 65535 */
        return FR_NO_FILE;

    if (!(i % (SS(dp->fs) / SZ_DIRE))) {    /* Sector changed? */
        dp->sect++;                 /* Next sector */

        if (!dp->clust) {       /* Static table */
            if (i >= dp->fs->n_rootdir) /* Report EOT if it reached end of static table */
                return FR_NO_FILE;
        }
        else {                  /* Dynamic table */
            if (((i / (SS(dp->fs) / SZ_DIRE)) & (dp->fs->csize - 1)) == 0) {    /* Cluster changed? */
                clst = get_fat(dp->fs, dp->clust);              /* Get next cluster */
                if (clst <= 1) return FR_INT_ERR;
                if (clst == 0xFFFFFFFF) return FR_DISK_ERR;
                if (clst >= dp->fs->n_fatent) {                 /* If it reached end of dynamic table, */
#if !_FS_READONLY
                    if (!stretch) return FR_NO_FILE;            /* If do not stretch, report EOT */
                    clst = create_chain(dp->fs, dp->clust);     /* Stretch cluster chain */
                    if (clst == 0) return FR_DENIED;            /* No free cluster */
                    if (clst == 1) return FR_INT_ERR;
                    if (clst == 0xFFFFFFFF) return FR_DISK_ERR;
                    /* Clean-up stretched table */
                    if (sync_window(dp->fs)) return FR_DISK_ERR;/* Flush disk access window */
                    mem_set(dp->fs->win, 0, SS(dp->fs));        /* Clear window buffer */
                    dp->fs->winsect = clust2sect(dp->fs, clst); /* Cluster start sector */
                    for (c = 0; c < dp->fs->csize; c++) {       /* Fill the new cluster with 0 */
                        dp->fs->wflag = 1;
                        if (sync_window(dp->fs)) return FR_DISK_ERR;
                        dp->fs->winsect++;
                    }
                    dp->fs->winsect -= c;                       /* Rewind window offset */
#else
                    if (!stretch) return FR_NO_FILE;            /* If do not stretch, report EOT (this is to suppress warning) */
                    return FR_NO_FILE;                          /* Report EOT */
#endif
                }
                dp->clust = clst;               /* Initialize data for new cluster */
                dp->sect = clust2sect(dp->fs, clst);
            }
        }
    }

    dp->index = (WORD)i;    /* Current index */
    dp->dir = dp->fs->win + (i % (SS(dp->fs) / SZ_DIRE)) * SZ_DIRE; /* Current entry in the window */

    return FR_OK;
}

/*-----------------------------------------------------------------------*/
/* Directory handling - Reserve directory entry                          */
/*-----------------------------------------------------------------------*/
#if !_FS_READONLY
static
FRESULT dir_alloc ( /* FR_OK(0):succeeded, !=0:error */
    DIR* dp,        /* Pointer to the directory object */
    UINT nent       /* Number of contiguous entries to allocate (1-21) */
)
{
    FRESULT res;
    UINT n;


    res = dir_sdi(dp, 0);
    if (res == FR_OK) {
        n = 0;
        do {
            res = move_window(dp->fs, dp->sect);
            if (res != FR_OK) break;
            if (dp->dir[0] == DDEM || dp->dir[0] == 0) {    /* Is it a free entry? */
                if (++n == nent) break; /* A block of contiguous free entries is found */
            } else {
                n = 0;                  /* Not a blank entry. Restart to search */
            }
            res = dir_next(dp, 1);      /* Next entry with table stretch enabled */
        } while (res == FR_OK);
    }
    if (res == FR_NO_FILE) res = FR_DENIED; /* No directory entry to allocate */
    return res;
}
#endif

/*-----------------------------------------------------------------------*/
/* Directory handling - Load/Store start cluster number                  */
/*-----------------------------------------------------------------------*/
static
DWORD ld_clust (    /* Returns the top cluster value of the SFN entry */
    FATFS* fs,      /* Pointer to the fs object */
    const UCHAR* dir /* Pointer to the SFN entry */
)
{
    DWORD cl;

    cl = LD_WORD(dir + DIR_FstClusLO);
    if (fs->fs_type == FS_FAT32)
        cl |= (DWORD)LD_WORD(dir + DIR_FstClusHI) << 16;

    return cl;
}

#if !_FS_READONLY
static
void st_clust (
    UCHAR* dir,  /* Pointer to the SFN entry */
    DWORD cl    /* Value to be set */
)
{
    ST_WORD(dir + DIR_FstClusLO, cl);
    ST_WORD(dir + DIR_FstClusHI, cl >> 16);
}
#endif

/*-----------------------------------------------------------------------*/
/* LFN handling - Test/Pick/Fit an LFN segment from/to directory entry   */
/*-----------------------------------------------------------------------*/
#if _USE_LFN
static
const UCHAR LfnOfs[] = {1,3,5,7,9,14,16,18,20,22,24,28,30};  /* Offset of LFN characters in the directory entry */

static
int cmp_lfn (           /* 1:matched, 0:not matched */
    WCHAR* lfnbuf,      /* Pointer to the LFN working buffer to be compared */
    UCHAR* dir           /* Pointer to the directory entry containing the part of LFN */
)
{
    UINT i, s;
    WCHAR wc, uc;

    if (LD_WORD(dir + LDIR_FstClusLO) != 0) return 0;   /* Check LDIR_FstClusLO */

    i = ((dir[LDIR_Ord] & 0x3F) - 1) * 13;  /* Offset in the LFN buffer */

    for (wc = 1, s = 0; s < 13; s++) {      /* Process all characters in the entry */
        uc = LD_WORD(dir + LfnOfs[s]);      /* Pick an LFN character */
        if (wc) {
            if (i >= _MAX_LFN || ff_wtoupper(uc) != ff_wtoupper(lfnbuf[i++]))   /* Compare it */
                return 0;                   /* Not matched */
            wc = uc;
        } else {
            if (uc != 0xFFFF) return 0;     /* Check filler */
        }
    }

    if ((dir[LDIR_Ord] & LLEF) && wc && lfnbuf[i])  /* Last segment matched but different length */
        return 0;

    return 1;       /* The part of LFN matched */
}

static
int pick_lfn (          /* 1:succeeded, 0:buffer overflow or invalid LFN entry */
    WCHAR* lfnbuf,      /* Pointer to the LFN working buffer */
    UCHAR* dir           /* Pointer to the LFN entry */
)
{
    UINT i, s;
    WCHAR wc, uc;

    if (LD_WORD(dir + LDIR_FstClusLO) != 0) return 0;   /* Check LDIR_FstClusLO */

    i = ((dir[LDIR_Ord] & 0x3F) - 1) * 13;  /* Offset in the LFN buffer */

    for (wc = 1, s = 0; s < 13; s++) {      /* Process all characters in the entry */
        uc = LD_WORD(dir + LfnOfs[s]);      /* Pick an LFN character */
        if (wc) {
            if (i >= _MAX_LFN) return 0;    /* Buffer overflow? */
            lfnbuf[i++] = wc = uc;          /* Store it */
        } else {
            if (uc != 0xFFFF) return 0;     /* Check filler */
        }
    }

    if (dir[LDIR_Ord] & LLEF) {             /* Put terminator if it is the last LFN part */
        if (i >= _MAX_LFN) return 0;        /* Buffer overflow? */
        lfnbuf[i] = 0;
    }

    return 1;       /* The part of LFN is valid */
}

#if !_FS_READONLY
static
void fit_lfn (
    const WCHAR* lfnbuf,    /* Pointer to the LFN working buffer */
    UCHAR* dir,              /* Pointer to the LFN entry to be processed */
    UCHAR ord,               /* LFN order (1-20) */
    UCHAR sum                /* Checksum of the corresponding SFN */
)
{
    UINT i, s;
    WCHAR wc;

    dir[LDIR_Chksum] = sum;         /* Set checksum */
    dir[LDIR_Attr] = AM_LFN;        /* Set attribute. LFN entry */
    dir[LDIR_Type] = 0;
    ST_WORD(dir + LDIR_FstClusLO, 0);

    i = (ord - 1) * 13;             /* Get offset in the LFN working buffer */
    s = wc = 0;
    do {
        if (wc != 0xFFFF) wc = lfnbuf[i++]; /* Get an effective character */
        ST_WORD(dir+LfnOfs[s], wc); /* Put it */
        if (!wc) wc = 0xFFFF;       /* Padding characters following last character */
    } while (++s < 13);
    if (wc == 0xFFFF || !lfnbuf[i]) ord |= LLEF;    /* Bottom LFN part is the start of LFN sequence */
    dir[LDIR_Ord] = ord;            /* Set the LFN order */
}

#endif
#endif

/*-----------------------------------------------------------------------*/
/* Create numbered name                                                  */
/*-----------------------------------------------------------------------*/
#if _USE_LFN
static
void gen_numname (
    UCHAR* dst,          /* Pointer to the buffer to store numbered SFN */
    const UCHAR* src,    /* Pointer to SFN */
    const WCHAR* lfn,   /* Pointer to LFN */
    UINT seq            /* Sequence number */
)
{
    UCHAR ns[8], c;
    UINT i, j;
    WCHAR wc;
    DWORD sr;

    mem_cpy(dst, src, 11);

    if (seq > 5) {  /* On many collisions, generate a hash number instead of sequential number */
        sr = seq;
        while (*lfn) {  /* Create a CRC */
            wc = *lfn++;
            for (i = 0; i < 16; i++) {
                sr = (sr << 1) + (wc & 1);
                wc >>= 1;
                if (sr & 0x10000) sr ^= 0x11021;
            }
        }
        seq = (UINT)sr;
    }

    /* itoa (hexdecimal) */
    i = 7;
    do {
        c = (seq % 16) + '0';
        if (c > '9') c += 7;
        ns[i--] = c;
        seq /= 16;
    } while (seq);
    ns[i] = '~';

    /* Append the number */
    for (j = 0; j < i && dst[j] != ' '; j++) {
        if (IsDBCS1(dst[j])) {
            if (j == i - 1) break;
            j++;
        }
    }
    do {
        dst[j++] = (i < 8) ? ns[i++] : ' ';
    } while (j < 8);
}
#endif

/*-----------------------------------------------------------------------*/
/* Calculate checksum of an SFN entry                                    */
/*-----------------------------------------------------------------------*/
#if _USE_LFN
static
UCHAR sum_sfn (
    const UCHAR* dir     /* Pointer to the SFN entry */
)
{
    UCHAR sum = 0;
    UINT n = 11;

    do sum = (sum >> 1) + (sum << 7) + *dir++; while (--n);
    return sum;
}
#endif

/*-----------------------------------------------------------------------*/
/* Directory handling - Find an object in the directory                  */
/*-----------------------------------------------------------------------*/
static
FRESULT dir_find (  /* FR_OK(0):succeeded, !=0:error */
    DIR* dp         /* Pointer to the directory object linked to the file name */
)
{
    FRESULT res;
    UCHAR c, *dir;
#if _USE_LFN
    UCHAR a, ord, sum;
#endif

    res = dir_sdi(dp, 0);           /* Rewind directory object */
    if (res != FR_OK) return res;

#if _USE_LFN
    ord = sum = 0xFF; dp->lfn_idx = 0xFFFF; /* Reset LFN sequence */
#endif
    do {
        res = move_window(dp->fs, dp->sect);
        if (res != FR_OK) break;
        dir = dp->dir;                  /* Ptr to the directory entry of current index */
        c = dir[DIR_Name];
        if (c == 0) { res = FR_NO_FILE; break; }    /* Reached to end of table */
#if _USE_LFN    /* LFN configuration */
        a = dir[DIR_Attr] & AM_MASK;
        if (c == DDEM || ((a & AM_VOL) && a != AM_LFN)) {   /* An entry without valid data */
            ord = 0xFF; dp->lfn_idx = 0xFFFF;   /* Reset LFN sequence */
        } else {
            if (a == AM_LFN) {          /* An LFN entry is found */
                if (dp->lfn) {
                    if (c & LLEF) {     /* Is it start of LFN sequence? */
                        sum = dir[LDIR_Chksum];
                        c &= ~LLEF; ord = c;    /* LFN start order */
                        dp->lfn_idx = dp->index;    /* Start index of LFN */
                    }
                    /* Check validity of the LFN entry and compare it with given name */
                    ord = (c == ord && sum == dir[LDIR_Chksum] && cmp_lfn(dp->lfn, dir)) ? ord - 1 : 0xFF;
                }
            } else {                    /* An SFN entry is found */
                if (!ord && sum == sum_sfn(dir)) break; /* LFN matched? */
                if (!(dp->fn[NSFLAG] & NS_LOSS) && !mem_cmp(dir, dp->fn, 11)) break;    /* SFN matched? */
                ord = 0xFF; dp->lfn_idx = 0xFFFF;   /* Reset LFN sequence */
            }
        }
#else       /* Non LFN configuration */
        if (!(dir[DIR_Attr] & AM_VOL) && !mem_cmp(dir, dp->fn, 11)) /* Is it a valid entry? */
            break;
#endif
        res = dir_next(dp, 0);      /* Next entry */
    } while (res == FR_OK);

    return res;
}

/*-----------------------------------------------------------------------*/
/* Read an object from the directory                                     */
/*-----------------------------------------------------------------------*/
#if _FS_MINIMIZE <= 1 || _USE_LABEL || _FS_RPATH >= 2
static
FRESULT dir_read (
    DIR* dp,        /* Pointer to the directory object */
    int vol         /* Filtered by 0:file/directory or 1:volume label */
)
{
    FRESULT res;
    UCHAR a, c, *dir;
#if _USE_LFN
    UCHAR ord = 0xFF, sum = 0xFF;
#endif

    res = FR_NO_FILE;
    while (dp->sect) {
        res = move_window(dp->fs, dp->sect);
        if (res != FR_OK) break;
        dir = dp->dir;                  /* Ptr to the directory entry of current index */
        c = dir[DIR_Name];
        if (c == 0) { res = FR_NO_FILE; break; }    /* Reached to end of table */
        a = dir[DIR_Attr] & AM_MASK;
#if _USE_LFN    /* LFN configuration */
        if (c == DDEM || (!_FS_RPATH && c == '.') || (int)((a & ~AM_ARC) == AM_VOL) != vol) {   /* An entry without valid data */
            ord = 0xFF;
        } else {
            if (a == AM_LFN) {          /* An LFN entry is found */
                if (c & LLEF) {         /* Is it start of LFN sequence? */
                    sum = dir[LDIR_Chksum];
                    c &= ~LLEF; ord = c;
                    dp->lfn_idx = dp->index;
                }
                /* Check LFN validity and capture it */
                ord = (c == ord && sum == dir[LDIR_Chksum] && pick_lfn(dp->lfn, dir)) ? ord - 1 : 0xFF;
            } else {                    /* An SFN entry is found */
                if (ord || sum != sum_sfn(dir)) /* Is there a valid LFN? */
                    dp->lfn_idx = 0xFFFF;       /* It has no LFN. */
                break;
            }
        }
#else       /* Non LFN configuration */
        if (c != DDEM && (_FS_RPATH || c != '.') && a != AM_LFN && (int)((a & ~AM_ARC) == AM_VOL) == vol)   /* Is it a valid entry? */
            break;
#endif
        res = dir_next(dp, 0);              /* Next entry */
        if (res != FR_OK) break;
    }

    if (res != FR_OK) dp->sect = 0;

    return res;
}
#endif  /* _FS_MINIMIZE <= 1 || _USE_LABEL || _FS_RPATH >= 2 */

/*-----------------------------------------------------------------------*/
/* Register an object to the directory                                   */
/*-----------------------------------------------------------------------*/
#if !_FS_READONLY
static
FRESULT dir_register (  /* FR_OK:succeeded, FR_DENIED:no free entry or too many SFN collision, FR_DISK_ERR:disk error */
    DIR* dp             /* Target directory with object name to be created */
)
{
    FRESULT res;
#if _USE_LFN    /* LFN configuration */
    UINT n, nent;
    UCHAR sn[12], *fn, sum;
    WCHAR *lfn;

    fn = dp->fn; lfn = dp->lfn;
    mem_cpy(sn, fn, 12);

    if (_FS_RPATH && (sn[NSFLAG] & NS_DOT))     /* Cannot create dot entry */
        return FR_INVALID_NAME;

    if (sn[NSFLAG] & NS_LOSS) {         /* When LFN is out of 8.3 format, generate a numbered name */
        fn[NSFLAG] = 0; dp->lfn = 0;            /* Find only SFN */
        for (n = 1; n < 100; n++) {
            gen_numname(fn, sn, lfn, n);    /* Generate a numbered name */
            res = dir_find(dp);             /* Check if the name collides with existing SFN */
            if (res != FR_OK) break;
        }
        if (n == 100) return FR_DENIED;     /* Abort if too many collisions */
        if (res != FR_NO_FILE) return res;  /* Abort if the result is other than 'not collided' */
        fn[NSFLAG] = sn[NSFLAG]; dp->lfn = lfn;
    }

    if (sn[NSFLAG] & NS_LFN) {          /* When LFN is to be created, allocate entries for an SFN + LFNs. */
        for (n = 0; lfn[n]; n++) ;
        nent = (n + 25) / 13;
    } else {                        /* Otherwise allocate an entry for an SFN  */
        nent = 1;
    }
    res = dir_alloc(dp, nent);      /* Allocate entries */

    if (res == FR_OK && --nent) {   /* Set LFN entry if needed */
        res = dir_sdi(dp, dp->index - nent);
        if (res == FR_OK) {
            sum = sum_sfn(dp->fn);  /* Checksum value of the SFN tied to the LFN */
            do {                    /* Store LFN entries in bottom first */
                res = move_window(dp->fs, dp->sect);
                if (res != FR_OK) break;
                fit_lfn(dp->lfn, dp->dir, (UCHAR)nent, sum);
                dp->fs->wflag = 1;
                res = dir_next(dp, 0);  /* Next entry */
            } while (res == FR_OK && --nent);
        }
    }
#else   /* Non LFN configuration */
    res = dir_alloc(dp, 1);     /* Allocate an entry for SFN */
#endif

    if (res == FR_OK) {             /* Set SFN entry */
        res = move_window(dp->fs, dp->sect);
        if (res == FR_OK) {
            mem_set(dp->dir, 0, SZ_DIRE);   /* Clean the entry */
            mem_cpy(dp->dir, dp->fn, 11);   /* Put SFN */
#if _USE_LFN
            dp->dir[DIR_NTres] = dp->fn[NSFLAG] & (NS_BODY | NS_EXT);   /* Put NT flag */
#endif
            dp->fs->wflag = 1;
        }
    }

    return res;
}
#endif /* !_FS_READONLY */

/*-----------------------------------------------------------------------*/
/* Remove an object from the directory                                   */
/*-----------------------------------------------------------------------*/
#if !_FS_READONLY && !_FS_MINIMIZE
static
FRESULT dir_remove (    /* FR_OK:Succeeded, FR_DISK_ERR:A disk error */
    DIR* dp             /* Directory object pointing the entry to be removed */
)
{
    FRESULT res;
#if _USE_LFN    /* LFN configuration */
    UINT i;

    i = dp->index;  /* SFN index */
    res = dir_sdi(dp, (dp->lfn_idx == 0xFFFF) ? i : dp->lfn_idx);   /* Goto the SFN or top of the LFN entries */
    if (res == FR_OK) {
        do {
            res = move_window(dp->fs, dp->sect);
            if (res != FR_OK) break;
            mem_set(dp->dir, 0, SZ_DIRE);   /* Clear and mark the entry "deleted" */
            *dp->dir = DDEM;
            dp->fs->wflag = 1;
            if (dp->index >= i) break;  /* When reached SFN, all entries of the object has been deleted. */
            res = dir_next(dp, 0);      /* Next entry */
        } while (res == FR_OK);
        if (res == FR_NO_FILE) res = FR_INT_ERR;
    }

#else           /* Non LFN configuration */
    res = dir_sdi(dp, dp->index);
    if (res == FR_OK) {
        res = move_window(dp->fs, dp->sect);
        if (res == FR_OK) {
            mem_set(dp->dir, 0, SZ_DIRE);   /* Clear and mark the entry "deleted" */
            *dp->dir = DDEM;
            dp->fs->wflag = 1;
        }
    }
#endif

    return res;
}
#endif /* !_FS_READONLY */

/*-----------------------------------------------------------------------*/
/* Get file information from directory entry                             */
/*-----------------------------------------------------------------------*/
#if _FS_MINIMIZE <= 1 || _FS_RPATH >= 2
static
void get_fileinfo (     /* No return code */
    DIR* dp,            /* Pointer to the directory object */
    FILINFO* fno        /* Pointer to the file information to be filled */
)
{
    UINT i;
    TCHAR *p, c;
    UCHAR *dir;
#if _USE_LFN
    WCHAR w, *lfn;
#endif

    p = fno->fname;
    if (dp->sect) {     /* Get SFN */
        dir = dp->dir;
        i = 0;
        while (i < 11) {        /* Copy name body and extension */
            c = (TCHAR)dir[i++];
            if (c == ' ') continue;             /* Skip padding spaces */
            if (c == RDDEM) c = (TCHAR)DDEM;    /* Restore replaced DDEM character */
            if (i == 9) *p++ = '.';             /* Insert a . if extension is exist */
#if _USE_LFN
            if (IsUpper(c) && (dir[DIR_NTres] & (i >= 9 ? NS_EXT : NS_BODY)))
                c += 0x20;          /* To lower */
#if _LFN_UNICODE
            if (IsDBCS1(c) && i != 8 && i != 11 && IsDBCS2(dir[i]))
                c = c << 8 | dir[i++];
            c = ff_convert(c, 1);   /* OEM -> Unicode */
            if (!c) c = '?';
#endif
#endif
            *p++ = c;
        }
        fno->fattrib = dir[DIR_Attr];               /* Attribute */
        fno->fsize = LD_DWORD(dir + DIR_FileSize);  /* Size */
        fno->fdate = LD_WORD(dir + DIR_WrtDate);    /* Date */
        fno->ftime = LD_WORD(dir + DIR_WrtTime);    /* Time */
    }
    *p = 0;     /* Terminate SFN string by a \0 */

#if _USE_LFN
    if (fno->lfname) {
        i = 0; p = fno->lfname;
        if (dp->sect && fno->lfsize && dp->lfn_idx != 0xFFFF) { /* Get LFN if available */
            lfn = dp->lfn;
            while ((w = *lfn++) != 0) {     /* Get an LFN character */
#if !_LFN_UNICODE
                w = ff_convert(w, 0);       /* Unicode -> OEM */
                if (!w) { i = 0; break; }   /* No LFN if it could not be converted */
                if (_DF1S && w >= 0x100)    /* Put 1st byte if it is a DBC (always false on SBCS cfg) */
                    p[i++] = (TCHAR)(w >> 8);
#endif
                if (i >= fno->lfsize - 1) { i = 0; break; } /* No LFN if buffer overflow */
                p[i++] = (TCHAR)w;
            }
        }
        p[i] = 0;   /* Terminate LFN string by a \0 */
    }
#endif
}
#endif /* _FS_MINIMIZE <= 1 || _FS_RPATH >= 2 */

/*-----------------------------------------------------------------------*/
/* Pattern matching                                                      */
/*-----------------------------------------------------------------------*/
#if _USE_FIND && _FS_MINIMIZE <= 1
static
WCHAR get_achar (       /* Get a character and advances ptr 1 or 2 */
    const TCHAR** ptr   /* Pointer to pointer to the SBCS/DBCS/Unicode string */
)
{
    WCHAR chr;

#if !_LFN_UNICODE
    chr = (UCHAR)*(*ptr)++;                  /* Get a byte */
    if (IsLower(chr)) chr -= 0x20;          /* To upper ASCII char */
    if (IsDBCS1(chr) && IsDBCS2(**ptr))     /* Get DBC 2nd byte if needed */
        chr = chr << 8 | (UCHAR)*(*ptr)++;
#ifdef _EXCVT
    if (chr >= 0x80) chr = ExCvt[chr - 0x80];   /* To upper SBCS extended char */
#endif
#else
    chr = ff_wtoupper(*(*ptr)++);           /* Get a word and to upper */
#endif
    return chr;
}

static
int pattern_matching (  /* 0:mismatched, 1:matched */
    const TCHAR* pat,   /* Matching pattern */
    const TCHAR* nam,   /* String to be tested */
    int skip,           /* Number of pre-skip chars (number of ?s) */
    int inf             /* Infinite search (* specified) */
)
{
    const TCHAR *pp, *np;
    WCHAR pc, nc;
    int nm, nx;


    while (skip--) {                /* Pre-skip name chars */
        if (!get_achar(&nam)) return 0; /* Branch mismatched if less name chars */
    }
    if (!*pat && inf) return 1;     /* (short circuit) */

    do {
        pp = pat; np = nam;         /* Top of pattern and name to match */
        for (;;) {
            if (*pp == '?' || *pp == '*') { /* Wildcard? */
                nm = nx = 0;
                do {                /* Analyze the wildcard chars */
                    if (*pp++ == '?') nm++; else nx = 1;
                } while (*pp == '?' || *pp == '*');
                if (pattern_matching(pp, np, nm, nx)) return 1; /* Test new branch (recurs upto number of wildcard blocks in the pattern) */
                nc = *np; break;    /* Branch mismatched */
            }
            pc = get_achar(&pp);    /* Get a pattern char */
            nc = get_achar(&np);    /* Get a name char */
            if (pc != nc) break;    /* Branch mismatched? */
            if (!pc) return 1;      /* Branch matched? (matched at end of both strings) */
        }
        get_achar(&nam);            /* nam++ */
    } while (inf && nc);            /* Retry until end of name if infinite search is specified */

    return 0;
}
#endif /* _USE_FIND && _FS_MINIMIZE <= 1 */

/*-----------------------------------------------------------------------*/
/* Pick a top segment and create the object name in directory form       */
/*-----------------------------------------------------------------------*/
static
FRESULT create_name (   /* FR_OK: successful, FR_INVALID_NAME: could not create */
    DIR* dp,            /* Pointer to the directory object */
    const TCHAR** path  /* Pointer to pointer to the segment in the path string */
)
{
#if _USE_LFN    /* LFN configuration */
    UCHAR b, cf;
    WCHAR w, *lfn;
    UINT i, ni, si, di;
    const TCHAR *p;

    /* Create LFN in Unicode */
    for (p = *path; *p == '/' || *p == '\\'; p++) ; /* Strip duplicated separator */
    lfn = dp->lfn;
    si = di = 0;
    for (;;) {
        w = p[si++];                    /* Get a character */
        if (w < ' ' || w == '/' || w == '\\') break;    /* Break on end of segment */
        if (di >= _MAX_LFN)             /* Reject too long name */
            return FR_INVALID_NAME;
#if !_LFN_UNICODE
        w &= 0xFF;
        if (IsDBCS1(w)) {               /* Check if it is a DBC 1st byte (always false on SBCS cfg) */
            b = (UCHAR)p[si++];          /* Get 2nd byte */
            w = (w << 8) + b;           /* Create a DBC */
            if (!IsDBCS2(b))
                return FR_INVALID_NAME; /* Reject invalid sequence */
        }
        w = ff_convert(w, 1);           /* Convert ANSI/OEM to Unicode */
        if (!w) return FR_INVALID_NAME; /* Reject invalid code */
#endif
        if (w < 0x80 && chk_chr("\"*:<>\?|\x7F", w)) /* Reject illegal characters for LFN */
            return FR_INVALID_NAME;
        lfn[di++] = w;                  /* Store the Unicode character */
    }
    *path = &p[si];                     /* Return pointer to the next segment */
    cf = (w < ' ') ? NS_LAST : 0;       /* Set last segment flag if end of path */
#if _FS_RPATH
    if ((di == 1 && lfn[di - 1] == '.') ||
        (di == 2 && lfn[di - 1] == '.' && lfn[di - 2] == '.')) {    /* Is this segment a dot entry? */
        lfn[di] = 0;
        for (i = 0; i < 11; i++)        /* Create dot name for SFN entry */
            dp->fn[i] = (i < di) ? '.' : ' ';
        dp->fn[i] = cf | NS_DOT;        /* This is a dot entry */
        return FR_OK;
    }
#endif
    while (di) {                        /* Snip off trailing spaces and dots if exist */
        w = lfn[di - 1];
        if (w != ' ' && w != '.') break;
        di--;
    }
    if (!di) return FR_INVALID_NAME;    /* Reject nul string */
    lfn[di] = 0;                        /* LFN is created */

    /* Create SFN in directory form */
    mem_set(dp->fn, ' ', 11);
    for (si = 0; lfn[si] == ' ' || lfn[si] == '.'; si++) ;  /* Strip leading spaces and dots */
    if (si) cf |= NS_LOSS | NS_LFN;
    while (di && lfn[di - 1] != '.') di--;  /* Find extension (di<=si: no extension) */

    b = i = 0; ni = 8;
    for (;;) {
        w = lfn[si++];                  /* Get an LFN character */
        if (!w) break;                  /* Break on end of the LFN */
        if (w == ' ' || (w == '.' && si != di)) {   /* Remove spaces and dots */
            cf |= NS_LOSS | NS_LFN; continue;
        }

        if (i >= ni || si == di) {      /* Extension or end of SFN */
            if (ni == 11) {             /* Long extension */
                cf |= NS_LOSS | NS_LFN; break;
            }
            if (si != di) cf |= NS_LOSS | NS_LFN;   /* Out of 8.3 format */
            if (si > di) break;         /* No extension */
            si = di; i = 8; ni = 11;    /* Enter extension section */
            b <<= 2; continue;
        }

        if (w >= 0x80) {                /* Non ASCII character */
#ifdef _EXCVT
            w = ff_convert(w, 0);       /* Unicode -> OEM code */
            if (w) w = ExCvt[w - 0x80]; /* Convert extended character to upper (SBCS) */
#else
            w = ff_convert(ff_wtoupper(w), 0);  /* Upper converted Unicode -> OEM code */
#endif
            cf |= NS_LFN;               /* Force create LFN entry */
        }

        if (_DF1S && w >= 0x100) {      /* Is this DBC? (always false at SBCS cfg) */
            if (i >= ni - 1) {
                cf |= NS_LOSS | NS_LFN; i = ni; continue;
            }
            dp->fn[i++] = (UCHAR)(w >> 8);
        } else {                        /* SBC */
            if (!w || chk_chr("+,;=[]", w)) {   /* Replace illegal characters for SFN */
                w = '_'; cf |= NS_LOSS | NS_LFN;/* Lossy conversion */
            } else {
                if (IsUpper(w)) {       /* ASCII large capital */
                    b |= 2;
                } else {
                    if (IsLower(w)) {   /* ASCII small capital */
                        b |= 1; w -= 0x20;
                    }
                }
            }
        }
        dp->fn[i++] = (UCHAR)w;
    }

    if (dp->fn[0] == DDEM) dp->fn[0] = RDDEM;   /* If the first character collides with DDEM, replace it with RDDEM */

    if (ni == 8) b <<= 2;
    if ((b & 0x0C) == 0x0C || (b & 0x03) == 0x03)   /* Create LFN entry when there are composite capitals */
        cf |= NS_LFN;
    if (!(cf & NS_LFN)) {                       /* When LFN is in 8.3 format without extended character, NT flags are created */
        if ((b & 0x03) == 0x01) cf |= NS_EXT;   /* NT flag (Extension has only small capital) */
        if ((b & 0x0C) == 0x04) cf |= NS_BODY;  /* NT flag (Filename has only small capital) */
    }

    dp->fn[NSFLAG] = cf;    /* SFN is created */

    return FR_OK;


#else   /* Non-LFN configuration */
    UCHAR b, c, d, *sfn;
    UINT ni, si, i;
    const char *p;

    /* Create file name in directory form */
    for (p = *path; *p == '/' || *p == '\\'; p++) ; /* Skip duplicated separator */
    sfn = dp->fn;
    mem_set(sfn, ' ', 11);
    si = i = b = 0; ni = 8;
#if _FS_RPATH
    if (p[si] == '.') { /* Is this a dot entry? */
        for (;;) {
            c = (UCHAR)p[si++];
            if (c != '.' || si >= 3) break;
            sfn[i++] = c;
        }
        if (c != '/' && c != '\\' && c > ' ') return FR_INVALID_NAME;
        *path = &p[si];                                 /* Return pointer to the next segment */
        sfn[NSFLAG] = (c <= ' ') ? NS_LAST | NS_DOT : NS_DOT;   /* Set last segment flag if end of path */
        return FR_OK;
    }
#endif
    for (;;) {
        c = (UCHAR)p[si++];
        if (c <= ' ' || c == '/' || c == '\\') break;   /* Break on end of segment */
        if (c == '.' || i >= ni) {
            if (ni != 8 || c != '.') return FR_INVALID_NAME;
            i = 8; ni = 11;
            b <<= 2; continue;
        }
        if (c >= 0x80) {                /* Extended character? */
            b |= 3;                     /* Eliminate NT flag */
#ifdef _EXCVT
            c = ExCvt[c - 0x80];        /* To upper extended characters (SBCS cfg) */
#else
#if !_DF1S
            return FR_INVALID_NAME;     /* Reject extended characters (ASCII cfg) */
#endif
#endif
        }
        if (IsDBCS1(c)) {               /* Check if it is a DBC 1st byte (always false at SBCS cfg.) */
            d = (UCHAR)p[si++];          /* Get 2nd byte */
            if (!IsDBCS2(d) || i >= ni - 1) /* Reject invalid DBC */
                return FR_INVALID_NAME;
            sfn[i++] = c;
            sfn[i++] = d;
        } else {                        /* SBC */
            if (chk_chr("\"*+,:;<=>\?[]|\x7F", c))  /* Reject illegal chrs for SFN */
                return FR_INVALID_NAME;
            if (IsUpper(c)) {           /* ASCII large capital? */
                b |= 2;
            } else {
                if (IsLower(c)) {       /* ASCII small capital? */
                    b |= 1; c -= 0x20;
                }
            }
            sfn[i++] = c;
        }
    }
    *path = &p[si];                     /* Return pointer to the next segment */
    c = (c <= ' ') ? NS_LAST : 0;       /* Set last segment flag if end of path */

    if (!i) return FR_INVALID_NAME;     /* Reject nul string */
    if (sfn[0] == DDEM) sfn[0] = RDDEM; /* When first character collides with DDEM, replace it with RDDEM */

    if (ni == 8) b <<= 2;
    if ((b & 0x03) == 0x01) c |= NS_EXT;    /* NT flag (Name extension has only small capital) */
    if ((b & 0x0C) == 0x04) c |= NS_BODY;   /* NT flag (Name body has only small capital) */

    sfn[NSFLAG] = c;        /* Store NT flag, File name is created */

    return FR_OK;
#endif
}

/*-----------------------------------------------------------------------*/
/* Follow a file path                                                    */
/*-----------------------------------------------------------------------*/
static
FRESULT follow_path (   /* FR_OK(0): successful, !=0: error code */
    DIR* dp,            /* Directory object to return last directory and found object */
    const TCHAR* path   /* Full-path string to find a file or directory */
)
{
    FRESULT res;
    UCHAR *dir, ns;


#if _FS_RPATH
    if (*path == '/' || *path == '\\') {    /* There is a heading separator */
        path++; dp->sclust = 0;             /* Strip it and start from the root directory */
    } else {                                /* No heading separator */
        dp->sclust = dp->fs->cdir;          /* Start from the current directory */
    }
#else
    if (*path == '/' || *path == '\\')      /* Strip heading separator if exist */
        path++;
    dp->sclust = 0;                         /* Always start from the root directory */
#endif

    if ((UINT)*path < ' ') {                /* Null path name is the origin directory itself */
        res = dir_sdi(dp, 0);
        dp->dir = 0;
    } else {                                /* Follow path */
        for (;;) {
            res = create_name(dp, &path);   /* Get a segment name of the path */
            if (res != FR_OK) break;
            res = dir_find(dp);             /* Find an object with the sagment name */
            ns = dp->fn[NSFLAG];
            if (res != FR_OK) {             /* Failed to find the object */
                if (res == FR_NO_FILE) {    /* Object is not found */
                    if (_FS_RPATH && (ns & NS_DOT)) {   /* If dot entry is not exist, */
                        dp->sclust = 0; dp->dir = 0;    /* it is the root directory and stay there */
                        if (!(ns & NS_LAST)) continue;  /* Continue to follow if not last segment */
                        res = FR_OK;                    /* Ended at the root directroy. Function completed. */
                    } else {                            /* Could not find the object */
                        if (!(ns & NS_LAST)) res = FR_NO_PATH;  /* Adjust error code if not last segment */
                    }
                }
                break;
            }
            if (ns & NS_LAST) break;            /* Last segment matched. Function completed. */
            dir = dp->dir;                      /* Follow the sub-directory */
            if (!(dir[DIR_Attr] & AM_DIR)) {    /* It is not a sub-directory and cannot follow */
                res = FR_NO_PATH; break;
            }
            dp->sclust = ld_clust(dp->fs, dir);
        }
    }

    return res;
}

/*-----------------------------------------------------------------------*/
/* Get logical drive number from path name                               */
/*-----------------------------------------------------------------------*/
static
int get_ldnumber (      /* Returns logical drive number (-1:invalid drive) */
    const TCHAR** path  /* Pointer to pointer to the path name */
)
{
    const TCHAR *tp, *tt;
    UINT i;
    int vol = -1;
#if _STR_VOLUME_ID      /* Find string drive id */
    static const char* const str[] = {_VOLUME_STRS};
    const char *sp;
    char c;
    TCHAR tc;
#endif

    if (*path) {    /* If the pointer is not a null */
        for (tt = *path; (UINT)*tt >= (_USE_LFN ? ' ' : '!') && *tt != ':'; tt++) ; /* Find ':' in the path */
        if (*tt == ':') {   /* If a ':' is exist in the path name */
            tp = *path;
            i = *tp++ - '0';
            if (i < 10 && tp == tt) {   /* Is there a numeric drive id? */
                if (i < _VOLUMES) { /* If a drive id is found, get the value and strip it */
                    vol = (int)i;
                    *path = ++tt;
                }
            }
#if _STR_VOLUME_ID
             else { /* No numeric drive number, find string drive id */
                i = 0; tt++;
                do {
                    sp = str[i]; tp = *path;
                    do {    /* Compare a string drive id with path name */
                        c = *sp++; tc = *tp++;
                        if (IsLower(tc)) tc -= 0x20;
                    } while (c && (TCHAR)c == tc);
                } while ((c || tp != tt) && ++i < _VOLUMES);    /* Repeat for each id until pattern match */
                if (i < _VOLUMES) { /* If a drive id is found, get the value and strip it */
                    vol = (int)i;
                    *path = tt;
                }
            }
#endif
            return vol;
        }
#if _FS_RPATH && _VOLUMES >= 2
        vol = CurrVol;  /* Current drive */
#else
        vol = 0;        /* Drive 0 */
#endif
    }
    return vol;
}

/*-----------------------------------------------------------------------*/
/* Load a sector and check if it is an FAT boot sector                   */
/*-----------------------------------------------------------------------*/
static
UCHAR check_fs ( /* 0:Valid FAT-BS, 1:Valid BS but not FAT, 2:Not a BS, 3:Disk error */
    FATFS* fs,  /* File system object */
    DWORD sect  /* Sector# (lba) to check if it is an FAT boot record or not */
)
{
    fs->wflag = 0; fs->winsect = 0xFFFFFFFF;    /* Invaidate window */
    if (move_window(fs, sect) != FR_OK)         /* Load boot record */
        return 3;

    if (LD_WORD(&fs->win[BS_55AA]) != 0xAA55)   /* Check boot record signature (always placed at offset 510 even if the sector size is >512) */
        return 2;

    if ((LD_DWORD(&fs->win[BS_FilSysType]) & 0xFFFFFF) == 0x544146)     /* Check "FAT" string */
        return 0;
    if ((LD_DWORD(&fs->win[BS_FilSysType32]) & 0xFFFFFF) == 0x544146)   /* Check "FAT" string */
        return 0;

    return 1;
}

/*-----------------------------------------------------------------------*/
/* Find logical drive and check if the volume is mounted                 */
/*-----------------------------------------------------------------------*/
static
FRESULT find_volume (   /* FR_OK(0): successful, !=0: any error occurred */
    FATFS** rfs,        /* Pointer to pointer to the found file system object */
    const TCHAR** path, /* Pointer to pointer to the path name (drive number) */
    UCHAR wmode          /* !=0: Check write protection for write access */
)
{
    UCHAR fmt, *pt;
    int vol;
    DSTATUS stat;
    DWORD bsect, fasize, tsect, sysect, nclst, szbfat, br[4];
    WORD nrsv;
    FATFS *fs;
    UINT i;

    /* Get logical drive number from the path name */
    *rfs = 0;
    vol = get_ldnumber(path);
    if (vol < 0) return FR_INVALID_DRIVE;

    /* Check if the file system object is valid or not */
    fs = FatFs[vol];                    /* Get pointer to the file system object */
    if (!fs) return FR_NOT_ENABLED;     /* Is the file system object available? */

    ENTER_FF(fs);                       /* Lock the volume */
    *rfs = fs;                          /* Return pointer to the file system object */

    if (fs->fs_type) {                  /* If the volume has been mounted */
        stat = disk_status(fs->drv);
        if (!(stat & STA_NOINIT)) {     /* and the physical drive is kept initialized */
            if (!_FS_READONLY && wmode && (stat & STA_PROTECT)) /* Check write protection if needed */
                return FR_WRITE_PROTECTED;
            return FR_OK;               /* The file system object is valid */
        }
    }

    /* The file system object is not valid. */
    /* Following code attempts to mount the volume. (analyze BPB and initialize the fs object) */

    fs->fs_type = 0;                    /* Clear the file system object */
    fs->drv = LD2PD(vol);               /* Bind the logical drive and a physical drive */
    stat = disk_initialize(fs->drv);    /* Initialize the physical drive */
    if (stat & STA_NOINIT)              /* Check if the initialization succeeded */
        return FR_NOT_READY;            /* Failed to initialize due to no medium or hard error */
    if (!_FS_READONLY && wmode && (stat & STA_PROTECT)) /* Check disk write protection if needed */
        return FR_WRITE_PROTECTED;
#if _MAX_SS != _MIN_SS                      /* Get sector size (multiple sector size cfg only) */
    if (disk_ioctl(fs->drv, GET_SECTOR_SIZE, &SS(fs)) != RES_OK
        || SS(fs) < _MIN_SS || SS(fs) > _MAX_SS) return FR_DISK_ERR;
#endif
    /* Find an FAT partition on the drive. Supports only generic partitioning, FDISK and SFD. */
    bsect = 0;
    fmt = check_fs(fs, bsect);                  /* Load sector 0 and check if it is an FAT boot sector as SFD */
    if (fmt == 1 || (!fmt && (LD2PT(vol)))) {   /* Not an FAT boot sector or forced partition number */
        for (i = 0; i < 4; i++) {           /* Get partition offset */
            pt = fs->win + MBR_Table + i * SZ_PTE;
            br[i] = pt[4] ? LD_DWORD(&pt[8]) : 0;
        }
        i = LD2PT(vol);                     /* Partition number: 0:auto, 1-4:forced */
        if (i) i--;
        do {                                /* Find an FAT volume */
            bsect = br[i];
            fmt = bsect ? check_fs(fs, bsect) : 2;  /* Check the partition */
        } while (!LD2PT(vol) && fmt && ++i < 4);
    }
    if (fmt == 3) return FR_DISK_ERR;       /* An error occured in the disk I/O layer */
    if (fmt) return FR_NO_FILESYSTEM;       /* No FAT volume is found */

    /* An FAT volume is found. Following code initializes the file system object */

    if (LD_WORD(fs->win + BPB_BytsPerSec) != SS(fs))    /* (BPB_BytsPerSec must be equal to the physical sector size) */
        return FR_NO_FILESYSTEM;

    fasize = LD_WORD(fs->win + BPB_FATSz16);            /* Number of sectors per FAT */
    if (!fasize) fasize = LD_DWORD(fs->win + BPB_FATSz32);
    fs->fsize = fasize;

    fs->n_fats = fs->win[BPB_NumFATs];                  /* Number of FAT copies */
    if (fs->n_fats != 1 && fs->n_fats != 2)             /* (Must be 1 or 2) */
        return FR_NO_FILESYSTEM;
    fasize *= fs->n_fats;                               /* Number of sectors for FAT area */

    fs->csize = fs->win[BPB_SecPerClus];                /* Number of sectors per cluster */
    if (!fs->csize || (fs->csize & (fs->csize - 1)))    /* (Must be power of 2) */
        return FR_NO_FILESYSTEM;

    fs->n_rootdir = LD_WORD(fs->win + BPB_RootEntCnt);  /* Number of root directory entries */
    if (fs->n_rootdir % (SS(fs) / SZ_DIRE))             /* (Must be sector aligned) */
        return FR_NO_FILESYSTEM;

    tsect = LD_WORD(fs->win + BPB_TotSec16);            /* Number of sectors on the volume */
    if (!tsect) tsect = LD_DWORD(fs->win + BPB_TotSec32);

    nrsv = LD_WORD(fs->win + BPB_RsvdSecCnt);           /* Number of reserved sectors */
    if (!nrsv) return FR_NO_FILESYSTEM;                 /* (Must not be 0) */

    /* Determine the FAT sub type */
    sysect = nrsv + fasize + fs->n_rootdir / (SS(fs) / SZ_DIRE);    /* RSV + FAT + DIR */
    if (tsect < sysect) return FR_NO_FILESYSTEM;        /* (Invalid volume size) */
    nclst = (tsect - sysect) / fs->csize;               /* Number of clusters */
    if (!nclst) return FR_NO_FILESYSTEM;                /* (Invalid volume size) */
    fmt = FS_FAT12;
    if (nclst >= MIN_FAT16) fmt = FS_FAT16;
    if (nclst >= MIN_FAT32) fmt = FS_FAT32;

    /* Boundaries and Limits */
    fs->n_fatent = nclst + 2;                           /* Number of FAT entries */
    fs->volbase = bsect;                                /* Volume start sector */
    fs->fatbase = bsect + nrsv;                         /* FAT start sector */
    fs->database = bsect + sysect;                      /* Data start sector */
    if (fmt == FS_FAT32) {
        if (fs->n_rootdir) return FR_NO_FILESYSTEM;     /* (BPB_RootEntCnt must be 0) */
        fs->dirbase = LD_DWORD(fs->win + BPB_RootClus); /* Root directory start cluster */
        szbfat = fs->n_fatent * 4;                      /* (Needed FAT size) */
    } else {
        if (!fs->n_rootdir) return FR_NO_FILESYSTEM;    /* (BPB_RootEntCnt must not be 0) */
        fs->dirbase = fs->fatbase + fasize;             /* Root directory start sector */
        szbfat = (fmt == FS_FAT16) ?                    /* (Needed FAT size) */
            fs->n_fatent * 2 : fs->n_fatent * 3 / 2 + (fs->n_fatent & 1);
    }
    if (fs->fsize < (szbfat + (SS(fs) - 1)) / SS(fs))   /* (BPB_FATSz must not be less than the size needed) */
        return FR_NO_FILESYSTEM;

#if !_FS_READONLY
    /* Initialize cluster allocation information */
    fs->last_clust = fs->free_clust = 0xFFFFFFFF;

    /* Get fsinfo if available */
    fs->fsi_flag = 0x80;
#if (_FS_NOFSINFO & 3) != 3
    if (fmt == FS_FAT32             /* Enable FSINFO only if FAT32 and BPB_FSInfo == 1 */
        && LD_WORD(fs->win + BPB_FSInfo) == 1
        && move_window(fs, bsect + 1) == FR_OK)
    {
        fs->fsi_flag = 0;
        if (LD_WORD(fs->win + BS_55AA) == 0xAA55    /* Load FSINFO data if available */
            && LD_DWORD(fs->win + FSI_LeadSig) == 0x41615252
            && LD_DWORD(fs->win + FSI_StrucSig) == 0x61417272)
        {
#if (_FS_NOFSINFO & 1) == 0
            fs->free_clust = LD_DWORD(fs->win + FSI_Free_Count);
#endif
#if (_FS_NOFSINFO & 2) == 0
            fs->last_clust = LD_DWORD(fs->win + FSI_Nxt_Free);
#endif
        }
    }
#endif
#endif
    fs->fs_type = fmt;  /* FAT sub-type */
    fs->id = ++Fsid;    /* File system mount ID */
#if _FS_RPATH
    fs->cdir = 0;       /* Set current directory to root */
#endif
#if _FS_LOCK            /* Clear file lock semaphores */
    clear_lock(fs);
#endif

    return FR_OK;
}

/*-----------------------------------------------------------------------*/
/* Check if the file/directory object is valid or not                    */
/*-----------------------------------------------------------------------*/
static
FRESULT validate (  /* FR_OK(0): The object is valid, !=0: Invalid */
    void* obj       /* Pointer to the object FIL/DIR to check validity */
)
{
    FIL *fil = (FIL*)obj;   /* Assuming offset of .fs and .id in the FIL/DIR structure is identical */


    if (!fil || !fil->fs || !fil->fs->fs_type || fil->fs->id != fil->id || (disk_status(fil->fs->drv) & STA_NOINIT))
        return FR_INVALID_OBJECT;

    ENTER_FF(fil->fs);      /* Lock file system */

    return FR_OK;
}

/*--------------------------------------------------------------------------
   Public Functions
---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Mount/Unmount a Logical Drive                                         */
/*-----------------------------------------------------------------------*/
FRESULT f_mount (
    FATFS* fs,          /* Pointer to the file system object (NULL:unmount)*/
    const TCHAR* path,  /* Logical drive number to be mounted/unmounted */
    UCHAR opt            /* 0:Do not mount (delayed mount), 1:Mount immediately */
)
{
    FATFS *cfs;
    int vol;
    FRESULT res;
    const TCHAR *rp = path;

    vol = get_ldnumber(&rp);
    if (vol < 0) return FR_INVALID_DRIVE;
    cfs = FatFs[vol];                   /* Pointer to fs object */

    if (cfs) {
#if _FS_LOCK
        clear_lock(cfs);
#endif
#if _FS_REENTRANT                       /* Discard sync object of the current volume */
        if (!ff_del_syncobj(cfs->sobj)) return FR_INT_ERR;
#endif
        cfs->fs_type = 0;               /* Clear old fs object */
    }

    if (fs) {
        fs->fs_type = 0;                /* Clear new fs object */
#if _FS_REENTRANT                       /* Create sync object for the new volume */
        if (!ff_cre_syncobj((UCHAR)vol, &fs->sobj)) return FR_INT_ERR;
#endif
    }
    FatFs[vol] = fs;                    /* Register new fs object */

    if (!fs || opt != 1) return FR_OK;  /* Do not mount now, it will be mounted later */

    res = find_volume(&fs, &path, 0);   /* Force mounted the volume */
    LEAVE_FF(fs, res);
}

/*-----------------------------------------------------------------------*/
/* Open or Create a File                                                 */
/*-----------------------------------------------------------------------*/
FRESULT f_open (
    FIL* fp,            /* Pointer to the blank file object */
    const TCHAR* path,  /* Pointer to the file name */
    UCHAR mode           /* Access mode and file open mode flags */
)
{
    FRESULT res;
    DIR dj;
    UCHAR *dir;
    DEFINE_NAMEBUF;
#if !_FS_READONLY
    DWORD dw, cl;
#endif

    if (!fp) return FR_INVALID_OBJECT;
    fp->fs = 0;         /* Clear file object */

    /* Get logical drive number */
#if !_FS_READONLY
    mode &= FA_READ | FA_WRITE | FA_CREATE_ALWAYS | FA_OPEN_ALWAYS | FA_CREATE_NEW;
    res = find_volume(&dj.fs, &path, (UCHAR)(mode & ~FA_READ));
#else
    mode &= FA_READ;
    res = find_volume(&dj.fs, &path, 0);
#endif
    if (res == FR_OK) {
        INIT_BUF(dj);
        res = follow_path(&dj, path);   /* Follow the file path */
        dir = dj.dir;
#if !_FS_READONLY   /* R/W configuration */
        if (res == FR_OK) {
            if (!dir)   /* Default directory itself */
                res = FR_INVALID_NAME;
#if _FS_LOCK
            else
                res = chk_lock(&dj, (mode & ~FA_READ) ? 1 : 0);
#endif
        }
        /* Create or Open a file */
        if (mode & (FA_CREATE_ALWAYS | FA_OPEN_ALWAYS | FA_CREATE_NEW)) {
            if (res != FR_OK) {                 /* No file, create new */
                if (res == FR_NO_FILE)          /* There is no file to open, create a new entry */
#if _FS_LOCK
                    res = enq_lock() ? dir_register(&dj) : FR_TOO_MANY_OPEN_FILES;
#else
                    res = dir_register(&dj);
#endif
                mode |= FA_CREATE_ALWAYS;       /* File is created */
                dir = dj.dir;                   /* New entry */
            }
            else {                              /* Any object is already existing */
                if (dir[DIR_Attr] & (AM_RDO | AM_DIR)) {    /* Cannot overwrite it (R/O or DIR) */
                    res = FR_DENIED;
                } else {
                    if (mode & FA_CREATE_NEW)   /* Cannot create as new file */
                        res = FR_EXIST;
                }
            }
            if (res == FR_OK && (mode & FA_CREATE_ALWAYS)) {    /* Truncate it if overwrite mode */
                dw = GET_FATTIME();
                ST_DWORD(dir + DIR_CrtTime, dw);/* Set created time */
                ST_DWORD(dir + DIR_WrtTime, dw);/* Set modified time */
                dir[DIR_Attr] = 0;              /* Reset attribute */
                ST_DWORD(dir + DIR_FileSize, 0);/* Reset file size */
                cl = ld_clust(dj.fs, dir);      /* Get cluster chain */
                st_clust(dir, 0);               /* Reset cluster */
                dj.fs->wflag = 1;
                if (cl) {                       /* Remove the cluster chain if exist */
                    dw = dj.fs->winsect;
                    res = remove_chain(dj.fs, cl);
                    if (res == FR_OK) {
                        dj.fs->last_clust = cl - 1; /* Reuse the cluster hole */
                        res = move_window(dj.fs, dw);
                    }
                }
            }
        }
        else {  /* Open an existing file */
            if (res == FR_OK) {                 /* Following succeeded */
                if (dir[DIR_Attr] & AM_DIR) {   /* It is a directory */
                    res = FR_NO_FILE;
                } else {
                    if ((mode & FA_WRITE) && (dir[DIR_Attr] & AM_RDO)) /* R/O violation */
                        res = FR_DENIED;
                }
            }
        }
        if (res == FR_OK) {
            if (mode & FA_CREATE_ALWAYS)        /* Set file change flag if created or overwritten */
                mode |= FA__WRITTEN;
            fp->dir_sect = dj.fs->winsect;      /* Pointer to the directory entry */
            fp->dir_ptr = dir;
#if _FS_LOCK
            fp->lockid = inc_lock(&dj, (mode & ~FA_READ) ? 1 : 0);
            if (!fp->lockid) res = FR_INT_ERR;
#endif
        }

#else               /* R/O configuration */
        if (res == FR_OK) {                 /* Follow succeeded */
            dir = dj.dir;
            if (!dir) {                     /* Current directory itself */
                res = FR_INVALID_NAME;
            } else {
                if (dir[DIR_Attr] & AM_DIR) /* It is a directory */
                    res = FR_NO_FILE;
            }
        }
#endif
        FREE_BUF();

        if (res == FR_OK) {
            fp->flag = mode;                    /* File access mode */
            fp->err = 0;                        /* Clear error flag */
            fp->sclust = ld_clust(dj.fs, dir);  /* File start cluster */
            fp->fsize = LD_DWORD(dir + DIR_FileSize);   /* File size */
            fp->fptr = 0;                       /* File pointer */
            fp->dsect = 0;
#if _USE_FASTSEEK
            fp->cltbl = 0;                      /* Normal seek mode */
#endif
            fp->fs = dj.fs;                     /* Validate file object */
            fp->id = fp->fs->id;
        }
    }

    LEAVE_FF(dj.fs, res);
}

/*-----------------------------------------------------------------------*/
/* Read File                                                             */
/*-----------------------------------------------------------------------*/
FRESULT f_read (
    FIL* fp,        /* Pointer to the file object */
    void* buff,     /* Pointer to data buffer */
    UINT btr,       /* Number of bytes to read */
    UINT* br        /* Pointer to number of bytes read */
)
{
    FRESULT res;
    DWORD clst, sect, remain;
    UINT rcnt, cc;
    UCHAR csect, *rbuff = (UCHAR*)buff;

    *br = 0;    /* Clear read byte counter */

    res = validate(fp);                         /* Check validity */
    if (res != FR_OK) LEAVE_FF(fp->fs, res);
    if (fp->err)                                /* Check error */
        LEAVE_FF(fp->fs, (FRESULT)fp->err);
    if (!(fp->flag & FA_READ))                  /* Check access mode */
        LEAVE_FF(fp->fs, FR_DENIED);
    remain = fp->fsize - fp->fptr;
    if (btr > remain) btr = (UINT)remain;       /* Truncate btr by remaining bytes */

    for ( ;  btr;                               /* Repeat until all data read */
        rbuff += rcnt, fp->fptr += rcnt, *br += rcnt, btr -= rcnt) {
        if ((fp->fptr % SS(fp->fs)) == 0) {     /* On the sector boundary? */
            csect = (UCHAR)(fp->fptr / SS(fp->fs) & (fp->fs->csize - 1));    /* Sector offset in the cluster */
            if (!csect) {                       /* On the cluster boundary? */
                if (fp->fptr == 0) {            /* On the top of the file? */
                    clst = fp->sclust;          /* Follow from the origin */
                } else {                        /* Middle or end of the file */
#if _USE_FASTSEEK
                    if (fp->cltbl)
                        clst = clmt_clust(fp, fp->fptr);    /* Get cluster# from the CLMT */
                    else
#endif
                        clst = get_fat(fp->fs, fp->clust);  /* Follow cluster chain on the FAT */
                }
                if (clst < 2) ABORT(fp->fs, FR_INT_ERR);
                if (clst == 0xFFFFFFFF) ABORT(fp->fs, FR_DISK_ERR);
                fp->clust = clst;               /* Update current cluster */
            }
            sect = clust2sect(fp->fs, fp->clust);   /* Get current sector */
            if (!sect) ABORT(fp->fs, FR_INT_ERR);
            sect += csect;
            cc = btr / SS(fp->fs);              /* When remaining bytes >= sector size, */
            if (cc) {                           /* Read maximum contiguous sectors directly */
                if (csect + cc > fp->fs->csize) /* Clip at cluster boundary */
                    cc = fp->fs->csize - csect;
                if (disk_read(fp->fs->drv, rbuff, sect, cc) != RES_OK)
                    ABORT(fp->fs, FR_DISK_ERR);
#if !_FS_READONLY && _FS_MINIMIZE <= 2          /* Replace one of the read sectors with cached data if it contains a dirty sector */
#if _FS_TINY
                if (fp->fs->wflag && fp->fs->winsect - sect < cc)
                    mem_cpy(rbuff + ((fp->fs->winsect - sect) * SS(fp->fs)), fp->fs->win, SS(fp->fs));
#else
                if ((fp->flag & FA__DIRTY) && fp->dsect - sect < cc)
                    mem_cpy(rbuff + ((fp->dsect - sect) * SS(fp->fs)), fp->buf, SS(fp->fs));
#endif
#endif
                rcnt = SS(fp->fs) * cc;         /* Number of bytes transferred */
                continue;
            }
#if !_FS_TINY
            if (fp->dsect != sect) {            /* Load data sector if not in cache */
#if !_FS_READONLY
                if (fp->flag & FA__DIRTY) {     /* Write-back dirty sector cache */
                    if (disk_write(fp->fs->drv, fp->buf, fp->dsect, 1) != RES_OK)
                        ABORT(fp->fs, FR_DISK_ERR);
                    fp->flag &= ~FA__DIRTY;
                }
#endif
                if (disk_read(fp->fs->drv, fp->buf, sect, 1) != RES_OK) /* Fill sector cache */
                    ABORT(fp->fs, FR_DISK_ERR);
            }
#endif
            fp->dsect = sect;
        }
        rcnt = SS(fp->fs) - ((UINT)fp->fptr % SS(fp->fs));  /* Get partial sector data from sector buffer */
        if (rcnt > btr) rcnt = btr;
#if _FS_TINY
        if (move_window(fp->fs, fp->dsect) != FR_OK)        /* Move sector window */
            ABORT(fp->fs, FR_DISK_ERR);
        mem_cpy(rbuff, &fp->fs->win[fp->fptr % SS(fp->fs)], rcnt);  /* Pick partial sector */
#else
        mem_cpy(rbuff, &fp->buf[fp->fptr % SS(fp->fs)], rcnt);  /* Pick partial sector */
#endif
    }

    LEAVE_FF(fp->fs, FR_OK);
}

#if !_FS_READONLY
/*-----------------------------------------------------------------------*/
/* Write File                                                            */
/*-----------------------------------------------------------------------*/
FRESULT f_write (
    FIL* fp,            /* Pointer to the file object */
    const void *buff,   /* Pointer to the data to be written */
    UINT btw,           /* Number of bytes to write */
    UINT* bw            /* Pointer to number of bytes written */
)
{
    FRESULT res;
    DWORD clst, sect;
    UINT wcnt, cc;
    const UCHAR *wbuff = (const UCHAR*)buff;
    UCHAR csect;

    *bw = 0;    /* Clear write byte counter */

    res = validate(fp);                     /* Check validity */
    if (res != FR_OK) LEAVE_FF(fp->fs, res);
    if (fp->err)                            /* Check error */
        LEAVE_FF(fp->fs, (FRESULT)fp->err);
    if (!(fp->flag & FA_WRITE))             /* Check access mode */
        LEAVE_FF(fp->fs, FR_DENIED);
    if (fp->fptr + btw < fp->fptr) btw = 0; /* File size cannot reach 4GB */

    for ( ;  btw;                           /* Repeat until all data written */
        wbuff += wcnt, fp->fptr += wcnt, *bw += wcnt, btw -= wcnt) {
        if ((fp->fptr % SS(fp->fs)) == 0) { /* On the sector boundary? */
            csect = (UCHAR)(fp->fptr / SS(fp->fs) & (fp->fs->csize - 1));    /* Sector offset in the cluster */
            if (!csect) {                   /* On the cluster boundary? */
                if (fp->fptr == 0) {        /* On the top of the file? */
                    clst = fp->sclust;      /* Follow from the origin */
                    if (clst == 0)          /* When no cluster is allocated, */
                        clst = create_chain(fp->fs, 0); /* Create a new cluster chain */
                } else {                    /* Middle or end of the file */
#if _USE_FASTSEEK
                    if (fp->cltbl)
                        clst = clmt_clust(fp, fp->fptr);    /* Get cluster# from the CLMT */
                    else
#endif
                        clst = create_chain(fp->fs, fp->clust); /* Follow or stretch cluster chain on the FAT */
                }
                if (clst == 0) break;       /* Could not allocate a new cluster (disk full) */
                if (clst == 1) ABORT(fp->fs, FR_INT_ERR);
                if (clst == 0xFFFFFFFF) ABORT(fp->fs, FR_DISK_ERR);
                fp->clust = clst;           /* Update current cluster */
                if (fp->sclust == 0) fp->sclust = clst; /* Set start cluster if the first write */
            }
#if _FS_TINY
            if (fp->fs->winsect == fp->dsect && sync_window(fp->fs))    /* Write-back sector cache */
                ABORT(fp->fs, FR_DISK_ERR);
#else
            if (fp->flag & FA__DIRTY) {     /* Write-back sector cache */
                if (disk_write(fp->fs->drv, fp->buf, fp->dsect, 1) != RES_OK)
                    ABORT(fp->fs, FR_DISK_ERR);
                fp->flag &= ~FA__DIRTY;
            }
#endif
            sect = clust2sect(fp->fs, fp->clust);   /* Get current sector */
            if (!sect) ABORT(fp->fs, FR_INT_ERR);
            sect += csect;
            cc = btw / SS(fp->fs);          /* When remaining bytes >= sector size, */
            if (cc) {                       /* Write maximum contiguous sectors directly */
                if (csect + cc > fp->fs->csize) /* Clip at cluster boundary */
                    cc = fp->fs->csize - csect;
                if (disk_write(fp->fs->drv, wbuff, sect, cc) != RES_OK)
                    ABORT(fp->fs, FR_DISK_ERR);
#if _FS_MINIMIZE <= 2
#if _FS_TINY
                if (fp->fs->winsect - sect < cc) {  /* Refill sector cache if it gets invalidated by the direct write */
                    mem_cpy(fp->fs->win, wbuff + ((fp->fs->winsect - sect) * SS(fp->fs)), SS(fp->fs));
                    fp->fs->wflag = 0;
                }
#else
                if (fp->dsect - sect < cc) { /* Refill sector cache if it gets invalidated by the direct write */
                    mem_cpy(fp->buf, wbuff + ((fp->dsect - sect) * SS(fp->fs)), SS(fp->fs));
                    fp->flag &= ~FA__DIRTY;
                }
#endif
#endif
                wcnt = SS(fp->fs) * cc;     /* Number of bytes transferred */
                continue;
            }
#if _FS_TINY
            if (fp->fptr >= fp->fsize) {    /* Avoid silly cache filling at growing edge */
                if (sync_window(fp->fs)) ABORT(fp->fs, FR_DISK_ERR);
                fp->fs->winsect = sect;
            }
#else
            if (fp->dsect != sect) {        /* Fill sector cache with file data */
                if (fp->fptr < fp->fsize &&
                    disk_read(fp->fs->drv, fp->buf, sect, 1) != RES_OK)
                        ABORT(fp->fs, FR_DISK_ERR);
            }
#endif
            fp->dsect = sect;
        }
        wcnt = SS(fp->fs) - ((UINT)fp->fptr % SS(fp->fs));/* Put partial sector into file I/O buffer */
        if (wcnt > btw) wcnt = btw;
#if _FS_TINY
        if (move_window(fp->fs, fp->dsect) != FR_OK)    /* Move sector window */
            ABORT(fp->fs, FR_DISK_ERR);
        mem_cpy(&fp->fs->win[fp->fptr % SS(fp->fs)], wbuff, wcnt);  /* Fit partial sector */
        fp->fs->wflag = 1;
#else
        mem_cpy(&fp->buf[fp->fptr % SS(fp->fs)], wbuff, wcnt);  /* Fit partial sector */
        fp->flag |= FA__DIRTY;
#endif
    }

    if (fp->fptr > fp->fsize) fp->fsize = fp->fptr; /* Update file size if needed */
    fp->flag |= FA__WRITTEN;                        /* Set file change flag */

    LEAVE_FF(fp->fs, FR_OK);
}

/*-----------------------------------------------------------------------*/
/* Synchronize the File                                                  */
/*-----------------------------------------------------------------------*/
FRESULT f_sync (
    FIL* fp     /* Pointer to the file object */
)
{
    FRESULT res;
    DWORD tm;
    UCHAR *dir;

    res = validate(fp);                 /* Check validity of the object */
    if (res == FR_OK) {
        if (fp->flag & FA__WRITTEN) {   /* Is there any change to the file? */
#if !_FS_TINY
            if (fp->flag & FA__DIRTY) { /* Write-back cached data if needed */
                if (disk_write(fp->fs->drv, fp->buf, fp->dsect, 1) != RES_OK)
                    LEAVE_FF(fp->fs, FR_DISK_ERR);
                fp->flag &= ~FA__DIRTY;
            }
#endif
            /* Update the directory entry */
            res = move_window(fp->fs, fp->dir_sect);
            if (res == FR_OK) {
                dir = fp->dir_ptr;
                dir[DIR_Attr] |= AM_ARC;                    /* Set archive bit */
                ST_DWORD(dir + DIR_FileSize, fp->fsize);    /* Update file size */
                st_clust(dir, fp->sclust);                  /* Update start cluster */
                tm = GET_FATTIME();                         /* Update modified time */
                ST_DWORD(dir + DIR_WrtTime, tm);
                ST_WORD(dir + DIR_LstAccDate, 0);
                fp->flag &= ~FA__WRITTEN;
                fp->fs->wflag = 1;
                res = sync_fs(fp->fs);
            }
        }
    }

    LEAVE_FF(fp->fs, res);
}
#endif /* !_FS_READONLY */

/*-----------------------------------------------------------------------*/
/* Close File                                                            */
/*-----------------------------------------------------------------------*/
FRESULT f_close (
    FIL *fp     /* Pointer to the file object to be closed */
)
{
    FRESULT res;


#if !_FS_READONLY
    res = f_sync(fp);                   /* Flush cached data */
    if (res == FR_OK)
#endif
    {
        res = validate(fp);             /* Lock volume */
        if (res == FR_OK) {
#if _FS_REENTRANT
            FATFS *fs = fp->fs;
#endif
#if _FS_LOCK
            res = dec_lock(fp->lockid); /* Decrement file open counter */
            if (res == FR_OK)
#endif
                fp->fs = 0;             /* Invalidate file object */
#if _FS_REENTRANT
            unlock_fs(fs, FR_OK);       /* Unlock volume */
#endif
        }
    }
    return res;
}

/*-----------------------------------------------------------------------*/
/* Change Current Directory or Current Drive, Get Current Directory      */
/*-----------------------------------------------------------------------*/
#if _FS_RPATH >= 1
#if _VOLUMES >= 2
FRESULT f_chdrive (
    const TCHAR* path       /* Drive number */
)
{
    int vol;


    vol = get_ldnumber(&path);
    if (vol < 0) return FR_INVALID_DRIVE;

    CurrVol = (UCHAR)vol;

    return FR_OK;
}
#endif

FRESULT f_chdir (
    const TCHAR* path   /* Pointer to the directory path */
)
{
    FRESULT res;
    DIR dj;
    DEFINE_NAMEBUF;

    /* Get logical drive number */
    res = find_volume(&dj.fs, &path, 0);
    if (res == FR_OK) {
        INIT_BUF(dj);
        res = follow_path(&dj, path);       /* Follow the path */
        FREE_BUF();
        if (res == FR_OK) {                 /* Follow completed */
            if (!dj.dir) {
                dj.fs->cdir = dj.sclust;    /* Start directory itself */
            } else {
                if (dj.dir[DIR_Attr] & AM_DIR)  /* Reached to the directory */
                    dj.fs->cdir = ld_clust(dj.fs, dj.dir);
                else
                    res = FR_NO_PATH;       /* Reached but a file */
            }
        }
        if (res == FR_NO_FILE) res = FR_NO_PATH;
    }

    LEAVE_FF(dj.fs, res);
}

#if _FS_RPATH >= 2
FRESULT f_getcwd (
    TCHAR* buff,    /* Pointer to the directory path */
    UINT len        /* Size of path */
)
{
    FRESULT res;
    DIR dj;
    UINT i, n;
    DWORD ccl;
    TCHAR *tp;
    FILINFO fno;
    DEFINE_NAMEBUF;


    *buff = 0;
    /* Get logical drive number */
    res = find_volume(&dj.fs, (const TCHAR**)&buff, 0); /* Get current volume */
    if (res == FR_OK) {
        INIT_BUF(dj);
        i = len;            /* Bottom of buffer (directory stack base) */
        dj.sclust = dj.fs->cdir;            /* Start to follow upper directory from current directory */
        while ((ccl = dj.sclust) != 0) {    /* Repeat while current directory is a sub-directory */
            res = dir_sdi(&dj, 1);          /* Get parent directory */
            if (res != FR_OK) break;
            res = dir_read(&dj, 0);
            if (res != FR_OK) break;
            dj.sclust = ld_clust(dj.fs, dj.dir);    /* Goto parent directory */
            res = dir_sdi(&dj, 0);
            if (res != FR_OK) break;
            do {                            /* Find the entry links to the child directory */
                res = dir_read(&dj, 0);
                if (res != FR_OK) break;
                if (ccl == ld_clust(dj.fs, dj.dir)) break;  /* Found the entry */
                res = dir_next(&dj, 0);
            } while (res == FR_OK);
            if (res == FR_NO_FILE) res = FR_INT_ERR;/* It cannot be 'not found'. */
            if (res != FR_OK) break;
#if _USE_LFN
            fno.lfname = buff;
            fno.lfsize = i;
#endif
            get_fileinfo(&dj, &fno);        /* Get the directory name and push it to the buffer */
            tp = fno.fname;
#if _USE_LFN
            if (*buff) tp = buff;
#endif
            for (n = 0; tp[n]; n++) ;
            if (i < n + 3) {
                res = FR_NOT_ENOUGH_CORE; break;
            }
            while (n) buff[--i] = tp[--n];
            buff[--i] = '/';
        }
        tp = buff;
        if (res == FR_OK) {
#if _VOLUMES >= 2
            *tp++ = '0' + CurrVol;          /* Put drive number */
            *tp++ = ':';
#endif
            if (i == len) {                 /* Root-directory */
                *tp++ = '/';
            } else {                        /* Sub-directroy */
                do      /* Add stacked path str */
                    *tp++ = buff[i++];
                while (i < len);
            }
        }
        *tp = 0;
        FREE_BUF();
    }

    LEAVE_FF(dj.fs, res);
}
#endif /* _FS_RPATH >= 2 */
#endif /* _FS_RPATH >= 1 */

#if _FS_MINIMIZE <= 2
/*-----------------------------------------------------------------------*/
/* Seek File R/W Pointer                                                 */
/*-----------------------------------------------------------------------*/
FRESULT f_lseek (
    FIL* fp,        /* Pointer to the file object */
    DWORD ofs       /* File pointer from top of file */
)
{
    FRESULT res;
    DWORD clst, bcs, nsect, ifptr;
#if _USE_FASTSEEK
    DWORD cl, pcl, ncl, tcl, dsc, tlen, ulen, *tbl;
#endif


    res = validate(fp);                 /* Check validity of the object */
    if (res != FR_OK) LEAVE_FF(fp->fs, res);
    if (fp->err)                        /* Check error */
        LEAVE_FF(fp->fs, (FRESULT)fp->err);

#if _USE_FASTSEEK
    if (fp->cltbl) {    /* Fast seek */
        if (ofs == CREATE_LINKMAP) {    /* Create CLMT */
            tbl = fp->cltbl;
            tlen = *tbl++; ulen = 2;    /* Given table size and required table size */
            cl = fp->sclust;            /* Top of the chain */
            if (cl) {
                do {
                    /* Get a fragment */
                    tcl = cl; ncl = 0; ulen += 2;   /* Top, length and used items */
                    do {
                        pcl = cl; ncl++;
                        cl = get_fat(fp->fs, cl);
                        if (cl <= 1) ABORT(fp->fs, FR_INT_ERR);
                        if (cl == 0xFFFFFFFF) ABORT(fp->fs, FR_DISK_ERR);
                    } while (cl == pcl + 1);
                    if (ulen <= tlen) {     /* Store the length and top of the fragment */
                        *tbl++ = ncl; *tbl++ = tcl;
                    }
                } while (cl < fp->fs->n_fatent);    /* Repeat until end of chain */
            }
            *fp->cltbl = ulen;  /* Number of items used */
            if (ulen <= tlen)
                *tbl = 0;       /* Terminate table */
            else
                res = FR_NOT_ENOUGH_CORE;   /* Given table size is smaller than required */

        } else {                        /* Fast seek */
            if (ofs > fp->fsize)        /* Clip offset at the file size */
                ofs = fp->fsize;
            fp->fptr = ofs;             /* Set file pointer */
            if (ofs) {
                fp->clust = clmt_clust(fp, ofs - 1);
                dsc = clust2sect(fp->fs, fp->clust);
                if (!dsc) ABORT(fp->fs, FR_INT_ERR);
                dsc += (ofs - 1) / SS(fp->fs) & (fp->fs->csize - 1);
                if (fp->fptr % SS(fp->fs) && dsc != fp->dsect) {    /* Refill sector cache if needed */
#if !_FS_TINY
#if !_FS_READONLY
                    if (fp->flag & FA__DIRTY) {     /* Write-back dirty sector cache */
                        if (disk_write(fp->fs->drv, fp->buf, fp->dsect, 1) != RES_OK)
                            ABORT(fp->fs, FR_DISK_ERR);
                        fp->flag &= ~FA__DIRTY;
                    }
#endif
                    if (disk_read(fp->fs->drv, fp->buf, dsc, 1) != RES_OK)  /* Load current sector */
                        ABORT(fp->fs, FR_DISK_ERR);
#endif
                    fp->dsect = dsc;
                }
            }
        }
    } else
#endif

    /* Normal Seek */
    {
        if (ofs > fp->fsize                 /* In read-only mode, clip offset with the file size */
#if !_FS_READONLY
             && !(fp->flag & FA_WRITE)
#endif
            ) ofs = fp->fsize;

        ifptr = fp->fptr;
        fp->fptr = nsect = 0;
        if (ofs) {
            bcs = (DWORD)fp->fs->csize * SS(fp->fs);    /* Cluster size (byte) */
            if (ifptr > 0 &&
                (ofs - 1) / bcs >= (ifptr - 1) / bcs) { /* When seek to same or following cluster, */
                fp->fptr = (ifptr - 1) & ~(bcs - 1);    /* start from the current cluster */
                ofs -= fp->fptr;
                clst = fp->clust;
            } else {                                    /* When seek to back cluster, */
                clst = fp->sclust;                      /* start from the first cluster */
#if !_FS_READONLY
                if (clst == 0) {                        /* If no cluster chain, create a new chain */
                    clst = create_chain(fp->fs, 0);
                    if (clst == 1) ABORT(fp->fs, FR_INT_ERR);
                    if (clst == 0xFFFFFFFF) ABORT(fp->fs, FR_DISK_ERR);
                    fp->sclust = clst;
                }
#endif
                fp->clust = clst;
            }
            if (clst != 0) {
                while (ofs > bcs) {                     /* Cluster following loop */
#if !_FS_READONLY
                    if (fp->flag & FA_WRITE) {          /* Check if in write mode or not */
                        clst = create_chain(fp->fs, clst);  /* Force stretch if in write mode */
                        if (clst == 0) {                /* When disk gets full, clip file size */
                            ofs = bcs; break;
                        }
                    } else
#endif
                        clst = get_fat(fp->fs, clst);   /* Follow cluster chain if not in write mode */
                    if (clst == 0xFFFFFFFF) ABORT(fp->fs, FR_DISK_ERR);
                    if (clst <= 1 || clst >= fp->fs->n_fatent) ABORT(fp->fs, FR_INT_ERR);
                    fp->clust = clst;
                    fp->fptr += bcs;
                    ofs -= bcs;
                }
                fp->fptr += ofs;
                if (ofs % SS(fp->fs)) {
                    nsect = clust2sect(fp->fs, clst);   /* Current sector */
                    if (!nsect) ABORT(fp->fs, FR_INT_ERR);
                    nsect += ofs / SS(fp->fs);
                }
            }
        }
        if (fp->fptr % SS(fp->fs) && nsect != fp->dsect) {  /* Fill sector cache if needed */
#if !_FS_TINY
#if !_FS_READONLY
            if (fp->flag & FA__DIRTY) {         /* Write-back dirty sector cache */
                if (disk_write(fp->fs->drv, fp->buf, fp->dsect, 1) != RES_OK)
                    ABORT(fp->fs, FR_DISK_ERR);
                fp->flag &= ~FA__DIRTY;
            }
#endif
            if (disk_read(fp->fs->drv, fp->buf, nsect, 1) != RES_OK)    /* Fill sector cache */
                ABORT(fp->fs, FR_DISK_ERR);
#endif
            fp->dsect = nsect;
        }
#if !_FS_READONLY
        if (fp->fptr > fp->fsize) {         /* Set file change flag if the file size is extended */
            fp->fsize = fp->fptr;
            fp->flag |= FA__WRITTEN;
        }
#endif
    }

    LEAVE_FF(fp->fs, res);
}

#if _FS_MINIMIZE <= 1
/*-----------------------------------------------------------------------*/
/* Create a Directory Object                                             */
/*-----------------------------------------------------------------------*/

FRESULT f_opendir (
    DIR* dp,            /* Pointer to directory object to create */
    const TCHAR* path   /* Pointer to the directory path */
)
{
    FRESULT res;
    FATFS* fs;
    DEFINE_NAMEBUF;


    if (!dp) return FR_INVALID_OBJECT;

    /* Get logical drive number */
    res = find_volume(&fs, &path, 0);
    if (res == FR_OK) {
        dp->fs = fs;
        INIT_BUF(*dp);
        res = follow_path(dp, path);            /* Follow the path to the directory */
        FREE_BUF();
        if (res == FR_OK) {                     /* Follow completed */
            if (dp->dir) {                      /* It is not the origin directory itself */
                if (dp->dir[DIR_Attr] & AM_DIR) /* The object is a sub directory */
                    dp->sclust = ld_clust(fs, dp->dir);
                else                            /* The object is a file */
                    res = FR_NO_PATH;
            }
            if (res == FR_OK) {
                dp->id = fs->id;
                res = dir_sdi(dp, 0);           /* Rewind directory */
#if _FS_LOCK
                if (res == FR_OK) {
                    if (dp->sclust) {
                        dp->lockid = inc_lock(dp, 0);   /* Lock the sub directory */
                        if (!dp->lockid)
                            res = FR_TOO_MANY_OPEN_FILES;
                    } else {
                        dp->lockid = 0; /* Root directory need not to be locked */
                    }
                }
#endif
            }
        }
        if (res == FR_NO_FILE) res = FR_NO_PATH;
    }
    if (res != FR_OK) dp->fs = 0;       /* Invalidate the directory object if function faild */

    LEAVE_FF(fs, res);
}

/*-----------------------------------------------------------------------*/
/* Close Directory                                                       */
/*-----------------------------------------------------------------------*/
FRESULT f_closedir (
    DIR *dp     /* Pointer to the directory object to be closed */
)
{
    FRESULT res;


    res = validate(dp);
    if (res == FR_OK) {
#if _FS_REENTRANT
        FATFS *fs = dp->fs;
#endif
#if _FS_LOCK
        if (dp->lockid)             /* Decrement sub-directory open counter */
            res = dec_lock(dp->lockid);
        if (res == FR_OK)
#endif
            dp->fs = 0;             /* Invalidate directory object */
#if _FS_REENTRANT
        unlock_fs(fs, FR_OK);       /* Unlock volume */
#endif
    }
    return res;
}

/*-----------------------------------------------------------------------*/
/* Read Directory Entries in Sequence                                    */
/*-----------------------------------------------------------------------*/
FRESULT f_readdir (
    DIR* dp,            /* Pointer to the open directory object */
    FILINFO* fno        /* Pointer to file information to return */
)
{
    FRESULT res;
    DEFINE_NAMEBUF;


    res = validate(dp);                     /* Check validity of the object */
    if (res == FR_OK) {
        if (!fno) {
            res = dir_sdi(dp, 0);           /* Rewind the directory object */
        } else {
            INIT_BUF(*dp);
            res = dir_read(dp, 0);          /* Read an item */
            if (res == FR_NO_FILE) {        /* Reached end of directory */
                dp->sect = 0;
                res = FR_OK;
            }
            if (res == FR_OK) {             /* A valid entry is found */
                get_fileinfo(dp, fno);      /* Get the object information */
                res = dir_next(dp, 0);      /* Increment index for next */
                if (res == FR_NO_FILE) {
                    dp->sect = 0;
                    res = FR_OK;
                }
            }
            FREE_BUF();
        }
    }

    LEAVE_FF(dp->fs, res);
}

#if _USE_FIND
/*-----------------------------------------------------------------------*/
/* Find next file                                                        */
/*-----------------------------------------------------------------------*/
FRESULT f_findnext (
    DIR* dp,        /* Pointer to the open directory object */
    FILINFO* fno    /* Pointer to the file information structure */
)
{
    FRESULT res;


    for (;;) {
        res = f_readdir(dp, fno);       /* Get a directory item */
        if (res != FR_OK || !fno || !fno->fname[0]) break;  /* Terminate if any error or end of directory */
#if _USE_LFN
        if (fno->lfname && pattern_matching(dp->pat, fno->lfname, 0, 0)) break; /* Test for LFN if exist */
#endif
        if (pattern_matching(dp->pat, fno->fname, 0, 0)) break; /* Test for SFN */
    }
    return res;

}

/*-----------------------------------------------------------------------*/
/* Find first file                                                       */
/*-----------------------------------------------------------------------*/
FRESULT f_findfirst (
    DIR* dp,                /* Pointer to the blank directory object */
    FILINFO* fno,           /* Pointer to the file information structure */
    const TCHAR* path,      /* Pointer to the directory to open */
    const TCHAR* pattern    /* Pointer to the matching pattern */
)
{
    FRESULT res;

    dp->pat = pattern;      /* Save pointer to pattern string */
    res = f_opendir(dp, path);      /* Open the target directory */
    if (res == FR_OK)
        res = f_findnext(dp, fno);  /* Find the first item */
    return res;
}

#endif  /* _USE_FIND */

#if _FS_MINIMIZE == 0
/*-----------------------------------------------------------------------*/
/* Get File Status                                                       */
/*-----------------------------------------------------------------------*/
FRESULT f_stat (
    const TCHAR* path,  /* Pointer to the file path */
    FILINFO* fno        /* Pointer to file information to return */
)
{
    FRESULT res;
    DIR dj;
    DEFINE_NAMEBUF;

    /* Get logical drive number */
    res = find_volume(&dj.fs, &path, 0);
    if (res == FR_OK) {
        INIT_BUF(dj);
        res = follow_path(&dj, path);   /* Follow the file path */
        if (res == FR_OK) {             /* Follow completed */
            if (dj.dir) {       /* Found an object */
                if (fno) get_fileinfo(&dj, fno);
            } else {            /* It is root directory */
                res = FR_INVALID_NAME;
            }
        }
        FREE_BUF();
    }

    LEAVE_FF(dj.fs, res);
}

#if !_FS_READONLY
/*-----------------------------------------------------------------------*/
/* Get Number of Free Clusters                                           */
/*-----------------------------------------------------------------------*/
FRESULT f_getfree (
    const TCHAR* path,  /* Path name of the logical drive number */
    DWORD* nclst,       /* Pointer to a variable to return number of free clusters */
    FATFS** fatfs       /* Pointer to return pointer to corresponding file system object */
)
{
    FRESULT res;
    FATFS *fs;
    DWORD nfree, clst, sect, stat;
    UINT i;
    UCHAR fat, *p;


    /* Get logical drive number */
    res = find_volume(fatfs, &path, 0);
    fs = *fatfs;
    if (res == FR_OK) {
        /* If free_clust is valid, return it without full cluster scan */
        if (fs->free_clust <= fs->n_fatent - 2) {
            *nclst = fs->free_clust;
        } else {
            /* Get number of free clusters */
            fat = fs->fs_type;
            nfree = 0;
            if (fat == FS_FAT12) {  /* Sector unalighed entries: Search FAT via regular routine. */
                clst = 2;
                do {
                    stat = get_fat(fs, clst);
                    if (stat == 0xFFFFFFFF) { res = FR_DISK_ERR; break; }
                    if (stat == 1) { res = FR_INT_ERR; break; }
                    if (stat == 0) nfree++;
                } while (++clst < fs->n_fatent);
            } else {                /* Sector alighed entries: Accelerate the FAT search. */
                clst = fs->n_fatent; sect = fs->fatbase;
                i = 0; p = 0;
                do {
                    if (!i) {
                        res = move_window(fs, sect++);
                        if (res != FR_OK) break;
                        p = fs->win;
                        i = SS(fs);
                    }
                    if (fat == FS_FAT16) {
                        if (LD_WORD(p) == 0) nfree++;
                        p += 2; i -= 2;
                    } else {
                        if ((LD_DWORD(p) & 0x0FFFFFFF) == 0) nfree++;
                        p += 4; i -= 4;
                    }
                } while (--clst);
            }
            fs->free_clust = nfree; /* free_clust is valid */
            fs->fsi_flag |= 1;      /* FSInfo is to be updated */
            *nclst = nfree;         /* Return the free clusters */
        }
    }
    LEAVE_FF(fs, res);
}

/*-----------------------------------------------------------------------*/
/* Truncate File                                                         */
/*-----------------------------------------------------------------------*/
FRESULT f_truncate (
    FIL* fp     /* Pointer to the file object */
)
{
    FRESULT res;
    DWORD ncl;


    res = validate(fp);                     /* Check validity of the object */
    if (res == FR_OK) {
        if (fp->err) {                      /* Check error */
            res = (FRESULT)fp->err;
        } else {
            if (!(fp->flag & FA_WRITE))     /* Check access mode */
                res = FR_DENIED;
        }
    }
    if (res == FR_OK) {
        if (fp->fsize > fp->fptr) {
            fp->fsize = fp->fptr;   /* Set file size to current R/W point */
            fp->flag |= FA__WRITTEN;
            if (fp->fptr == 0) {    /* When set file size to zero, remove entire cluster chain */
                res = remove_chain(fp->fs, fp->sclust);
                fp->sclust = 0;
            } else {                /* When truncate a part of the file, remove remaining clusters */
                ncl = get_fat(fp->fs, fp->clust);
                res = FR_OK;
                if (ncl == 0xFFFFFFFF) res = FR_DISK_ERR;
                if (ncl == 1) res = FR_INT_ERR;
                if (res == FR_OK && ncl < fp->fs->n_fatent) {
                    res = put_fat(fp->fs, fp->clust, 0x0FFFFFFF);
                    if (res == FR_OK) res = remove_chain(fp->fs, ncl);
                }
            }
#if !_FS_TINY
            if (res == FR_OK && (fp->flag & FA__DIRTY)) {
                if (disk_write(fp->fs->drv, fp->buf, fp->dsect, 1) != RES_OK)
                    res = FR_DISK_ERR;
                else
                    fp->flag &= ~FA__DIRTY;
            }
#endif
        }
        if (res != FR_OK) fp->err = (FRESULT)res;
    }

    LEAVE_FF(fp->fs, res);
}

/*-----------------------------------------------------------------------*/
/* Delete a File or Directory                                            */
/*-----------------------------------------------------------------------*/
FRESULT f_unlink (
    const TCHAR* path       /* Pointer to the file or directory path */
)
{
    FRESULT res;
    DIR dj, sdj;
    UCHAR *dir;
    DWORD dclst = 0;
    DEFINE_NAMEBUF;

    /* Get logical drive number */
    res = find_volume(&dj.fs, &path, 1);
    if (res == FR_OK) {
        INIT_BUF(dj);
        res = follow_path(&dj, path);       /* Follow the file path */
        if (_FS_RPATH && res == FR_OK && (dj.fn[NSFLAG] & NS_DOT))
            res = FR_INVALID_NAME;          /* Cannot remove dot entry */
#if _FS_LOCK
        if (res == FR_OK) res = chk_lock(&dj, 2);   /* Cannot remove open object */
#endif
        if (res == FR_OK) {                 /* The object is accessible */
            dir = dj.dir;
            if (!dir) {
                res = FR_INVALID_NAME;      /* Cannot remove the origin directory */
            } else {
                if (dir[DIR_Attr] & AM_RDO)
                    res = FR_DENIED;        /* Cannot remove R/O object */
            }
            if (res == FR_OK) {
                dclst = ld_clust(dj.fs, dir);
                if (dclst && (dir[DIR_Attr] & AM_DIR)) {    /* Is it a sub-directory ? */
#if _FS_RPATH
                    if (dclst == dj.fs->cdir) {             /* Is it the current directory? */
                        res = FR_DENIED;
                    } else
#endif
                    {
                        mem_cpy(&sdj, &dj, sizeof (DIR));   /* Open the sub-directory */
                        sdj.sclust = dclst;
                        res = dir_sdi(&sdj, 2);
                        if (res == FR_OK) {
                            res = dir_read(&sdj, 0);            /* Read an item (excluding dot entries) */
                            if (res == FR_OK) res = FR_DENIED;  /* Not empty? (cannot remove) */
                            if (res == FR_NO_FILE) res = FR_OK; /* Empty? (can remove) */
                        }
                    }
                }
            }
            if (res == FR_OK) {
                res = dir_remove(&dj);      /* Remove the directory entry */
                if (res == FR_OK && dclst)  /* Remove the cluster chain if exist */
                    res = remove_chain(dj.fs, dclst);
                if (res == FR_OK) res = sync_fs(dj.fs);
            }
        }
        FREE_BUF();
    }

    LEAVE_FF(dj.fs, res);
}

/*-----------------------------------------------------------------------*/
/* Create a Directory                                                    */
/*-----------------------------------------------------------------------*/
FRESULT f_mkdir (
    const TCHAR* path       /* Pointer to the directory path */
)
{
    FRESULT res;
    DIR dj;
    UCHAR *dir, n;
    DWORD dsc, dcl, pcl, tm = GET_FATTIME();
    DEFINE_NAMEBUF;


    /* Get logical drive number */
    res = find_volume(&dj.fs, &path, 1);
    if (res == FR_OK) {
        INIT_BUF(dj);
        res = follow_path(&dj, path);           /* Follow the file path */
        if (res == FR_OK) res = FR_EXIST;       /* Any object with same name is already existing */
        if (_FS_RPATH && res == FR_NO_FILE && (dj.fn[NSFLAG] & NS_DOT))
            res = FR_INVALID_NAME;
        if (res == FR_NO_FILE) {                /* Can create a new directory */
            dcl = create_chain(dj.fs, 0);       /* Allocate a cluster for the new directory table */
            res = FR_OK;
            if (dcl == 0) res = FR_DENIED;      /* No space to allocate a new cluster */
            if (dcl == 1) res = FR_INT_ERR;
            if (dcl == 0xFFFFFFFF) res = FR_DISK_ERR;
            if (res == FR_OK)                   /* Flush FAT */
                res = sync_window(dj.fs);
            if (res == FR_OK) {                 /* Initialize the new directory table */
                dsc = clust2sect(dj.fs, dcl);
                dir = dj.fs->win;
                mem_set(dir, 0, SS(dj.fs));
                mem_set(dir + DIR_Name, ' ', 11);   /* Create "." entry */
                dir[DIR_Name] = '.';
                dir[DIR_Attr] = AM_DIR;
                ST_DWORD(dir + DIR_WrtTime, tm);
                st_clust(dir, dcl);
                mem_cpy(dir + SZ_DIRE, dir, SZ_DIRE);   /* Create ".." entry */
                dir[SZ_DIRE + 1] = '.'; pcl = dj.sclust;
                if (dj.fs->fs_type == FS_FAT32 && pcl == dj.fs->dirbase)
                    pcl = 0;
                st_clust(dir + SZ_DIRE, pcl);
                for (n = dj.fs->csize; n; n--) {    /* Write dot entries and clear following sectors */
                    dj.fs->winsect = dsc++;
                    dj.fs->wflag = 1;
                    res = sync_window(dj.fs);
                    if (res != FR_OK) break;
                    mem_set(dir, 0, SS(dj.fs));
                }
            }
            if (res == FR_OK) res = dir_register(&dj);  /* Register the object to the directoy */
            if (res != FR_OK) {
                remove_chain(dj.fs, dcl);           /* Could not register, remove cluster chain */
            } else {
                dir = dj.dir;
                dir[DIR_Attr] = AM_DIR;             /* Attribute */
                ST_DWORD(dir + DIR_WrtTime, tm);    /* Created time */
                st_clust(dir, dcl);                 /* Table start cluster */
                dj.fs->wflag = 1;
                res = sync_fs(dj.fs);
            }
        }
        FREE_BUF();
    }

    LEAVE_FF(dj.fs, res);
}


/*-----------------------------------------------------------------------*/
/* Change Attribute                                                      */
/*-----------------------------------------------------------------------*/
FRESULT f_chmod (
    const TCHAR* path,  /* Pointer to the file path */
    UCHAR attr,          /* Attribute bits */
    UCHAR mask           /* Attribute mask to change */
)
{
    FRESULT res;
    DIR dj;
    UCHAR *dir;
    DEFINE_NAMEBUF;


    res = find_volume(&dj.fs, &path, 1);    /* Get logical drive number */
    if (res == FR_OK) {
        INIT_BUF(dj);
        res = follow_path(&dj, path);       /* Follow the file path */
        FREE_BUF();
        if (_FS_RPATH && res == FR_OK && (dj.fn[NSFLAG] & NS_DOT))
            res = FR_INVALID_NAME;
        if (res == FR_OK) {
            dir = dj.dir;
            if (!dir) {                     /* Is it a root directory? */
                res = FR_INVALID_NAME;
            } else {                        /* File or sub directory */
                mask &= AM_RDO|AM_HID|AM_SYS|AM_ARC;    /* Valid attribute mask */
                dir[DIR_Attr] = (attr & mask) | (dir[DIR_Attr] & (UCHAR)~mask);  /* Apply attribute change */
                dj.fs->wflag = 1;
                res = sync_fs(dj.fs);
            }
        }
    }

    LEAVE_FF(dj.fs, res);
}

/*-----------------------------------------------------------------------*/
/* Rename File/Directory                                                 */
/*-----------------------------------------------------------------------*/
FRESULT f_rename (
    const TCHAR* path_old,  /* Pointer to the object to be renamed */
    const TCHAR* path_new   /* Pointer to the new name */
)
{
    FRESULT res;
    DIR djo, djn;
    UCHAR buf[21], *dir;
    DWORD dw;
    DEFINE_NAMEBUF;


    /* Get logical drive number of the source object */
    res = find_volume(&djo.fs, &path_old, 1);
    if (res == FR_OK) {
        djn.fs = djo.fs;
        INIT_BUF(djo);
        res = follow_path(&djo, path_old);      /* Check old object */
        if (_FS_RPATH && res == FR_OK && (djo.fn[NSFLAG] & NS_DOT))
            res = FR_INVALID_NAME;
#if _FS_LOCK
        if (res == FR_OK) res = chk_lock(&djo, 2);
#endif
        if (res == FR_OK) {                     /* Old object is found */
            if (!djo.dir) {                     /* Is root dir? */
                res = FR_NO_FILE;
            } else {
                mem_cpy(buf, djo.dir + DIR_Attr, 21);   /* Save information about object except name */
                mem_cpy(&djn, &djo, sizeof (DIR));      /* Duplicate the directory object */
                if (get_ldnumber(&path_new) >= 0)       /* Snip drive number off and ignore it */
                    res = follow_path(&djn, path_new);  /* and make sure if new object name is not conflicting */
                else
                    res = FR_INVALID_DRIVE;
                if (res == FR_OK) res = FR_EXIST;       /* The new object name is already existing */
                if (res == FR_NO_FILE) {                /* It is a valid path and no name collision */
                    res = dir_register(&djn);           /* Register the new entry */
                    if (res == FR_OK) {
/* Start of critical section where any interruption can cause a cross-link */
                        dir = djn.dir;                  /* Copy information about object except name */
                        mem_cpy(dir + 13, buf + 2, 19);
                        dir[DIR_Attr] = buf[0] | AM_ARC;
                        djo.fs->wflag = 1;
                        if ((dir[DIR_Attr] & AM_DIR) && djo.sclust != djn.sclust) { /* Update .. entry in the sub-directory if needed */
                            dw = clust2sect(djo.fs, ld_clust(djo.fs, dir));
                            if (!dw) {
                                res = FR_INT_ERR;
                            } else {
                                res = move_window(djo.fs, dw);
                                dir = djo.fs->win + SZ_DIRE * 1;    /* Ptr to .. entry */
                                if (res == FR_OK && dir[1] == '.') {
                                    st_clust(dir, djn.sclust);
                                    djo.fs->wflag = 1;
                                }
                            }
                        }
                        if (res == FR_OK) {
                            res = dir_remove(&djo);     /* Remove old entry */
                            if (res == FR_OK)
                                res = sync_fs(djo.fs);
                        }
/* End of critical section */
                    }
                }
            }
        }
        FREE_BUF();
    }

    LEAVE_FF(djo.fs, res);
}

/*-----------------------------------------------------------------------*/
/* Change Timestamp                                                      */
/*-----------------------------------------------------------------------*/
FRESULT f_utime (
    const TCHAR* path,  /* Pointer to the file/directory name */
    const FILINFO* fno  /* Pointer to the time stamp to be set */
)
{
    FRESULT res;
    DIR dj;
    UCHAR *dir;
    DEFINE_NAMEBUF;


    /* Get logical drive number */
    res = find_volume(&dj.fs, &path, 1);
    if (res == FR_OK) {
        INIT_BUF(dj);
        res = follow_path(&dj, path);   /* Follow the file path */
        FREE_BUF();
        if (_FS_RPATH && res == FR_OK && (dj.fn[NSFLAG] & NS_DOT))
            res = FR_INVALID_NAME;
        if (res == FR_OK) {
            dir = dj.dir;
            if (!dir) {                 /* Root directory */
                res = FR_INVALID_NAME;
            } else {                    /* File or sub-directory */
                ST_WORD(dir + DIR_WrtTime, fno->ftime);
                ST_WORD(dir + DIR_WrtDate, fno->fdate);
                dj.fs->wflag = 1;
                res = sync_fs(dj.fs);
            }
        }
    }

    LEAVE_FF(dj.fs, res);
}

#endif /* !_FS_READONLY */
#endif /* _FS_MINIMIZE == 0 */
#endif /* _FS_MINIMIZE <= 1 */
#endif /* _FS_MINIMIZE <= 2 */

#if _USE_LABEL
/*-----------------------------------------------------------------------*/
/* Get volume label                                                      */
/*-----------------------------------------------------------------------*/
FRESULT f_getlabel (
    const TCHAR* path,  /* Path name of the logical drive number */
    TCHAR* label,       /* Pointer to a buffer to return the volume label */
    DWORD* vsn          /* Pointer to a variable to return the volume serial number */
)
{
    FRESULT res;
    DIR dj;
    UINT i, j;
#if _USE_LFN && _LFN_UNICODE
    WCHAR w;
#endif

    /* Get logical drive number */
    res = find_volume(&dj.fs, &path, 0);

    /* Get volume label */
    if (res == FR_OK && label) {
        dj.sclust = 0;                  /* Open root directory */
        res = dir_sdi(&dj, 0);
        if (res == FR_OK) {
            res = dir_read(&dj, 1);     /* Get an entry with AM_VOL */
            if (res == FR_OK) {         /* A volume label is exist */
#if _USE_LFN && _LFN_UNICODE
                i = j = 0;
                do {
                    w = (i < 11) ? dj.dir[i++] : ' ';
                    if (IsDBCS1(w) && i < 11 && IsDBCS2(dj.dir[i]))
                        w = w << 8 | dj.dir[i++];
                    label[j++] = ff_convert(w, 1);  /* OEM -> Unicode */
                } while (j < 11);
#else
                mem_cpy(label, dj.dir, 11);
#endif
                j = 11;
                do {
                    label[j] = 0;
                    if (!j) break;
                } while (label[--j] == ' ');
            }
            if (res == FR_NO_FILE) {    /* No label, return nul string */
                label[0] = 0;
                res = FR_OK;
            }
        }
    }

    /* Get volume serial number */
    if (res == FR_OK && vsn) {
        res = move_window(dj.fs, dj.fs->volbase);
        if (res == FR_OK) {
            i = dj.fs->fs_type == FS_FAT32 ? BS_VolID32 : BS_VolID;
            *vsn = LD_DWORD(&dj.fs->win[i]);
        }
    }

    LEAVE_FF(dj.fs, res);
}

#if !_FS_READONLY
/*-----------------------------------------------------------------------*/
/* Set volume label                                                      */
/*-----------------------------------------------------------------------*/
FRESULT f_setlabel (
    const TCHAR* label  /* Pointer to the volume label to set */
)
{
    FRESULT res;
    DIR dj;
    UCHAR vn[11];
    UINT i, j, sl;
    WCHAR w;
    DWORD tm;


    /* Get logical drive number */
    res = find_volume(&dj.fs, &label, 1);
    if (res) LEAVE_FF(dj.fs, res);

    /* Create a volume label in directory form */
    vn[0] = 0;
    for (sl = 0; label[sl]; sl++) ;             /* Get name length */
    for ( ; sl && label[sl - 1] == ' '; sl--) ; /* Remove trailing spaces */
    if (sl) {   /* Create volume label in directory form */
        i = j = 0;
        do {
#if _USE_LFN && _LFN_UNICODE
            w = ff_convert(ff_wtoupper(label[i++]), 0);
#else
            w = (UCHAR)label[i++];
            if (IsDBCS1(w))
                w = (j < 10 && i < sl && IsDBCS2(label[i])) ? w << 8 | (UCHAR)label[i++] : 0;
#if _USE_LFN
            w = ff_convert(ff_wtoupper(ff_convert(w, 1)), 0);
#else
            if (IsLower(w)) w -= 0x20;          /* To upper ASCII characters */
#ifdef _EXCVT
            if (w >= 0x80) w = ExCvt[w - 0x80]; /* To upper extended characters (SBCS cfg) */
#else
            if (!_DF1S && w >= 0x80) w = 0;     /* Reject extended characters (ASCII cfg) */
#endif
#endif
#endif
            if (!w || chk_chr("\"*+,.:;<=>\?[]|\x7F", w) || j >= (UINT)((w >= 0x100) ? 10 : 11)) /* Reject invalid characters for volume label */
                LEAVE_FF(dj.fs, FR_INVALID_NAME);
            if (w >= 0x100) vn[j++] = (UCHAR)(w >> 8);
            vn[j++] = (UCHAR)w;
        } while (i < sl);
        while (j < 11) vn[j++] = ' ';   /* Fill remaining name field */
        if (vn[0] == DDEM) LEAVE_FF(dj.fs, FR_INVALID_NAME);    /* Reject illegal name (heading DDEM) */
    }

    /* Set volume label */
    dj.sclust = 0;                  /* Open root directory */
    res = dir_sdi(&dj, 0);
    if (res == FR_OK) {
        res = dir_read(&dj, 1);     /* Get an entry with AM_VOL */
        if (res == FR_OK) {         /* A volume label is found */
            if (vn[0]) {
                mem_cpy(dj.dir, vn, 11);    /* Change the volume label name */
                tm = GET_FATTIME();
                ST_DWORD(dj.dir + DIR_WrtTime, tm);
            } else {
                dj.dir[0] = DDEM;           /* Remove the volume label */
            }
            dj.fs->wflag = 1;
            res = sync_fs(dj.fs);
        } else {                    /* No volume label is found or error */
            if (res == FR_NO_FILE) {
                res = FR_OK;
                if (vn[0]) {                /* Create volume label as new */
                    res = dir_alloc(&dj, 1);    /* Allocate an entry for volume label */
                    if (res == FR_OK) {
                        mem_set(dj.dir, 0, SZ_DIRE);    /* Set volume label */
                        mem_cpy(dj.dir, vn, 11);
                        dj.dir[DIR_Attr] = AM_VOL;
                        tm = GET_FATTIME();
                        ST_DWORD(dj.dir + DIR_WrtTime, tm);
                        dj.fs->wflag = 1;
                        res = sync_fs(dj.fs);
                    }
                }
            }
        }
    }

    LEAVE_FF(dj.fs, res);
}

#endif /* !_FS_READONLY */
#endif /* _USE_LABEL */

/*-----------------------------------------------------------------------*/
/* Forward data to the stream directly (available on only tiny cfg)      */
/*-----------------------------------------------------------------------*/
#if _USE_FORWARD && _FS_TINY
FRESULT f_forward (
    FIL* fp,                        /* Pointer to the file object */
    UINT (*func)(const UCHAR*,UINT), /* Pointer to the streaming function */
    UINT btf,                       /* Number of bytes to forward */
    UINT* bf                        /* Pointer to number of bytes forwarded */
)
{
    FRESULT res;
    DWORD remain, clst, sect;
    UINT rcnt;
    UCHAR csect;


    *bf = 0;    /* Clear transfer byte counter */

    res = validate(fp);                             /* Check validity of the object */
    if (res != FR_OK) LEAVE_FF(fp->fs, res);
    if (fp->err)                                    /* Check error */
        LEAVE_FF(fp->fs, (FRESULT)fp->err);
    if (!(fp->flag & FA_READ))                      /* Check access mode */
        LEAVE_FF(fp->fs, FR_DENIED);

    remain = fp->fsize - fp->fptr;
    if (btf > remain) btf = (UINT)remain;           /* Truncate btf by remaining bytes */

    for ( ;  btf && (*func)(0, 0);                  /* Repeat until all data transferred or stream becomes busy */
        fp->fptr += rcnt, *bf += rcnt, btf -= rcnt) {
        csect = (UCHAR)(fp->fptr / SS(fp->fs) & (fp->fs->csize - 1));    /* Sector offset in the cluster */
        if ((fp->fptr % SS(fp->fs)) == 0) {         /* On the sector boundary? */
            if (!csect) {                           /* On the cluster boundary? */
                clst = (fp->fptr == 0) ?            /* On the top of the file? */
                    fp->sclust : get_fat(fp->fs, fp->clust);
                if (clst <= 1) ABORT(fp->fs, FR_INT_ERR);
                if (clst == 0xFFFFFFFF) ABORT(fp->fs, FR_DISK_ERR);
                fp->clust = clst;                   /* Update current cluster */
            }
        }
        sect = clust2sect(fp->fs, fp->clust);       /* Get current data sector */
        if (!sect) ABORT(fp->fs, FR_INT_ERR);
        sect += csect;
        if (move_window(fp->fs, sect) != FR_OK)     /* Move sector window */
            ABORT(fp->fs, FR_DISK_ERR);
        fp->dsect = sect;
        rcnt = SS(fp->fs) - (WORD)(fp->fptr % SS(fp->fs));  /* Forward data from sector window */
        if (rcnt > btf) rcnt = btf;
        rcnt = (*func)(&fp->fs->win[(WORD)fp->fptr % SS(fp->fs)], rcnt);
        if (!rcnt) ABORT(fp->fs, FR_INT_ERR);
    }

    LEAVE_FF(fp->fs, FR_OK);
}
#endif /* _USE_FORWARD */

#if _USE_MKFS && !_FS_READONLY
/*-----------------------------------------------------------------------*/
/* Create file system on the logical drive                               */
/*-----------------------------------------------------------------------*/
#define N_ROOTDIR   512     /* Number of root directory entries for FAT12/16 */
#define N_FATS      1       /* Number of FATs (1 or 2) */

FRESULT f_mkfs (
    const TCHAR* path,  /* Logical drive number */
    UCHAR sfd,           /* Partitioning rule 0:FDISK, 1:SFD */
    UINT au             /* Size of allocation unit in unit of byte or sector */
)
{
    static const WORD vst[] = { 1024,   512,  256,  128,   64,    32,   16,    8,    4,    2,   0};
    static const WORD cst[] = {32768, 16384, 8192, 4096, 2048, 16384, 8192, 4096, 2048, 1024, 512};
    int vol;
    UCHAR fmt, md, sys, *tbl, pdrv, part;
    DWORD n_clst, vs, n, wsect;
    UINT i;
    DWORD b_vol, b_fat, b_dir, b_data;  /* LBA */
    DWORD n_vol, n_rsv, n_fat, n_dir;   /* Size */
    FATFS *fs;
    DSTATUS stat;
#if _USE_TRIM
    DWORD eb[2];
#endif

    /* Check mounted drive and clear work area */
    if (sfd > 1) return FR_INVALID_PARAMETER;
    vol = get_ldnumber(&path);
    if (vol < 0) return FR_INVALID_DRIVE;
    fs = FatFs[vol];
    if (!fs) return FR_NOT_ENABLED;
    fs->fs_type = 0;
    pdrv = LD2PD(vol);  /* Physical drive */
    part = LD2PT(vol);  /* Partition (0:auto detect, 1-4:get from partition table)*/

    /* Get disk statics */
    stat = disk_initialize(pdrv);
    if (stat & STA_NOINIT) return FR_NOT_READY;
    if (stat & STA_PROTECT) return FR_WRITE_PROTECTED;
#if _MAX_SS != _MIN_SS      /* Get disk sector size */
    if (disk_ioctl(pdrv, GET_SECTOR_SIZE, &SS(fs)) != RES_OK || SS(fs) > _MAX_SS || SS(fs) < _MIN_SS)
        return FR_DISK_ERR;
#endif
    if (_MULTI_PARTITION && part) {
        /* Get partition information from partition table in the MBR */
        if (disk_read(pdrv, fs->win, 0, 1) != RES_OK) return FR_DISK_ERR;
        if (LD_WORD(fs->win + BS_55AA) != 0xAA55) return FR_MKFS_ABORTED;
        tbl = &fs->win[MBR_Table + (part - 1) * SZ_PTE];
        if (!tbl[4]) return FR_MKFS_ABORTED;    /* No partition? */
        b_vol = LD_DWORD(tbl + 8);  /* Volume start sector */
        n_vol = LD_DWORD(tbl + 12); /* Volume size */
    } else {
        /* Create a partition in this function */
        if (disk_ioctl(pdrv, GET_SECTOR_COUNT, &n_vol) != RES_OK || n_vol < 128)
            return FR_DISK_ERR;
        b_vol = (sfd) ? 0 : 63;     /* Volume start sector */
        n_vol -= b_vol;             /* Volume size */
    }

    if (au & (au - 1)) au = 0;
    if (!au) {                      /* AU auto selection */
        vs = n_vol / (2000 / (SS(fs) / 512));
        for (i = 0; vs < vst[i]; i++) ;
        au = cst[i];
    }
    if (au >= _MIN_SS) au /= SS(fs);    /* Number of sectors per cluster */
    if (!au) au = 1;
    if (au > 128) au = 128;

    /* Pre-compute number of clusters and FAT sub-type */
    n_clst = n_vol / au;
    fmt = FS_FAT12;
    if (n_clst >= MIN_FAT16) fmt = FS_FAT16;
    if (n_clst >= MIN_FAT32) fmt = FS_FAT32;

    /* Determine offset and size of FAT structure */
    if (fmt == FS_FAT32) {
        n_fat = ((n_clst * 4) + 8 + SS(fs) - 1) / SS(fs);
        n_rsv = 32;
        n_dir = 0;
    } else {
        n_fat = (fmt == FS_FAT12) ? (n_clst * 3 + 1) / 2 + 3 : (n_clst * 2) + 4;
        n_fat = (n_fat + SS(fs) - 1) / SS(fs);
        n_rsv = 1;
        n_dir = (DWORD)N_ROOTDIR * SZ_DIRE / SS(fs);
    }
    b_fat = b_vol + n_rsv;              /* FAT area start sector */
    b_dir = b_fat + n_fat * N_FATS;     /* Directory area start sector */
    b_data = b_dir + n_dir;             /* Data area start sector */
    if (n_vol < b_data + au - b_vol) return FR_MKFS_ABORTED;    /* Too small volume */

    /* Align data start sector to erase block boundary (for flash memory media) */
    if (disk_ioctl(pdrv, GET_BLOCK_SIZE, &n) != RES_OK || !n || n > 32768) n = 1;
    n = (b_data + n - 1) & ~(n - 1);    /* Next nearest erase block from current data start */
    n = (n - b_data) / N_FATS;
    if (fmt == FS_FAT32) {      /* FAT32: Move FAT offset */
        n_rsv += n;
        b_fat += n;
    } else {                    /* FAT12/16: Expand FAT size */
        n_fat += n;
    }

    /* Determine number of clusters and final check of validity of the FAT sub-type */
    n_clst = (n_vol - n_rsv - n_fat * N_FATS - n_dir) / au;
    if (   (fmt == FS_FAT16 && n_clst < MIN_FAT16)
        || (fmt == FS_FAT32 && n_clst < MIN_FAT32))
        return FR_MKFS_ABORTED;

    /* Determine system ID in the partition table */
    if (fmt == FS_FAT32) {
        sys = 0x0C;     /* FAT32X */
    } else {
        if (fmt == FS_FAT12 && n_vol < 0x10000) {
            sys = 0x01; /* FAT12(<65536) */
        } else {
            sys = (n_vol < 0x10000) ? 0x04 : 0x06;  /* FAT16(<65536) : FAT12/16(>=65536) */
        }
    }

    if (_MULTI_PARTITION && part) {
        /* Update system ID in the partition table */
        tbl = &fs->win[MBR_Table + (part - 1) * SZ_PTE];
        tbl[4] = sys;
        if (disk_write(pdrv, fs->win, 0, 1) != RES_OK)  /* Write it to teh MBR */
            return FR_DISK_ERR;
        md = 0xF8;
    } else {
        if (sfd) {  /* No partition table (SFD) */
            md = 0xF0;
        } else {    /* Create partition table (FDISK) */
            mem_set(fs->win, 0, SS(fs));
            tbl = fs->win + MBR_Table;  /* Create partition table for single partition in the drive */
            tbl[1] = 1;                     /* Partition start head */
            tbl[2] = 1;                     /* Partition start sector */
            tbl[3] = 0;                     /* Partition start cylinder */
            tbl[4] = sys;                   /* System type */
            tbl[5] = 254;                   /* Partition end head */
            n = (b_vol + n_vol) / 63 / 255;
            tbl[6] = (UCHAR)(n >> 2 | 63);   /* Partition end sector */
            tbl[7] = (UCHAR)n;               /* End cylinder */
            ST_DWORD(tbl + 8, 63);          /* Partition start in LBA */
            ST_DWORD(tbl + 12, n_vol);      /* Partition size in LBA */
            ST_WORD(fs->win + BS_55AA, 0xAA55); /* MBR signature */
            if (disk_write(pdrv, fs->win, 0, 1) != RES_OK)  /* Write it to the MBR */
                return FR_DISK_ERR;
            md = 0xF8;
        }
    }

    /* Create BPB in the VBR */
    tbl = fs->win;                          /* Clear sector */
    mem_set(tbl, 0, SS(fs));
    mem_cpy(tbl, "\xEB\xFE\x90" "MSDOS5.0", 11);/* Boot jump code, OEM name */
    i = SS(fs);                             /* Sector size */
    ST_WORD(tbl + BPB_BytsPerSec, i);
    tbl[BPB_SecPerClus] = (UCHAR)au;         /* Sectors per cluster */
    ST_WORD(tbl + BPB_RsvdSecCnt, n_rsv);   /* Reserved sectors */
    tbl[BPB_NumFATs] = N_FATS;              /* Number of FATs */
    i = (fmt == FS_FAT32) ? 0 : N_ROOTDIR;  /* Number of root directory entries */
    ST_WORD(tbl + BPB_RootEntCnt, i);
    if (n_vol < 0x10000) {                  /* Number of total sectors */
        ST_WORD(tbl + BPB_TotSec16, n_vol);
    } else {
        ST_DWORD(tbl + BPB_TotSec32, n_vol);
    }
    tbl[BPB_Media] = md;                    /* Media descriptor */
    ST_WORD(tbl + BPB_SecPerTrk, 63);       /* Number of sectors per track */
    ST_WORD(tbl + BPB_NumHeads, 255);       /* Number of heads */
    ST_DWORD(tbl + BPB_HiddSec, b_vol);     /* Hidden sectors */
    n = GET_FATTIME();                      /* Use current time as VSN */
    if (fmt == FS_FAT32) {
        ST_DWORD(tbl + BS_VolID32, n);      /* VSN */
        ST_DWORD(tbl + BPB_FATSz32, n_fat); /* Number of sectors per FAT */
        ST_DWORD(tbl + BPB_RootClus, 2);    /* Root directory start cluster (2) */
        ST_WORD(tbl + BPB_FSInfo, 1);       /* FSINFO record offset (VBR + 1) */
        ST_WORD(tbl + BPB_BkBootSec, 6);    /* Backup boot record offset (VBR + 6) */
        tbl[BS_DrvNum32] = 0x80;            /* Drive number */
        tbl[BS_BootSig32] = 0x29;           /* Extended boot signature */
        mem_cpy(tbl + BS_VolLab32, "NO NAME    " "FAT32   ", 19);   /* Volume label, FAT signature */
    } else {
        ST_DWORD(tbl + BS_VolID, n);        /* VSN */
        ST_WORD(tbl + BPB_FATSz16, n_fat);  /* Number of sectors per FAT */
        tbl[BS_DrvNum] = 0x80;              /* Drive number */
        tbl[BS_BootSig] = 0x29;             /* Extended boot signature */
        mem_cpy(tbl + BS_VolLab, "NO NAME    " "FAT     ", 19); /* Volume label, FAT signature */
    }
    ST_WORD(tbl + BS_55AA, 0xAA55);         /* Signature (Offset is fixed here regardless of sector size) */
    if (disk_write(pdrv, tbl, b_vol, 1) != RES_OK)  /* Write it to the VBR sector */
        return FR_DISK_ERR;
    if (fmt == FS_FAT32)                    /* Write it to the backup VBR if needed (VBR + 6) */
        disk_write(pdrv, tbl, b_vol + 6, 1);

    /* Initialize FAT area */
    wsect = b_fat;
    for (i = 0; i < N_FATS; i++) {      /* Initialize each FAT copy */
        mem_set(tbl, 0, SS(fs));            /* 1st sector of the FAT  */
        n = md;                             /* Media descriptor byte */
        if (fmt != FS_FAT32) {
            n |= (fmt == FS_FAT12) ? 0x00FFFF00 : 0xFFFFFF00;
            ST_DWORD(tbl + 0, n);           /* Reserve cluster #0-1 (FAT12/16) */
        } else {
            n |= 0xFFFFFF00;
            ST_DWORD(tbl + 0, n);           /* Reserve cluster #0-1 (FAT32) */
            ST_DWORD(tbl + 4, 0xFFFFFFFF);
            ST_DWORD(tbl + 8, 0x0FFFFFFF);  /* Reserve cluster #2 for root directory */
        }
        if (disk_write(pdrv, tbl, wsect++, 1) != RES_OK)
            return FR_DISK_ERR;
        mem_set(tbl, 0, SS(fs));            /* Fill following FAT entries with zero */
        for (n = 1; n < n_fat; n++) {       /* This loop may take a time on FAT32 volume due to many single sector writes */
            if (disk_write(pdrv, tbl, wsect++, 1) != RES_OK)
                return FR_DISK_ERR;
        }
    }

    /* Initialize root directory */
    i = (fmt == FS_FAT32) ? au : (UINT)n_dir;
    do {
        if (disk_write(pdrv, tbl, wsect++, 1) != RES_OK)
            return FR_DISK_ERR;
    } while (--i);

#if _USE_TRIM   /* Erase data area if needed */
    {
        eb[0] = wsect; eb[1] = wsect + (n_clst - ((fmt == FS_FAT32) ? 1 : 0)) * au - 1;
        disk_ioctl(pdrv, CTRL_TRIM, eb);
    }
#endif

    /* Create FSINFO if needed */
    if (fmt == FS_FAT32) {
        ST_DWORD(tbl + FSI_LeadSig, 0x41615252);
        ST_DWORD(tbl + FSI_StrucSig, 0x61417272);
        ST_DWORD(tbl + FSI_Free_Count, n_clst - 1); /* Number of free clusters */
        ST_DWORD(tbl + FSI_Nxt_Free, 2);            /* Last allocated cluster# */
        ST_WORD(tbl + BS_55AA, 0xAA55);
        disk_write(pdrv, tbl, b_vol + 1, 1);    /* Write original (VBR + 1) */
        disk_write(pdrv, tbl, b_vol + 7, 1);    /* Write backup (VBR + 7) */
    }

    return (disk_ioctl(pdrv, CTRL_SYNC, 0) == RES_OK) ? FR_OK : FR_DISK_ERR;
}

#if _MULTI_PARTITION
/*-----------------------------------------------------------------------*/
/* Create partition table on the physical drive                          */
/*-----------------------------------------------------------------------*/
FRESULT f_fdisk (
    UCHAR pdrv,          /* Physical drive number */
    const DWORD szt[],  /* Pointer to the size table for each partitions */
    void* work          /* Pointer to the working buffer */
)
{
    UINT i, n, sz_cyl, tot_cyl, b_cyl, e_cyl, p_cyl;
    UCHAR s_hd, e_hd, *p, *buf = (UCHAR*)work;
    DSTATUS stat;
    DWORD sz_disk, sz_part, s_part;


    stat = disk_initialize(pdrv);
    if (stat & STA_NOINIT) return FR_NOT_READY;
    if (stat & STA_PROTECT) return FR_WRITE_PROTECTED;
    if (disk_ioctl(pdrv, GET_SECTOR_COUNT, &sz_disk)) return FR_DISK_ERR;

    /* Determine CHS in the table regardless of the drive geometry */
    for (n = 16; n < 256 && sz_disk / n / 63 > 1024; n *= 2) ;
    if (n == 256) n--;
    e_hd = n - 1;
    sz_cyl = 63 * n;
    tot_cyl = sz_disk / sz_cyl;

    /* Create partition table */
    mem_set(buf, 0, _MAX_SS);
    p = buf + MBR_Table; b_cyl = 0;
    for (i = 0; i < 4; i++, p += SZ_PTE) {
        p_cyl = (szt[i] <= 100U) ? (DWORD)tot_cyl * szt[i] / 100 : szt[i] / sz_cyl;
        if (!p_cyl) continue;
        s_part = (DWORD)sz_cyl * b_cyl;
        sz_part = (DWORD)sz_cyl * p_cyl;
        if (i == 0) {   /* Exclude first track of cylinder 0 */
            s_hd = 1;
            s_part += 63; sz_part -= 63;
        } else {
            s_hd = 0;
        }
        e_cyl = b_cyl + p_cyl - 1;
        if (e_cyl >= tot_cyl) return FR_INVALID_PARAMETER;

        /* Set partition table */
        p[1] = s_hd;                        /* Start head */
        p[2] = (UCHAR)((b_cyl >> 2) + 1);    /* Start sector */
        p[3] = (UCHAR)b_cyl;                 /* Start cylinder */
        p[4] = 0x06;                        /* System type (temporary setting) */
        p[5] = e_hd;                        /* End head */
        p[6] = (UCHAR)((e_cyl >> 2) + 63);   /* End sector */
        p[7] = (UCHAR)e_cyl;                 /* End cylinder */
        ST_DWORD(p + 8, s_part);            /* Start sector in LBA */
        ST_DWORD(p + 12, sz_part);          /* Partition size */

        /* Next partition */
        b_cyl += p_cyl;
    }
    ST_WORD(p, 0xAA55);

    /* Write it to the MBR */
    return (disk_write(pdrv, buf, 0, 1) != RES_OK || disk_ioctl(pdrv, CTRL_SYNC, 0) != RES_OK) ? FR_DISK_ERR : FR_OK;
}

#endif /* _MULTI_PARTITION */
#endif /* _USE_MKFS && !_FS_READONLY */

#if _USE_STRFUNC
/*-----------------------------------------------------------------------*/
/* Get a string from the file                                            */
/*-----------------------------------------------------------------------*/
TCHAR* f_gets (
    TCHAR* buff,    /* Pointer to the string buffer to read */
    int len,        /* Size of string buffer (characters) */
    FIL* fp         /* Pointer to the file object */
)
{
    int n = 0;
    TCHAR c, *p = buff;
    UCHAR s[2];
    UINT rc;


    while (n < len - 1) {   /* Read characters until buffer gets filled */
#if _USE_LFN && _LFN_UNICODE
#if _STRF_ENCODE == 3       /* Read a character in UTF-8 */
        f_read(fp, s, 1, &rc);
        if (rc != 1) break;
        c = s[0];
        if (c >= 0x80) {
            if (c < 0xC0) continue; /* Skip stray trailer */
            if (c < 0xE0) {         /* Two-byte sequence */
                f_read(fp, s, 1, &rc);
                if (rc != 1) break;
                c = (c & 0x1F) << 6 | (s[0] & 0x3F);
                if (c < 0x80) c = '?';
            } else {
                if (c < 0xF0) {     /* Three-byte sequence */
                    f_read(fp, s, 2, &rc);
                    if (rc != 2) break;
                    c = c << 12 | (s[0] & 0x3F) << 6 | (s[1] & 0x3F);
                    if (c < 0x800) c = '?';
                } else {            /* Reject four-byte sequence */
                    c = '?';
                }
            }
        }
#elif _STRF_ENCODE == 2     /* Read a character in UTF-16BE */
        f_read(fp, s, 2, &rc);
        if (rc != 2) break;
        c = s[1] + (s[0] << 8);
#elif _STRF_ENCODE == 1     /* Read a character in UTF-16LE */
        f_read(fp, s, 2, &rc);
        if (rc != 2) break;
        c = s[0] + (s[1] << 8);
#else                       /* Read a character in ANSI/OEM */
        f_read(fp, s, 1, &rc);
        if (rc != 1) break;
        c = s[0];
        if (IsDBCS1(c)) {
            f_read(fp, s, 1, &rc);
            if (rc != 1) break;
            c = (c << 8) + s[0];
        }
        c = ff_convert(c, 1);   /* OEM -> Unicode */
        if (!c) c = '?';
#endif
#else                       /* Read a character without conversion */
        f_read(fp, s, 1, &rc);
        if (rc != 1) break;
        c = s[0];
#endif
        if (_USE_STRFUNC == 2 && c == '\r') continue;   /* Strip '\r' */
        *p++ = c;
        n++;
        if (c == '\n') break;       /* Break on EOL */
    }
    *p = 0;
    return n ? buff : 0;            /* When no data read (eof or error), return with error. */
}

#if !_FS_READONLY
#include <stdarg.h>
/*-----------------------------------------------------------------------*/
/* Put a character to the file                                           */
/*-----------------------------------------------------------------------*/
typedef struct {
    FIL* fp;
    int idx, nchr;
    UCHAR buf[64];
} putbuff;

static
void putc_bfd (
    putbuff* pb,
    TCHAR c
)
{
    UINT bw;
    int i;

    if (_USE_STRFUNC == 2 && c == '\n')  /* LF -> CRLF conversion */
        putc_bfd(pb, '\r');

    i = pb->idx;    /* Buffer write index (-1:error) */
    if (i < 0) return;

#if _USE_LFN && _LFN_UNICODE
#if _STRF_ENCODE == 3           /* Write a character in UTF-8 */
    if (c < 0x80) {             /* 7-bit */
        pb->buf[i++] = (UCHAR)c;
    } else {
        if (c < 0x800) {        /* 11-bit */
            pb->buf[i++] = (UCHAR)(0xC0 | c >> 6);
        } else {                /* 16-bit */
            pb->buf[i++] = (UCHAR)(0xE0 | c >> 12);
            pb->buf[i++] = (UCHAR)(0x80 | (c >> 6 & 0x3F));
        }
        pb->buf[i++] = (UCHAR)(0x80 | (c & 0x3F));
    }
#elif _STRF_ENCODE == 2         /* Write a character in UTF-16BE */
    pb->buf[i++] = (UCHAR)(c >> 8);
    pb->buf[i++] = (UCHAR)c;
#elif _STRF_ENCODE == 1         /* Write a character in UTF-16LE */
    pb->buf[i++] = (UCHAR)c;
    pb->buf[i++] = (UCHAR)(c >> 8);
#else                           /* Write a character in ANSI/OEM */
    c = ff_convert(c, 0);   /* Unicode -> OEM */
    if (!c) c = '?';
    if (c >= 0x100)
        pb->buf[i++] = (UCHAR)(c >> 8);
    pb->buf[i++] = (UCHAR)c;
#endif
#else                           /* Write a character without conversion */
    pb->buf[i++] = (UCHAR)c;
#endif

    if (i >= (int)(sizeof pb->buf) - 3) {   /* Write buffered characters to the file */
        f_write(pb->fp, pb->buf, (UINT)i, &bw);
        i = (bw == (UINT)i) ? 0 : -1;
    }
    pb->idx = i;
    pb->nchr++;
}


int f_putc (
    TCHAR c,    /* A character to be output */
    FIL* fp     /* Pointer to the file object */
)
{
    putbuff pb;
    UINT nw;


    pb.fp = fp;         /* Initialize output buffer */
    pb.nchr = pb.idx = 0;

    putc_bfd(&pb, c);   /* Put a character */

    if (   pb.idx >= 0  /* Flush buffered characters to the file */
        && f_write(pb.fp, pb.buf, (UINT)pb.idx, &nw) == FR_OK
        && (UINT)pb.idx == nw) return pb.nchr;
    return EOF;
}


/*-----------------------------------------------------------------------*/
/* Put a string to the file                                              */
/*-----------------------------------------------------------------------*/
int f_puts (
    const TCHAR* str,   /* Pointer to the string to be output */
    FIL* fp             /* Pointer to the file object */
)
{
    putbuff pb;
    UINT nw;


    pb.fp = fp;             /* Initialize output buffer */
    pb.nchr = pb.idx = 0;

    while (*str)            /* Put the string */
        putc_bfd(&pb, *str++);

    if (   pb.idx >= 0      /* Flush buffered characters to the file */
        && f_write(pb.fp, pb.buf, (UINT)pb.idx, &nw) == FR_OK
        && (UINT)pb.idx == nw) return pb.nchr;
    return EOF;
}


/*-----------------------------------------------------------------------*/
/* Put a formatted string to the file                                    */
/*-----------------------------------------------------------------------*/
int f_printf (
    FIL* fp,            /* Pointer to the file object */
    const TCHAR* fmt,   /* Pointer to the format string */
    ...                 /* Optional arguments... */
)
{
    va_list arp;
    UCHAR f, r;
    UINT nw, i, j, w;
    DWORD v;
    TCHAR c, d, s[16], *p;
    putbuff pb;


    pb.fp = fp;             /* Initialize output buffer */
    pb.nchr = pb.idx = 0;

    va_start(arp, fmt);

    for (;;) {
        c = *fmt++;
        if (c == 0) break;          /* End of string */
        if (c != '%') {             /* Non escape character */
            putc_bfd(&pb, c);
            continue;
        }
        w = f = 0;
        c = *fmt++;
        if (c == '0') {             /* Flag: '0' padding */
            f = 1; c = *fmt++;
        } else {
            if (c == '-') {         /* Flag: left justified */
                f = 2; c = *fmt++;
            }
        }
        while (IsDigit(c)) {        /* Precision */
            w = w * 10 + c - '0';
            c = *fmt++;
        }
        if (c == 'l' || c == 'L') { /* Prefix: Size is long int */
            f |= 4; c = *fmt++;
        }
        if (!c) break;
        d = c;
        if (IsLower(d)) d -= 0x20;
        switch (d) {                /* Type is... */
        case 'S' :                  /* String */
            p = va_arg(arp, TCHAR*);
            for (j = 0; p[j]; j++) ;
            if (!(f & 2)) {
                while (j++ < w) putc_bfd(&pb, ' ');
            }
            while (*p) putc_bfd(&pb, *p++);
            while (j++ < w) putc_bfd(&pb, ' ');
            continue;
        case 'C' :                  /* Character */
            putc_bfd(&pb, (TCHAR)va_arg(arp, int)); continue;
        case 'B' :                  /* Binary */
            r = 2; break;
        case 'O' :                  /* Octal */
            r = 8; break;
        case 'D' :                  /* Signed decimal */
        case 'U' :                  /* Unsigned decimal */
            r = 10; break;
        case 'X' :                  /* Hexdecimal */
            r = 16; break;
        default:                    /* Unknown type (pass-through) */
            putc_bfd(&pb, c); continue;
        }

        /* Get an argument and put it in numeral */
        v = (f & 4) ? (DWORD)va_arg(arp, long) : ((d == 'D') ? (DWORD)(long)va_arg(arp, int) : (DWORD)va_arg(arp, unsigned int));
        if (d == 'D' && (v & 0x80000000)) {
            v = 0 - v;
            f |= 8;
        }
        i = 0;
        do {
            d = (TCHAR)(v % r); v /= r;
            if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
            s[i++] = d + '0';
        } while (v && i < sizeof s / sizeof s[0]);
        if (f & 8) s[i++] = '-';
        j = i; d = (f & 1) ? '0' : ' ';
        while (!(f & 2) && j++ < w) putc_bfd(&pb, d);
        do putc_bfd(&pb, s[--i]); while (i);
        while (j++ < w) putc_bfd(&pb, d);
    }

    va_end(arp);

    if (   pb.idx >= 0      /* Flush buffered characters to the file */
        && f_write(pb.fp, pb.buf, (UINT)pb.idx, &nw) == FR_OK
        && (UINT)pb.idx == nw) return pb.nchr;
    return EOF;
}

#endif /* !_FS_READONLY */
#endif /* _USE_STRFUNC */


#endif /* _FATFS */
