/*************  技术支持与购买说明    **************
该库为CH32V103的文件系统库.
技术支持QQ群一：1138055784
******************************************/

#ifndef __PFF_H
#define __PFF_H

#include "CH32V_SDCARD.h"
#include "pffconf.h"

#ifndef PF_DEFINED
#define PF_DEFINED	8088	/* Revision ID */

#if PF_DEFINED != PFCONF_DEF
#error Wrong configuration file (pffconf.h).
#endif

#define	PF_INTDEF 1

/* File system object structure */
typedef struct {
	uint8_t	fs_type;	/* FAT sub type */
	uint8_t	flag;		/* File status flags */
	uint8_t	csize;		/* Number of sectors per cluster */
	uint8_t	pad1;
	WORD	n_rootdir;	/* Number of root directory entries (0 on FAT32) */
	CLUST	n_fatent;	/* Number of FAT entries (= number of clusters + 2) */
	DWORD	fatbase;	/* FAT start sector */
	DWORD	dirbase;	/* Root directory start sector (Cluster# on FAT32) */
	DWORD	database;	/* Data start sector */
	DWORD	fptr;		/* File R/W pointer */
	DWORD	fsize;		/* File size */
	CLUST	org_clust;	/* File start cluster */
	CLUST	curr_clust;	/* File current cluster */
	DWORD	dsect;		/* File current data sector */
} FATFS;

/* Directory object structure */
typedef struct {
	WORD	index;		/* Current read/write index number */
	uint8_t*	fn;			/* Pointer to the SFN (in/out) {file[8],ext[3],status[1]} */
	CLUST	sclust;		/* Table start cluster (0:Static table) */
	CLUST	clust;		/* Current cluster */
	DWORD	sect;		/* Current sector */
} DIR;

/* File status structure */
typedef struct {
	DWORD	fsize;		/* File size */
	WORD	fdate;		/* Last modified date */
	WORD	ftime;		/* Last modified time */
	uint8_t	fattrib;	/* Attribute */
	char	fname[13];	/* File name */
} FILINFO;

/* File function return code (FRESULT) */
typedef enum {
	FR_OK = 0,			/* 0 */
	FR_DISK_ERR,		/* 1 */
	FR_NOT_READY,		/* 2 */
	FR_NO_FILE,			/* 3 */
	FR_NOT_OPENED,		/* 4 */
	FR_NOT_ENABLED,		/* 5 */
	FR_NO_FILESYSTEM	/* 6 */
} FRESULT;

/*--------------------------------------------------------------*/
/* Flags and offset address                                     */
/* File status flag (FATFS.flag) */
#define	FA_OPENED	0x01
#define	FA_WPRT		0x02
#define	FA__WIP		0x40

/* FAT sub type (FATFS.fs_type) */
#define FS_FAT12	1
#define FS_FAT16	2
#define FS_FAT32	3

/* File attribute bits for directory entry */
#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define	AM_VOL	0x08	/* Volume label */
#define AM_LFN	0x0F	/* LFN entry */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */
#define AM_MASK	0x3F	/* Mask of defined bits */

#endif /* _PFATFS */

#if PF_DEFINED != 8088	/* Revision ID */
#error Wrong include file (pff.h).
#endif

#if PF_FS_FAT32
#if !PF_FS_FAT16 && !PF_FS_FAT12
#define _FS_32ONLY 1
#else
#define _FS_32ONLY 0
#endif
#else
#if !PF_FS_FAT16 && !PF_FS_FAT12
#error Wrong PF_FS_FATxx setting.
#endif
#define _FS_32ONLY 0
#endif

#define ABORT(err)	{fs->flag = 0; return err;}

/* Character code support macros */
#define IsUpper(c)	(((c)>='A')&&((c)<='Z'))
#define IsLower(c)	(((c)>='a')&&((c)<='z'))

#if PF_USE_LCC && !defined(_EXCVT)	/* DBCS configuration */

#ifdef _DF2S	/* Two 1st byte areas */
#define IsDBCS1(c)	(((uint8_t)(c) >= _DF1S && (uint8_t)(c) <= _DF1E) || ((uint8_t)(c) >= _DF2S && (uint8_t)(c) <= _DF2E))
#else			/* One 1st byte area */
#define IsDBCS1(c)	((uint8_t)(c) >= _DF1S && (uint8_t)(c) <= _DF1E)
#endif

