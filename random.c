#include "random.h"
#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

/* shameless stolen from ebacs */
void randombytes(uint8_t *x, size_t xlen)
{
    ssize_t i;
    static int fd = -1;

    ssize_t len = (ssize_t) xlen;
    assert(len >= 0);
    if (fd == -1) {
        for (;;) {
            fd = open("/dev/urandom", O_RDONLY);
            if (fd != -1)
                break;
            sleep(1);
        }
    }

    while (len > 0) {
        if (len < 1048576)
            i = len;
        else
            i = 1048576;

        i = read(fd, x, (size_t) i);
        if (i < 1) {
            sleep(1);
            continue;
        }

        x += i;
        len -= i;
    }
}

uint8_t randombit(void)
{
    uint8_t ret = 0;
    randombytes(&ret, 1);
    return (ret & 1);
}
