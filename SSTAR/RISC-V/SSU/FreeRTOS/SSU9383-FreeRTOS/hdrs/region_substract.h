/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/

#ifndef _REGIONSUBSTRACT_H_
#define _REGIONSUBSTRACT_H_

typedef int BOOL;
typedef unsigned char BYTE;
typedef void *PBLOCKHEAP;

/**
 * A rectangle defined by coordinates of corners.
 *
 * \note The lower-right corner does not belong to the rectangle,
 * i.e. the bottom horizontal line and the right vertical line are excluded
 * from the retangle.
 *
 * \sa PRECT, GAL_Rect
 */
typedef struct _RECT
{
    /**
     * The x coordinate of the upper-left corner of the rectangle.
     */
    int left;
    /**
     * The y coordinate of the upper-left corner of the rectangle.
     */
    int top;
    /**
     * The x coordinate of the lower-right corner of the rectangle.
     */
    int right;
    /**
     * The y coordinate of the lower-right corner of the rectangle.
     */
    int bottom;
} RECT;
/**
 * \var typedef RECT* PRECT
 * \brief Data type of the pointer to a RECT.
 *
 * \sa RECT
 */
typedef RECT* PRECT;

   /**
    * \defgroup region_fns Region operations
    *
    * As the name implies, a Region is simply an area, which is implemented as
    * a "y-x-banded" array of rectangles. To explain: Each Region is made up
    * of a certain number of rectangles sorted by y coordinate first,
    * and then by x coordinate.
    *
    * Whenever possible, bands will be merged together to cover a greater
    * vertical distance (and thus reduce the number of rectangles). Two bands
    * can be merged only if the bottom of one touches the top of the other and
    * they have rectangles in the same places (of the same width, of course).
    * This maintains the y-x-banding that's so nice to have...
    *
    * Example:
    *
    * \include region.c
    *
    * @{
    */

/**
 * Clipping rectangle structure.
 */
typedef struct _CLIPRECT
{
    /**
     * The clipping rectangle itself.
     */
    RECT rc;
    /**
     * The next clipping rectangle.
     */
    struct _CLIPRECT* next;
    /**
     * The previous clipping rectangle.
     */
    struct _CLIPRECT* prev;
} CLIPRECT;
typedef CLIPRECT* PCLIPRECT;

/* Clipping Region */
#define NULLREGION      0x00
#define SIMPLEREGION    0x01
#define COMPLEXREGION   0x02

/**
 * Clipping region structure, alos used for general regions.
 */
typedef struct _CLIPRGN
{
   /**
    * Type of the region, can be one of the following:
    *   - NULLREGION\n
    *     A null region.
    *   - SIMPLEREGION\n
    *     A simple region.
    *   - COMPLEXREGION\n
    *     A complex region.
    */
    BYTE            type;
   /**
    * Reserved for alignment.
    */
    BYTE            reserved[3];
   /**
    * The bounding rect of the region.
    */
    RECT            rcBound;
   /**
    * Head of the clipping rectangle list.
    */
    PCLIPRECT       head;
   /**
    * Tail of the clipping rectangle list.
    */
    PCLIPRECT       tail;
   /**
    * The private block data heap used to allocate clipping rectangles.
    * \sa BLOCKHEAP
    */
    PBLOCKHEAP      heap;
} CLIPRGN;

/**
 * \var typedef CLIPRGN* PCLIPRGN
 * \brief Data type of the pointer to a CLIPRGN.
 *
 * \sa CLIPRGN
 */
typedef CLIPRGN* PCLIPRGN;

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define region_for_each_rect(rect, rgn) \
    for(rect=((rgn)->head)?(&((rgn)->head->rc)):NULL; \
          rect; \
          rect=(container_of(rect, CLIPRECT, rc)->next)?&(container_of(rect, CLIPRECT, rc)->next->rc):NULL)

void dbg_dumpRegion (CLIPRGN* region);
BOOL IsEmptyClipRgn (const CLIPRGN* pRgn);
void EmptyClipRgn (PCLIPRGN pRgn);
void InitClipRgn (PCLIPRGN pRgn, PBLOCKHEAP heap);
BOOL AddClipRect (PCLIPRGN region, const RECT *rect);
BOOL SubtractRegion (CLIPRGN *rgnD, const CLIPRGN *rgnM, const CLIPRGN *rgnS);
int region_substract_init(void);
int region_substract_deinit(void);

#endif