#ifdef _DS3S	/* Three 2nd byte areas */
#define IsDBCS2(c)	(((uint8_t)(c) >= _DS1S && (uint8_t)(c) <= _DS1E) || ((uint8_t)(c) >= _DS2S && (uint8_t)(c) <= _DS2E) || ((uint8_t)(c) >= _DS3S && (uint8_t)(c) <= _DS3E))
#else			/* Two 2nd byte areas */
#define IsDBCS2(c)	(((uint8_t)(c) >= _DS1S && (uint8_t)(c) <= _DS1E) || ((uint8_t)(c) >= _DS2S && (uint8_t)(c) <= _DS2E))
#endif

#else			/* SBCS configuration */

#define IsDBCS1(c)	0
#define IsDBCS2(c)	0

#endif /* _EXCVT */


/* FatFs refers the members in the FAT structures with byte offset instead
/ of structure member because there are incompatibility of the packing option
/ between various compilers. */
#define BS_jmpBoot			0
#define BS_OEMName			3
#define BPB_BytsPerSec		11
#define BPB_SecPerClus		13
#define BPB_RsvdSecCnt		14
#define BPB_NumFATs			16
#define BPB_RootEntCnt		17
#define BPB_TotSec16		19
#define BPB_Media			21
#define BPB_FATSz16			22
#define BPB_SecPerTrk		24
#define BPB_NumHeads		26
#define BPB_HiddSec			28
#define BPB_TotSec32		32
#define BS_55AA				510

#define BS_DrvNum			36
#define BS_BootSig			38
#define BS_VolID			39
#define BS_VolLab			43
#define BS_FilSysType		54

#define BPB_FATSz32			36
#define BPB_ExtFlags		40
#define BPB_FSVer			42
#define BPB_RootClus		44
#define BPB_FSInfo			48
#define BPB_BkBootSec		50
#define BS_DrvNum32			64
#define BS_BootSig32		66
#define BS_VolID32			67
#define BS_VolLab32			71
#define BS_FilSysType32		82

#define MBR_Table			446

#define	DIR_Name			0
#define	DIR_Attr			11
#define	DIR_NTres			12
#define	DIR_CrtTime			14
#define	DIR_CrtDate			16
#define	DIR_FstClusHI		20
#define	DIR_WrtTime			22
#define	DIR_WrtDate			24
#define	DIR_FstClusLO		26
#define	DIR_FileSize		28

FRESULT pf_mount (FATFS* fs);								/* Mount/Unmount a logical drive */
FRESULT pf_open (const char* path);							/* Open a file */
FRESULT pf_read (void* buff, UINT btr, UINT* br);			/* Read data from the open file */
FRESULT pf_write (const void* buff, UINT btw, UINT* bw);	/* Write data to the open file */
FRESULT pf_lseek (DWORD ofs);								/* Move file pointer of the open file */
FRESULT pf_opendir (DIR* dj, const char* path);				/* Open a directory */
FRESULT pf_readdir (DIR* dj, FILINFO* fno);					/* Read a directory item from the open directory */


static FATFS  *FatFs;	/* Pointer to the file system object (logical drive) */
SDCARD sd(PC12);

DSTATUS disk_initialize()
{
	return sd.init();
}

DRESULT disk_readp(uint8_t* buff, uint32_t sector, uint16_t offser, uint16_t count)
{
	return sd.readp(buff, sector, offser, count);
}

DRESULT disk_writep(const uint8_t* buff, uint32_t sc)
{
	return sd.writep(buff,sc);
}

/*-----------------------------------------------------------------------*/
/* Load multi-byte word in the FAT structure                             */
/*-----------------------------------------------------------------------*/
static WORD ld_word (const uint8_t* ptr)	/*	 Load a 2-byte little-endian word */
{
	WORD rv;

	rv = ptr[1];
	rv = rv << 8 | ptr[0];
	return rv;
}

static DWORD ld_dword (const uint8_t* ptr)	/* Load a 4-byte little-endian word */
{
	DWORD rv;

	rv = ptr[3];
	rv = rv << 8 | ptr[2];
	rv = rv << 8 | ptr[1];
	rv = rv << 8 | ptr[0];
	return rv;
}

/*-----------------------------------------------------------------------*/
/* String functions                                                      */
/*-----------------------------------------------------------------------*/
/* Fill memory block */
static void mem_set (void* dst, int val, int cnt) {
	char *d = (char*)dst;
	while (cnt--) *d++ = (char)val;
}

/* Compare memory block */
static int mem_cmp (const void* dst, const void* src, int cnt) {
	const char *d = (const char *)dst, *s = (const char *)src;
	int r = 0;
	while (cnt-- && (r = *d++ - *s++) == 0) ;
	return r;
}

