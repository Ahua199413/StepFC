#include "sfc_famicom.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Load Default ROM
static sfc_ecode sfc_load_default_rom(void*, sfc_rom_info_t*);
// Free Default ROM
static sfc_ecode sfc_free_default_rom(void*, sfc_rom_info_t*);

// Just Declare a function pointer
typedef void(*sfc_funcptr_t)();

// StepFC: 初始化famicom
sfc_ecode sfc_famicom_init(sfc_famicom_t* 			famicom, 
							void* 					argument, 
							const sfc_interface_t* 	interfaces) 
{
	assert(famicom && "bad famicom");
	famicom->argument = argument;
	// Default Interface
	famicom->interfaces.load_rom = sfc_load_default_rom;
	famicom->interfaces.free_rom = sfc_free_default_rom;
	// 
	memset(&famicom->rom_info, 0, sizeof(famicom->rom_info));
	// 
	if (interfaces) {
		const int count = sizeof(*interfaces) / sizeof(interfaces->load_rom);
		sfc_funcptr_t* const func_src = (sfc_funcptr_t*)interfaces;
		sfc_funcptr_t* const func_des = (sfc_funcptr_t*)&famicom->interfaces;
		for (int i = 0; i != count; ++i) 
			if (func_src[i]) 
				func_des[i] = func_src[i];
	}
	// Load test ROM
	return famicom->interfaces.load_rom(argument, &famicom->rom_info);
	return SFC_ERROR_OK;
}

// StepFC: Uninit famicom
void sfc_famicom_uninit(sfc_famicom_t* famicom) {
    // Free ROM
    famicom->interfaces.free_rom(famicom->argument, &famicom->rom_info);
}

// Load Default test ROM
sfc_ecode sfc_load_default_rom(void* arg, sfc_rom_info_t* info) {
    assert(info->data_prgrom == NULL && "FREE FIRST");
    FILE* const file = fopen("nestest.nes", "rb");
    // Not Found
    if (!file) 
		return SFC_ERROR_FILE_NOT_FOUND;
    sfc_ecode code = SFC_ERROR_ILLEGAL_FILE;
    // read header
    sfc_nes_header_t nes_header;
    if (fread(&nes_header, sizeof(nes_header), 1, file)) {
        // 4 Byte at beginnig
        union this_union{ 
			uint32_t 	u32; 
			uint8_t 	id[4]; 
		};
        this_union.id[0] = 'N';
        this_union.id[1] = 'E';
        this_union.id[2] = 'S';
        this_union.id[3] = '\x1A';
        // Compare
        if (this_union.u32 == nes_header.id) {
            const size_t size1 = 16 * 1024 * nes_header.count_prgrom16kb;
            const size_t size2 =  8 * 1024 * nes_header.count_chrrom8kb;
            uint8_t* const ptr = (uint8_t*)malloc(size1 + size2);

            if (ptr) {
                code = SFC_ERROR_OK;
                // Skip Trainer
                if (nes_header.control1 & SFC_NES_TRAINER) 
					fseek(file, 512, SEEK_CUR);
                // 
                fread(ptr, size1 + size2, 1, file);

                // 
                info->data_prgrom = ptr;
                info->data_chrrom = ptr + size1;
                info->count_prgrom16kb = nes_header.count_prgrom16kb;
                info->count_chrrom8kb = nes_header.count_chrrom8kb;
                info->mapper_number = (nes_header.control1 >> 4) 
										| (nes_header.control2 & 0xF0);
                info->vmirroring    = (nes_header.control1 & SFC_NES_VMIRROR) > 0;
                info->four_screen   = (nes_header.control1 & SFC_NES_4SCREEN) > 0;
                info->save_ram      = (nes_header.control1 & SFC_NES_SAVERAM) > 0;
                assert(!(nes_header.control1 & SFC_NES_TRAINER) && "unsupported");
                assert(!(nes_header.control2 & SFC_NES_VS_UNISYSTEM) && "unsupported");
                assert(!(nes_header.control2 & SFC_NES_Playchoice10) && "unsupported");
            }
            // malloc failed
            else code = SFC_ERROR_OUT_OF_MEMORY;
        }
    }
    fclose(file);
    return code;
}

// Free Default test ROM
sfc_ecode sfc_free_default_rom(void* arg, sfc_rom_info_t* info) {
	
    free(info->data_prgrom);
    info->data_prgrom = NULL;

    return SFC_ERROR_OK;
}