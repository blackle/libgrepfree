#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <signal.h>

static void (*real_free)(void*)=NULL;
static char* search_string=NULL;
static int search_string_len=0;

static void mtrace_init(void)
{
    search_string = getenv("MEM_SEARCH_STRING");
    if (search_string == NULL) {
        fprintf(stderr, "environment variable MEM_SEARCH_STRING missing\n");
        exit(1);
    }
    search_string_len = strlen(search_string);

    real_free = dlsym(RTLD_NEXT, "free");
    if (NULL == real_free) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
        exit(1);
    }
}

void free(void * ptr)
{
    if (ptr == NULL) {
        return;
    }

    if(real_free==NULL) {
        mtrace_init();
    }

    size_t size = malloc_usable_size(ptr);

    char * chptr = (char *)ptr;
    char * curr = chptr;
    while (curr + search_string_len < chptr + size) {
        int i;
        for (i = 0; i < search_string_len; i++) {
            if (curr[i] != search_string[i]) {
                break;
            }
        }
        if (i == search_string_len) {
            raise(SIGTRAP);
        }
        curr++;
    }

    real_free(ptr);
}