/*-----------------------------------------------------------------------*/
/* FAT access - Read value of a FAT entry                                */
/*-----------------------------------------------------------------------*/
static CLUST get_fat (	/* 1:IO error, Else:Cluster status */
	CLUST clst	/* Cluster# to get the link information */
)
{
	uint8_t buf[4];
	FATFS *fs = FatFs;
#if PF_FS_FAT12
	UINT wc, bc, ofs;
#endif

	if (clst < 2 || clst >= fs->n_fatent) return 1;	/* Range check */
	switch (fs->fs_type) {
#if PF_FS_FAT12
	case FS_FAT12 : {
		bc = (UINT)clst; bc += bc / 2;
		ofs = bc % 512; bc /= 512;
		if (ofs != 511) {
			if (disk_readp(buf, fs->fatbase + bc, ofs, 2)) break;
		} else {
			if (disk_readp(buf, fs->fatbase + bc, 511, 1)) break;
			if (disk_readp(buf+1, fs->fatbase + bc + 1, 0, 1)) break;
		}
		wc = ld_word(buf);
		return (clst & 1) ? (wc >> 4) : (wc & 0xFFF);
	}
#endif
#if PF_FS_FAT16
	case FS_FAT16 :
		if (disk_readp(buf, fs->fatbase + clst / 256, ((UINT)clst % 256) * 2, 2)) break;
		return ld_word(buf);
#endif
#if PF_FS_FAT32
	case FS_FAT32 :
		if (disk_readp(buf, fs->fatbase + clst / 128, ((UINT)clst % 128) * 4, 4)) break;
		return ld_dword(buf) & 0x0FFFFFFF;
#endif
	}

	return 1;	/* An error occured at the disk I/O layer */
}

/*-----------------------------------------------------------------------*/
/* Get sector# from cluster# / Get cluster field from directory entry    */
/*-----------------------------------------------------------------------*/
static DWORD clust2sect (	/* !=0: Sector number, 0: Failed - invalid cluster# */
	CLUST clst		/* Cluster# to be converted */
)
{
	FATFS *fs = FatFs;


	clst -= 2;
	if (clst >= (fs->n_fatent - 2)) return 0;		/* Invalid cluster# */
	return (DWORD)clst * fs->csize + fs->database;
}


static CLUST get_clust (
	uint8_t* dir		/* Pointer to directory entry */
)
{
	FATFS *fs = FatFs;
	CLUST clst = 0;


	if (_FS_32ONLY || (PF_FS_FAT32 && fs->fs_type == FS_FAT32)) {
		clst = ld_word(dir+DIR_FstClusHI);
		clst <<= 16;
	}
	clst |= ld_word(dir+DIR_FstClusLO);

	return clst;
}

/*-----------------------------------------------------------------------*/
/* Directory handling - Rewind directory index                           */
/*-----------------------------------------------------------------------*/
static FRESULT dir_rewind (
	DIR *dj			/* Pointer to directory object */
)
{
	CLUST clst;
	FATFS *fs = FatFs;


	dj->index = 0;
	clst = dj->sclust;
	if (clst == 1 || clst >= fs->n_fatent) {	/* Check start cluster range */
		return FR_DISK_ERR;
	}
	if (PF_FS_FAT32 && !clst && (_FS_32ONLY || fs->fs_type == FS_FAT32)) {	/* Replace cluster# 0 with root cluster# if in FAT32 */
		clst = (CLUST)fs->dirbase;
	}
	dj->clust = clst;						/* Current cluster */
        
	//dj->sect = ((_FS_32ONLY) || (clst)) ? clust2sect(clst) : fs->dirbase;	/* Current sector */
        if((_FS_32ONLY) || (clst))
        {
           dj->sect =clust2sect(clst);

         }else{
           dj->sect=fs->dirbase;

        }

	return FR_OK;	/* Seek succeeded */
}

