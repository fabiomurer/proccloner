#include "writer.h"
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>

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