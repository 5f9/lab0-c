#include "completion.h"

void completion(const char *buf, linenoiseCompletions *lc)
{
    switch (buf[0]) {
    case 'f':
        linenoiseAddCompletion(lc, "free");
        break;
    case 'h':
        linenoiseAddCompletion(lc, "help");
        break;
    case 'i':
        linenoiseAddCompletion(lc, "ih");
        linenoiseAddCompletion(lc, "it");
        break;
    case 'l':
        linenoiseAddCompletion(lc, "log");
        break;
    case 'n':
        switch (buf[1]) {
        case 'e':
            linenoiseAddCompletion(lc, "new");
            break;
        case 's':
            linenoiseAddCompletion(lc, "nsort");
            break;
        default:
            linenoiseAddCompletion(lc, "new");
            linenoiseAddCompletion(lc, "nsort");
            break;
        }
        break;
    case 'o':
        linenoiseAddCompletion(lc, "option echo");
        linenoiseAddCompletion(lc, "option error");
        linenoiseAddCompletion(lc, "option fail");
        linenoiseAddCompletion(lc, "option length");
        linenoiseAddCompletion(lc, "option malloc");
        linenoiseAddCompletion(lc, "option simulation");
        linenoiseAddCompletion(lc, "option verbose");
        break;
    case 'q':
        linenoiseAddCompletion(lc, "quit");
        break;
    case 'r':
        switch (buf[1]) {
        case 'e':
            linenoiseAddCompletion(lc, "reverse");
            break;
        case 'h':
            linenoiseAddCompletion(lc, "rh");
            linenoiseAddCompletion(lc, "rhq");
            break;
        default:
            linenoiseAddCompletion(lc, "reverse");
            linenoiseAddCompletion(lc, "rh");
            linenoiseAddCompletion(lc, "rhq");
            break;
        }
        break;
    case 's':
        switch (buf[1]) {
        case 'h':
            linenoiseAddCompletion(lc, "show");
            break;
        case 'i':
            linenoiseAddCompletion(lc, "size");
            break;
        case 'o':
            linenoiseAddCompletion(lc, "sort");
            linenoiseAddCompletion(lc, "source");
            break;
        default:
            linenoiseAddCompletion(lc, "show");
            linenoiseAddCompletion(lc, "size");
            linenoiseAddCompletion(lc, "sort");
            linenoiseAddCompletion(lc, "source");
            break;
        }
        break;
    case 't':
        linenoiseAddCompletion(lc, "time");
        break;
    default:
        break;
    }
}

char *hints(const char *buf, int *color, int *bold)
{
    if (!strcmp(buf, "sort") || !strcmp(buf, "nsort")) {
        *color = 35;
        *bold = 0;
        return " ci desc";
    }
    return NULL;
}