/*-----------------------------------------------------------------------*/
/* Directory handling - Move directory index next                        */
/*-----------------------------------------------------------------------*/
static FRESULT dir_next (	/* FR_OK:Succeeded, FR_NO_FILE:End of table */
	DIR *dj			/* Pointer to directory object */
)
{
	CLUST clst;
	WORD i;
	FATFS *fs = FatFs;


	i = dj->index + 1;
	if (!i || !dj->sect) return FR_NO_FILE;	/* Report EOT when index has reached 65535 */

	if (!(i % 16)) {		/* Sector changed? */
		dj->sect++;			/* Next sector */

		if (dj->clust == 0) {	/* Static table */
			if (i >= fs->n_rootdir) return FR_NO_FILE;	/* Report EOT when end of table */
		}
		else {					/* Dynamic table */
			if (((i / 16) & (fs->csize - 1)) == 0) {	/* Cluster changed? */
				clst = get_fat(dj->clust);		/* Get next cluster */
				if (clst <= 1) return FR_DISK_ERR;
				if (clst >= fs->n_fatent) return FR_NO_FILE;	/* Report EOT when it reached end of dynamic table */
				dj->clust = clst;				/* Initialize data for new cluster */
				dj->sect = clust2sect(clst);
			}
		}
	}

	dj->index = i;

	return FR_OK;
}

/*-----------------------------------------------------------------------*/
/* Directory handling - Find an object in the directory                  */
/*-----------------------------------------------------------------------*/
static FRESULT dir_find (
	DIR *dj,		/* Pointer to the directory object linked to the file name */
	uint8_t *dir		/* 32-byte working buffer */
)
{
	FRESULT res;
	uint8_t c;


	res = dir_rewind(dj);			/* Rewind directory object */
	if (res != FR_OK) return res;

	do {
		res = disk_readp(dir, dj->sect, (dj->index % 16) * 32, 32)	/* Read an entry */
			? FR_DISK_ERR : FR_OK;
		if (res != FR_OK) break;
		c = dir[DIR_Name];	/* First character */
		if (c == 0) { res = FR_NO_FILE; break; }	/* Reached to end of table */
		if (!(dir[DIR_Attr] & AM_VOL) && !mem_cmp(dir, dj->fn, 11)) break;	/* Is it a valid entry? */
		res = dir_next(dj);					/* Next entry */
	} while (res == FR_OK);

	return res;
}

/*-----------------------------------------------------------------------*/
/* Read an object from the directory                                     */
/*-----------------------------------------------------------------------*/
#if PF_USE_DIR
static FRESULT dir_read (
	DIR *dj,		/* Pointer to the directory object to store read object name */
	uint8_t *dir		/* 32-byte working buffer */
)
{
	FRESULT res;
	uint8_t a, c;


	res = FR_NO_FILE;
	while (dj->sect) {
		res = disk_readp(dir, dj->sect, (dj->index % 16) * 32, 32)	/* Read an entry */
			? FR_DISK_ERR : FR_OK;
		if (res != FR_OK) break;
		c = dir[DIR_Name];
		if (c == 0) { res = FR_NO_FILE; break; }	/* Reached to end of table */
		a = dir[DIR_Attr] & AM_MASK;
		if (c != 0xE5 && c != '.' && !(a & AM_VOL))	break;	/* Is it a valid entry? */
		res = dir_next(dj);			/* Next entry */
		if (res != FR_OK) break;
	}

	if (res != FR_OK) dj->sect = 0;

	return res;
}
#endif

