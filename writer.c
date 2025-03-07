#include "writer.h"
#include "utility.h"

#include <errno.h>
#include <linux/limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>



void write_mapping_file(u_int8_t* buffer, u_int64_t len, u_int64_t start, u_int64_t end) {
    char out_filename[PATH_MAX];
    snprintf(out_filename, sizeof(out_filename), "/tmp/%lx-%lx.mem", start, end);

    FILE *out_file = fopen(out_filename, "wb");
    if (!out_file) {
        fprintf(stderr, "Failed to open output file %s: %s\n", out_filename, strerror(errno));
    }

    // Write the segment data to the file.
    fwrite(buffer, 1, len, out_file);
    fclose(out_file);
}

int perms_to_prot(char* perms) {
    int prot = 0;
    if (perms[0] == 'r') prot |= PROT_READ;
    if (perms[1] == 'w') prot |= PROT_WRITE;
    if (perms[2] == 'x') prot |= PROT_EXEC;

    return prot;
}

void write_mapping_mem(u_int8_t* buffer, u_int64_t len, u_int64_t start, u_int64_t end, char* perms, char* pathname) {
    int flags = MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE;
    int prot  = perms_to_prot(perms);

    void* p = mmap((void*)start, len, PROT_READ | PROT_WRITE | PROT_EXEC, flags, -1, 0);

    if (p == MAP_FAILED)  {
        fprintf(stderr, "cannot mmap segment %lx-%lx pathname: %s, error: %s\n", start, end, pathname, strerror(errno));
        return;
    }
    if (p != (void*)start) {
        fprintf(stderr, "not able to mmap at right address, %lx-%lx, pathname: %s\n", start, end, pathname);
        exit(EXIT_FAILURE);
    }

    memcpy(p, buffer, len);

    if (mprotect(p, len, prot) == -1) {
        error_and_exit("mprotect");
    }

    printf("Mappend segment 0x%lx-0x%lx (%ld bytes)\n", start, end, len);
}