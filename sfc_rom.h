#include <stdint.h>

// ROM訊息
typedef struct {
	uint8_t		*data_prgrom;		// program read-only rom
	uint8_t		*data_chrrom;		// character read-only rom
	uint32_t	count_prgrom16kb;	// Size of PRG ROM in 16 KB units
	uint32_t	count_chrrom8kb;	// Size of CHR ROM in 8  KB units
	uint8_t		mapper_number;		// Mapper
	uint8_t		vmirroring;			// Vertical Mirroring or Horizon
	uint8_t		four_screen;		// 
	uint8_t		save_ram;			// 
	uint8_t		reserved[4];		// for Aligning
} sfc_rom_info_t;

// NES Header
//文件头:
// 0-3: string    "NES"<EOF>
// 	 4: byte      以16KB為一单位，PRG-ROM的數量
//   5: byte      以 8KB作为单位，CHR-ROM的數量
//   6: bitfield  Flags 6
//   7: bitfield  Flags 7
//8-15: byte      Reserved
typedef struct {
	uint32_t	id;
	uint8_t		count_prgrom16kb;
	uint8_t		count_chrrom8kb;
	uint8_t		control1;
	uint8_t		control2;
	uint8_t		reserved[8];
} sfc_nes_header_t;


/// ROM control BYTE 1
enum {
    SFC_NES_VMIRROR = 0x01, 
    SFC_NES_SAVERAM = 0x02,
    SFC_NES_TRAINER = 0x04,
    SFC_NES_4SCREEN = 0x08
};

// ROM control BYTE 2
enum { 
    SFC_NES_VS_UNISYSTEM  = 0x01,
    SFC_NES_Playchoice10 = 0x02
};