/*-----------------------------------------------------------------------*/
/* Pick a segment and create the object name in directory form           */
/*-----------------------------------------------------------------------*/
static FRESULT create_name (
	DIR *dj,			/* Pointer to the directory object */
	const char **path	/* Pointer to pointer to the segment in the path string */
)
{
	uint8_t c, ni, si, i, *sfn;
#if IsDBCS1(c)
	uint8_t d;
#endif
	const char *p;
#if PF_USE_LCC && defined(_EXCVT)
	static const uint8_t cvt[] = _EXCVT;
#endif

	/* Create file name in directory form */
	sfn = dj->fn;
	mem_set(sfn, ' ', 11);
	si = i = 0; ni = 8;
	p = *path;
	for (;;) {
		c = p[si++];
		if (c <= ' ' || c == '/') break;	/* Break on end of segment */
		if (c == '.' || i >= ni) {
			if (ni != 8 || c != '.') break;
			i = 8; ni = 11;
			continue;
		}
#if PF_USE_LCC && defined(_EXCVT)
		if (c >= 0x80) c = cvt[c - 0x80];	/* To upper extended char (SBCS) */
#endif
#if IsDBCS1(c)
		if (IsDBCS1(c) && i < ni - 1) {	/* DBC 1st byte? */
			d = p[si++];				/* Get 2nd byte */
			sfn[i++] = c;
			sfn[i++] = d;
#else						/* Single byte code */
			if (PF_USE_LCC && IsLower(c)) c -= 0x20;	/* toupper */
			sfn[i++] = c;
#endif
	}
	*path = &p[si];						/* Rerurn pointer to the next segment */

	sfn[11] = (c <= ' ') ? 1 : 0;		/* Set last segment flag if end of path */

	return FR_OK;
}


/*-----------------------------------------------------------------------*/
/* Get file information from directory entry                             */
/*-----------------------------------------------------------------------*/
#if PF_USE_DIR
static void get_fileinfo (		/* No return code */
	DIR *dj,			/* Pointer to the directory object */
	uint8_t *dir,			/* 32-byte working buffer */
	FILINFO *fno	 	/* Pointer to store the file information */
)
{
	uint8_t i, c;
	char *p;


	p = fno->fname;
	if (dj->sect) {
		for (i = 0; i < 8; i++) {	/* Copy file name body */
			c = dir[i];
			if (c == ' ') break;
			if (c == 0x05) c = 0xE5;
			*p++ = c;
		}
		if (dir[8] != ' ') {		/* Copy file name extension */
			*p++ = '.';
			for (i = 8; i < 11; i++) {
				c = dir[i];
				if (c == ' ') break;
				*p++ = c;
			}
		}
		fno->fattrib = dir[DIR_Attr];				/* Attribute */
		fno->fsize = ld_dword(dir+DIR_FileSize);	/* Size */
		fno->fdate = ld_word(dir+DIR_WrtDate);		/* Date */
		fno->ftime = ld_word(dir+DIR_WrtTime);		/* Time */
	}
	*p = 0;
}
#endif /* PF_USE_DIR */

/*-----------------------------------------------------------------------*/
/* Follow a file path                                                    */
/*-----------------------------------------------------------------------*/
static FRESULT follow_path (	/* FR_OK(0): successful, !=0: error code */
	DIR *dj,			/* Directory object to return last directory and found object */
	uint8_t *dir,			/* 32-byte working buffer */
	const char *path	/* Full-path string to find a file or directory */
)
{
	FRESULT res;


	while (*path == ' ') path++;		/* Strip leading spaces */
	if (*path == '/') path++;			/* Strip heading separator if exist */
	dj->sclust = 0;						/* Set start directory (always root dir) */

	if ((uint8_t)*path < ' ') {			/* Null path means the root directory */
		res = dir_rewind(dj);
		dir[0] = 0;

	} else {							/* Follow path */
		for (;;) {
			res = create_name(dj, &path);	/* Get a segment */
			if (res != FR_OK) break;
			res = dir_find(dj, dir);		/* Find it */
			if (res != FR_OK) break;		/* Could not find the object */
			if (dj->fn[11]) break;			/* Last segment match. Function completed. */
			if (!(dir[DIR_Attr] & AM_DIR)) { /* Cannot follow path because it is a file */
				res = FR_NO_FILE; break;
			}
			dj->sclust = get_clust(dir);	/* Follow next */
		}
	}

	return res;
}

/*-----------------------------------------------------------------------*/
/* Check a sector if it is an FAT boot record                            */
/*-----------------------------------------------------------------------*/
static uint8_t check_fs (	/* 0:The FAT boot record, 1:Valid boot record but not an FAT, 2:Not a boot record, 3:Error */
	uint8_t *buf,	/* Working buffer */
	DWORD sect	/* Sector# (lba) to check if it is an FAT boot record or not */
)
{
	//if (disk_readp(buf, sect, 510, 2)) {	/* Read the boot record */
		//return 3;
	//}
	//if (ld_word(buf) != 0xAA55) {			/* Check record signature */
		//return 2;
	//}

	if (!_FS_32ONLY && !disk_readp(buf, sect, BS_FilSysType, 2) && ld_word(buf) == 0x4146) {	/* Check FAT12/16 */
		return 0;
	}
	if (PF_FS_FAT32 && !disk_readp(buf, sect, BS_FilSysType32, 2) && ld_word(buf) == 0x4146) {	/* Check FAT32 */
		return 0;
	}
	return 1;
}

//========================================================================
// 描述: 挂载/卸载本地驱动器.
// 参数: 得到的驱动器设备.
// 返回: none.
//========================================================================
FRESULT pf_mount (
	FATFS *fs		/* Pointer to new file system object */
)
{
	uint8_t i;
	uint8_t fmt, buf[36];
	DWORD bsect, fsize, tsect, mclst;

	FatFs = 0;
	i = 0;
	while(1){
		if (disk_initialize() & STA_NOINIT) {	/* Check if the drive is ready or not */
			i++;
			if(i>100)
			{
				return FR_NOT_READY;
			}
		}else{
			break;
		}
	}

	/* Search FAT partition on the drive */
	bsect = 0;
	fmt = check_fs(buf, bsect);			/* Check sector 0 as an SFD format */
	if (fmt == 1) {						/* Not an FAT boot record, it may be FDISK format */
		/* Check a partition listed in top of the partition table */
		if (disk_readp(buf, bsect, MBR_Table, 16)) {	/* 1st partition entry */
			fmt = 3;
		} else {
			if (buf[4]) {					/* Is the partition existing? */
				bsect = ld_dword(&buf[8]);	/* Partition offset in LBA */
				fmt = check_fs(buf, bsect);	/* Check the partition */
			}
		}
	}
	if (fmt == 3) return FR_DISK_ERR;
	if (fmt) return FR_NO_FILESYSTEM;	/* No valid FAT patition is found */

	/* Initialize the file system object */
	if (disk_readp(buf, bsect, 13, sizeof (buf))) return FR_DISK_ERR;

	fsize = ld_word(buf+BPB_FATSz16-13);				/* Number of sectors per FAT */
	if (!fsize) fsize = ld_dword(buf+BPB_FATSz32-13);

	fsize *= buf[BPB_NumFATs-13];						/* Number of sectors in FAT area */
	fs->fatbase = bsect + ld_word(buf+BPB_RsvdSecCnt-13); /* FAT start sector (lba) */
	fs->csize = buf[BPB_SecPerClus-13];					/* Number of sectors per cluster */
	fs->n_rootdir = ld_word(buf+BPB_RootEntCnt-13);		/* Nmuber of root directory entries */
	tsect = ld_word(buf+BPB_TotSec16-13);				/* Number of sectors on the file system */
	if (!tsect) tsect = ld_dword(buf+BPB_TotSec32-13);
	mclst = (tsect						/* Last cluster# + 1 */
		- ld_word(buf+BPB_RsvdSecCnt-13) - fsize - fs->n_rootdir / 16
		) / fs->csize + 2;
	fs->n_fatent = (CLUST)mclst;

	fmt = 0;							/* Determine the FAT sub type */
	if (PF_FS_FAT12 && mclst < 0xFF7) fmt = FS_FAT12;
	if (PF_FS_FAT16 && mclst >= 0xFF8 && mclst < 0xFFF7) fmt = FS_FAT16;
	if (PF_FS_FAT32 && mclst >= 0xFFF7) fmt = FS_FAT32;
	if (!fmt) return FR_NO_FILESYSTEM;
	fs->fs_type = fmt;

	if (_FS_32ONLY || (PF_FS_FAT32 && fmt == FS_FAT32)) {
		fs->dirbase = ld_dword(buf+(BPB_RootClus-13));	/* Root directory start cluster */
	} else {
		fs->dirbase = fs->fatbase + fsize;				/* Root directory start sector (lba) */
	}
	fs->database = fs->fatbase + fsize + fs->n_rootdir / 16;	/* Data start sector (lba) */

	fs->flag = 0;
	FatFs = fs;

	return FR_OK;
}

//========================================================================
// 描述: 打开文件.
// 参数: 路径.
// 返回: none.
//========================================================================
FRESULT pf_open (
	const char *path	/* Pointer to the file name */
)
{
	FRESULT res;
	DIR dj;
	uint8_t sp[12], dir[32];
	FATFS *fs = FatFs;


	if (!fs) return FR_NOT_ENABLED;		/* Check file system */

	fs->flag = 0;
	dj.fn = sp;
	res = follow_path(&dj, dir, path);	/* Follow the file path */
	if (res != FR_OK) return res;		/* Follow failed */
	if (!dir[0] || (dir[DIR_Attr] & AM_DIR)) return FR_NO_FILE;	/* It is a directory */

	fs->org_clust = get_clust(dir);		/* File start cluster */
	fs->fsize = ld_dword(dir+DIR_FileSize);	/* File size */
	fs->fptr = 0;						/* File pointer */
	fs->flag = FA_OPENED;

	return FR_OK;
}

//========================================================================
// 描述: 读取文件.
// 参数: buff:读取缓冲指针
//		 btr: 读取的长度
//		 br:已经读取的数量指针
// 返回: none.
//========================================================================
#if PF_USE_READ
FRESULT pf_read (
	void* buff,		/* Pointer to the read buffer (NULL:Forward data to the stream)*/
	UINT btr,		/* Number of bytes to read */
	UINT* br		/* Pointer to number of bytes read */
)
{
	DRESULT dr;
	CLUST clst;
	DWORD sect, remain;
	UINT rcnt;
	uint8_t cs, *rbuff = (uint8_t*)buff;
	FATFS *fs = FatFs;


	*br = 0;
	if (!fs) return FR_NOT_ENABLED;		/* Check file system */
	if (!(fs->flag & FA_OPENED)) return FR_NOT_OPENED;	/* Check if opened */

	remain = fs->fsize - fs->fptr;
	if (btr > remain) btr = (UINT)remain;			/* Truncate btr by remaining bytes */

	while (btr)	{									/* Repeat until all data transferred */
		if ((fs->fptr % 512) == 0) {				/* On the sector boundary? */
			cs = (uint8_t)(fs->fptr / 512 & (fs->csize - 1));	/* Sector offset in the cluster */
			if (!cs) {								/* On the cluster boundary? */
				if (fs->fptr == 0) {				/* On the top of the file? */
					clst = fs->org_clust;
				} else {
					clst = get_fat(fs->curr_clust);
				}
				if (clst <= 1) ABORT(FR_DISK_ERR);
				fs->curr_clust = clst;				/* Update current cluster */
			}
			sect = clust2sect(fs->curr_clust);		/* Get current sector */
			if (!sect) ABORT(FR_DISK_ERR);
			fs->dsect = sect + cs;
		}
		rcnt = 512 - (UINT)fs->fptr % 512;			/* Get partial sector data from sector buffer */
		if (rcnt > btr) rcnt = btr;
		dr = disk_readp(rbuff, fs->dsect, (UINT)fs->fptr % 512, rcnt);
		if (dr) ABORT(FR_DISK_ERR);
		fs->fptr += rcnt;							/* Advances file read pointer */
		btr -= rcnt; *br += rcnt;					/* Update read counter */
		if (rbuff) rbuff += rcnt;					/* Advances the data pointer if destination is memory */
	}

	return FR_OK;
}
#endif

//========================================================================
// 描述: 写入文件.
// 参数: buff:写入缓冲指针
//		 btw: 写入的长度
//		 bw:已经写入的数量指针
// 返回: none.
//========================================================================
#if PF_USE_WRITE

FRESULT pf_write (
	const void* buff,	/* Pointer to the data to be written */
	UINT btw,			/* Number of bytes to write (0:Finalize the current write operation) */
	UINT* bw			/* Pointer to number of bytes written */
)
{
	CLUST clst;
	DWORD sect, remain;
	const uint8_t *p = (const uint8_t *) buff;
	uint8_t cs;
	UINT wcnt;
	FATFS *fs = FatFs;


	*bw = 0;
	if (!fs) return FR_NOT_ENABLED;		/* Check file system */
	if (!(fs->flag & FA_OPENED)) return FR_NOT_OPENED;	/* Check if opened */

	if (!btw) {		/* Finalize request */
		if ((fs->flag & FA__WIP) && disk_writep(0, 0)) ABORT(FR_DISK_ERR);
		fs->flag &= ~FA__WIP;
		return FR_OK;
	} else {		/* Write data request */
		if (!(fs->flag & FA__WIP)) {	/* Round-down fptr to the sector boundary */
			fs->fptr &= 0xFFFFFE00;
		}
	}
	remain = fs->fsize - fs->fptr;
	if (btw > remain) btw = (UINT)remain;			/* Truncate btw by remaining bytes */

	while (btw)	{									/* Repeat until all data transferred */
		if ((UINT)fs->fptr % 512 == 0) {			/* On the sector boundary? */
			cs = (uint8_t)(fs->fptr / 512 & (fs->csize - 1));	/* Sector offset in the cluster */
			if (!cs) {								/* On the cluster boundary? */
				if (fs->fptr == 0) {				/* On the top of the file? */
					clst = fs->org_clust;
				} else {
					clst = get_fat(fs->curr_clust);
				}
				if (clst <= 1) ABORT(FR_DISK_ERR);
				fs->curr_clust = clst;				/* Update current cluster */
			}
			sect = clust2sect(fs->curr_clust);		/* Get current sector */
			if (!sect) ABORT(FR_DISK_ERR);
			fs->dsect = sect + cs;
			if (disk_writep(0, fs->dsect)) ABORT(FR_DISK_ERR);	/* Initiate a sector write operation */
			fs->flag |= FA__WIP;
		}
		wcnt = 512 - (UINT)fs->fptr % 512;			/* Number of bytes to write to the sector */
		if (wcnt > btw) wcnt = btw;
		if (disk_writep(p, wcnt)) ABORT(FR_DISK_ERR);	/* Send data to the sector */
		fs->fptr += wcnt; p += wcnt;				/* Update pointers and counters */
		btw -= wcnt; *bw += wcnt;
		if ((UINT)fs->fptr % 512 == 0) {
			if (disk_writep(0, 0)) ABORT(FR_DISK_ERR);	/* Finalize the currtent secter write operation */
			fs->flag &= ~FA__WIP;
		}
	}

	return FR_OK;
}
#endif

//========================================================================
// 描述: 设置读取文件指针偏移.
// 参数: 偏移量
// 返回: none.
//========================================================================
#if PF_USE_LSEEK
FRESULT pf_lseek (
	DWORD ofs		/* File pointer from top of file */
)
{
	CLUST clst;
	DWORD bcs, sect, ifptr;
	FATFS *fs = FatFs;


	if (!fs) return FR_NOT_ENABLED;		/* Check file system */
	if (!(fs->flag & FA_OPENED)) return FR_NOT_OPENED;	/* Check if opened */

	if (ofs > fs->fsize) ofs = fs->fsize;	/* Clip offset with the file size */
	ifptr = fs->fptr;
	fs->fptr = 0;
	if (ofs > 0) {
		bcs = (DWORD)fs->csize * 512;		/* Cluster size (byte) */
		if (ifptr > 0 &&
			(ofs - 1) / bcs >= (ifptr - 1) / bcs) {	/* When seek to same or following cluster, */
			fs->fptr = (ifptr - 1) & ~(bcs - 1);	/* start from the current cluster */
			ofs -= fs->fptr;
			clst = fs->curr_clust;
		} else {							/* When seek to back cluster, */
			clst = fs->org_clust;			/* start from the first cluster */
			fs->curr_clust = clst;
		}
		while (ofs > bcs) {				/* Cluster following loop */
			clst = get_fat(clst);		/* Follow cluster chain */
			if (clst <= 1 || clst >= fs->n_fatent) ABORT(FR_DISK_ERR);
			fs->curr_clust = clst;
			fs->fptr += bcs;
			ofs -= bcs;
		}
		fs->fptr += ofs;
		sect = clust2sect(clst);		/* Current sector */
		if (!sect) ABORT(FR_DISK_ERR);
		fs->dsect = sect + (fs->fptr / 512 & (fs->csize - 1));
	}

	return FR_OK;
}
#endif

//========================================================================
// 描述: 打开文件目录.
// 参数: dj:要创建的目录对象的指针
//		 path:目录路径
// 返回: none.
//========================================================================
#if PF_USE_DIR
FRESULT pf_opendir (
	DIR *dj,			/* Pointer to directory object to create */
	const char *path	/* Pointer to the directory path */
)
{
	FRESULT res;
	uint8_t sp[12], dir[32];
	FATFS *fs = FatFs;


	if (!fs) {				/* Check file system */
		res = FR_NOT_ENABLED;
	} else {
		dj->fn = sp;
		res = follow_path(dj, dir, path);		/* Follow the path to the directory */
		if (res == FR_OK) {						/* Follow completed */
			if (dir[0]) {						/* It is not the root dir */
				if (dir[DIR_Attr] & AM_DIR) {	/* The object is a directory */
					dj->sclust = get_clust(dir);
				} else {							/* The object is not a directory */
					res = FR_NO_FILE;
				}
			}
			if (res == FR_OK) {
				res = dir_rewind(dj);			/* Rewind dir */
			}
		}
	}

	return res;
}

//========================================================================
// 描述: 读取目录文件信息.
// 参数: dj:要读取的目录对象的指针
//		 fno:返回的文件信息
// 返回: none.
//========================================================================
FRESULT pf_readdir (
	DIR *dj,			/* Pointer to the open directory object */
	FILINFO *fno		/* Pointer to file information to return */
)
{
	FRESULT res;
	uint8_t sp[12], dir[32];
	FATFS *fs = FatFs;


	if (!fs) {				/* Check file system */
		res = FR_NOT_ENABLED;
	} else {
		dj->fn = sp;
		if (!fno) {
			res = dir_rewind(dj);
		} else {
			res = dir_read(dj, dir);	/* Get current directory item */
			if (res == FR_NO_FILE) res = FR_OK;
			if (res == FR_OK) {				/* A valid entry is found */
				get_fileinfo(dj, dir, fno);	/* Get the object information */
				res = dir_next(dj);			/* Increment read index for next */
				if (res == FR_NO_FILE) res = FR_OK;
			}
		}
	}
	return res;
}

#endif /* PF_USE_DIR */

#endif /* _PFATFS */
