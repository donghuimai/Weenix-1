#include "errno.h"
#include "globals.h"

#include "util/string.h"
#include "util/debug.h"

#include "mm/mm.h"
#include "mm/page.h"
#include "mm/mmobj.h"
#include "mm/kmalloc.h"
#include "mm/pframe.h"

#include "drivers/bytedev.h"

#include "vm/anon.h"

#include "fs/vnode.h"

static int null_read(bytedev_t *dev, int offset, void *buf, int count);
static int null_write(bytedev_t *dev, int offset, const void *buf, int count);

static int zero_read(bytedev_t *dev, int offset, void *buf, int count);
static int zero_mmap(vnode_t *file, vmarea_t *vma, mmobj_t **ret);

bytedev_ops_t null_dev_ops = {
        null_read,
        null_write,
        NULL,
        NULL,
        NULL,
        NULL
};

bytedev_ops_t zero_dev_ops = {
        zero_read,
        null_write,
        zero_mmap,
        NULL,
        NULL,
        NULL
};

/*
 * The byte device code needs to know about these mem devices, so create
 * bytedev_t's for null and zero, fill them in, and register them.
 */
void
memdevs_init()
{
    dbg(DBG_INIT, "memdevs_init is called\n");
    bytedev_t *null_dev = (bytedev_t *)kmalloc(sizeof(bytedev_t));
    null_dev->cd_id = MEM_NULL_DEVID;
    null_dev->cd_ops = &null_dev_ops;
    bytedev_register(null_dev);

    bytedev_t *zero_dev = (bytedev_t *)kmalloc(sizeof(bytedev_t));
    zero_dev->cd_id = MEM_ZERO_DEVID;
    zero_dev->cd_ops = &zero_dev_ops;
    bytedev_register(zero_dev);
        /*NOT_YET_IMPLEMENTED("DRIVERS: memdevs_init");*/
}

/**
 * Reads a given number of bytes from the null device into a
 * buffer. Any read performed on the null device should read 0 bytes.
 *
 * @param dev the null device
 * @param offset the offset to read from. Should be ignored
 * @param buf the buffer to read into
 * @param count the maximum number of bytes to read
 * @return the number of bytes read, which should be 0
 */
static int
null_read(bytedev_t *dev, int offset, void *buf, int count)
{
    char *buff = (char *)buf;
    buff[0] = '\x4';
    return 0;
        /*NOT_YET_IMPLEMENTED("DRIVERS: null_read");*/
}

/**
 * Writes a given number of bytes to the null device from a
 * buffer. Writing to the null device should _ALWAYS_ be successful
 * and write the maximum number of bytes.
 *
 * @param dev the null device
 * @param offset the offset to write to. Should be ignored
 * @param buf buffer to read from
 * @param count the maximum number of bytes to write
 * @return the number of bytes written, which should be count
 */
static int
null_write(bytedev_t *dev, int offset, const void *buf, int count)
{
    char *buff = (char *)buf;
    int i = 0;
    for (i = 0 ; i < count; i++) {
        if (buff[i] == '\0') {
            break;
        }
    }
    return i;
        /*NOT_YET_IMPLEMENTED("DRIVERS: null_write");*/
}

/**
 * Reads a given number of bytes from the zero device into a
 * buffer. Any read from the zero device should be a series of zeros.
 *
 * @param dev the zero device
 * @param offset the offset to read from. Should be ignored
 * @param buf the buffer to write to
 * @param count the maximum number of bytes to read
 * @return the number of bytes read. Should always read the maximum
 * number of bytes
 */
static int
zero_read(bytedev_t *dev, int offset, void *buf, int count)
{
    char *buff = (char *)buf;
    int i = 0;
    for (i = 0 ; i < count; i++) {
        buff[i] = '\0';
    }
    return count;
        /*NOT_YET_IMPLEMENTED("DRIVERS: zero_read");*/
}

/* Don't worry about these until VM. Once you're there, they shouldn't be hard. */

static int
zero_mmap(vnode_t *file, vmarea_t *vma, mmobj_t **ret)
{
    KASSERT(file);
    KASSERT(vma);

    /*How to watch the refcount?*/
    KASSERT(file->vn_mmobj.mmo_refcount >= 0);
    KASSERT(file->vn_mmobj.mmo_nrespages >= 0);

    *ret = anon_create();
    if (*ret == NULL) {
        return -ENOMEM;
    }

    return 0;
        /*NOT_YET_IMPLEMENTED("VM: zero_mmap");*/
        /*return -1;*/
}
