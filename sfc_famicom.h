#include "sfc_rom.h"
#include "sfc_code.h"

// Interface
typedef struct {
	// Load
	sfc_ecode(*load_rom)(void*, sfc_rom_info_t*);
	// Free
	sfc_ecode(*free_rom)(void*, sfc_rom_info_t*);
} sfc_interface_t;

//	Emulator 主體
typedef struct {
	// Parameter
	void*				argument;
	// Interface
	sfc_interface_t		interfaces;
	// Rom information
	sfc_rom_info_t		rom_info;
	
} sfc_famicom_t;

//	Init
sfc_ecode sfc_famicom_init(
	sfc_famicom_t*			famicom,
	void*					argument,
	const sfc_interface_t* 	interfaces
);

// Uninit
void sfc_famicom_uninit(sfc_famicom_t*);