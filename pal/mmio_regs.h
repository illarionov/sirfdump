
#ifndef _MMIO_H
#define _MMIO_H

struct regname {
	unsigned long addr;
	unsigned char name[32];
};

static struct regname regnames[] = {
	{ 0x40D00000, "ICIP" },
	{ 0x40D00004, "ICMR" },
	{ 0x40D00008, "ICLR" },
	{ 0x40D0000C, "ICFP" },
	{ 0x40D00010, "ICPR" },
	{ 0x40D00010, "ICCR" },

	{ 0x40E00000, "GPLR0" },
	{ 0x40E00004, "GPLR1" },
	{ 0x40E00008, "GPLR2" },

	{ 0x40E0000C, "GPDR0" },
	{ 0x40E00010, "GPDR1" },
	{ 0x40E00014, "GPDR2" },

	{ 0x40E00018, "GPSR0" },
	{ 0x40E0001C, "GPSR1" },
	{ 0x40E00020, "GPSR2" },

	{ 0x40E00024, "GPCR0" },
	{ 0x40E00028, "GPCR1" },
	{ 0x40E0002C, "GPCR2" },

	{ 0x40E00030, "GRER0" },
	{ 0x40E00034, "GRER1" },
	{ 0x40E00038, "GRER2" },

	{ 0x40E0003C, "GFER0" },
	{ 0x40E00040, "GFER1" },
	{ 0x40E00044, "GFER2" },

	{ 0x40E00048, "GEDR0" },
	{ 0x40E0004C, "GEDR1" },
	{ 0x40E00050, "GEDR2" },
};

#endif
