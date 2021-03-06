#include "sfc_famicom.h"
#include <stdio.h>

int main() {
    sfc_famicom_t famicom;
    sfc_famicom_init(&famicom, NULL, NULL);
    printf(
        "ROM: PRG-ROM: %d x 16kb   CHR-ROM %d x 8kb   Mapper: %03d\n", 
        (int)famicom.rom_info.count_prgrom16kb,
        (int)famicom.rom_info.count_chrrom8kb,
        (int)famicom.rom_info.mapper_number
    );
    getchar();
    sfc_famicom_uninit(&famicom);
    return 0;
}