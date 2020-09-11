#ifndef LAB0_COMPLETION_H
#define LAB0_COMPLETION_H

#include <stddef.h>
#include <string.h>
#include "linenoise/linenoise.h"
#define HISTORY_FILE ".qtest.history.txt"

void completion(const char *buf, linenoiseCompletions *lc);
char *hints(const char *buf, int *color, int *bold);

#endif