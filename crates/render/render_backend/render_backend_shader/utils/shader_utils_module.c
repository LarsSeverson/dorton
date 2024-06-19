#include "shader_utils_module.h"
#include "logger.h"

#include <stdio.h>

DResult read_shader_file(DArray *shader_code, DString *file_path)
{
    FILE *file = fopen(file_path->string, "rb");
    if (file == NULL)
    {
        DERROR("Failed to open file at: %s", file_path);
        return D_ERROR;
    }

    fseek(file, 0, SEEK_END);
    u32 file_size = (u32)ftell(file);
    rewind(file);


    darray_reserve(shader_code, char, file_size);

    u32 bytes_read = fread((u32 *)darray_data(shader_code), 1, file_size, file);

    fclose(file);
    return D_SUCCESS;
}