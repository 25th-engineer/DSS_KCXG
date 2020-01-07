/*
 support 512/page NAND Flash only
*/
#include <string.h>

#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "Nand.h"

//suppport boot params
#define	GLOBAL_PARAMS
#include "bootpara.h"

#define	EnNandFlash()	(rNFCONT |= 1)
#define	DsNandFlash()	(rNFCONT &= ~1)
#define	NFChipEn()		(rNFCONT &= ~(1<<1))
#define	NFChipDs()		(rNFCONT |= (1<<1))
#define	InitEcc()		(rNFCONT |= (1<<4))
#define	MEccUnlock()	(rNFCONT &= ~(1<<5))
#define	MEccLock()		(rNFCONT |= (1<<5))
#define	SEccUnlock()	(rNFCONT &= ~(1<<6))
#define	SEccLock()		(rNFCONT |= (1<<6))

#define	WrNFDat8(dat)	(rNFDATA8 = (dat))
#define	WrNFDat32(dat)	(rNFDATA = (dat))
#define	RdNFDat8()		(rNFDATA8)	//byte access
#define	RdNFDat32()		(rNFDATA)	//word access

#define	WrNFCmd(cmd)	(rNFCMD = (cmd))
#define	WrNFAddr(addr)	(rNFADDR = (addr))
#define	WrNFDat(dat)	WrNFDat8(dat)
#define	RdNFDat()		RdNFDat8()	//for 8 bit nand flash, use byte access

#define	RdNFMEcc()		(rNFMECC0)	//for 8 bit nand flash, only use NFMECC0
#define	RdNFSEcc()		(rNFSECC)	//for 8 bit nand flash, only use low 16 bits

#define	RdNFStat()		(rNFSTAT)
#define	NFIsBusy()		(!(rNFSTAT&1))
#define	NFIsReady()		(rNFSTAT&1)

//#define	WIAT_BUSY_HARD	1
//#define	ER_BAD_BLK_TEST
//#define	WR_BAD_BLK_TEST

#define	READCMD0	0
#define	READCMD1	1
#define	READCMD2	0x50
#define	ERASECMD0	0x60
#define	ERASECMD1	0xd0
#define	PROGCMD0	0x80
#define	PROGCMD1	0x10
#define	QUERYCMD	0x70
#define	RdIDCMD		0x90

static U16 NandAddr;

// HCLK=100Mhz
#define TACLS		1//7	// 1-clk(0ns) 
#define TWRPH0		4//7	// 3-clk(25ns)
#define TWRPH1		0//7	// 1-clk(10ns)  //TACLS+TWRPH0+TWRPH1>=50ns

static void InitNandCfg(void)
{
	// for S3C2440

	rNFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<0);	
	// TACLS		[14:12]	CLE&ALE duration = HCLK*TACLS.
	// TWRPH0		[10:8]	TWRPH0 duration = HCLK*(TWRPH0+1)
	// TWRPH1		[6:4]	TWRPH1 duration = HCLK*(TWRPH1+1)
	// AdvFlash(R)	[3]		Advanced NAND, 0:256/512, 1:1024/2048
	// PageSize(R)	[2]		NAND memory page size
	//						when [3]==0, 0:256, 1:512 bytes/page.
	//						when [3]==1, 0:1024, 1:2048 bytes/page.
	// AddrCycle(R)	[1]		NAND flash addr size
	//						when [3]==0, 0:3-addr, 1:4-addr.
	//						when [3]==1, 0:4-addr, 1:5-addr.
	// BusWidth(R/W) [0]	NAND bus width. 0:8-bit, 1:16-bit.
	
	rNFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(1<<6)|(1<<5)|(1<<4)|(1<<1)|(1<<0);
	// Lock-tight	[13]	0:Disable lock, 1:Enable lock.
	// Soft Lock	[12]	0:Disable lock, 1:Enable lock.
	// EnablillegalAcINT[10]	Illegal access interupt control. 0:Disable, 1:Enable
	// EnbRnBINT	[9]		RnB interrupt. 0:Disable, 1:Enable
	// RnB_TrandMode[8]		RnB transition detection config. 0:Low to High, 1:High to Low
	// SpareECCLock	[6]		0:Unlock, 1:Lock
	// MainECCLock	[5]		0:Unlock, 1:Lock
	// InitECC(W)	[4]		1:Init ECC decoder/encoder.
	// Reg_nCE		[1]		0:nFCE=0, 1:nFCE=1.
	// NANDC Enable	[0]		operating mode. 0:Disable, 1:Enable.

//	rNFSTAT = 0;
    
//    Nand_Reset();
}

#ifdef	WIAT_BUSY_HARD
#define	WaitNFBusy()	while(NFIsBusy())
#else
static U32 WaitNFBusy(void)	// R/B 未接好?
{
	U8 stat;
	
	WrNFCmd(QUERYCMD);
	do {
		stat = RdNFDat();
		//Uart_Printf("%x\n", stat);
	}while(!(stat&0x40));
	WrNFCmd(READCMD0);
	return stat&1;
}
#endif

static U32 ReadChipId(void)
{
	U32 id;
	
	NFChipEn();	
	WrNFCmd(RdIDCMD);
	WrNFAddr(0);
	while(NFIsBusy());	
	id  = RdNFDat()<<8;
	id |= RdNFDat();		
	NFChipDs();		
	
	return id;
}

static U16 ReadStatus(void)
{
	U16 stat;
	
	NFChipEn();	
	WrNFCmd(QUERYCMD);		
	stat = RdNFDat();	
	NFChipDs();
	
	return stat;
}

static U32 EraseBlock(U32 addr)
{
	U8 stat;

	addr &= ~0x1f;
		
	NFChipEn();	
	WrNFCmd(ERASECMD0);		
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	WrNFCmd(ERASECMD1);		
	stat = WaitNFBusy();
	NFChipDs();
	
#ifdef	ER_BAD_BLK_TEST
	if(!((addr+0xe0)&0xff)) stat = 1;	//just for test bad block
#endif
	
	Uart_Printf("Erase block 0x%x %s\n", addr, stat?"fail":"ok");
		
	return stat;
}

//addr = page address
static void ReadPage(U32 addr, U8 *buf)
{
	U16 i;
	
	NFChipEn();
	WrNFCmd(READCMD0);
	WrNFAddr(0);
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	InitEcc();
	WaitNFBusy();
	for(i=0; i<512; i++)
		buf[i] = RdNFDat();
	NFChipDs();
}

static U32 WritePage(U32 addr, U8 *buf)
{
	U32 i, mecc;
	U8 stat, tmp[7];
	
	
	NFChipEn();
	WrNFCmd(PROGCMD0);
	WrNFAddr(0);
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	InitEcc();	//reset mecc and secc
	MEccUnlock();
	for(i=0; i<512; i++)
		WrNFDat(buf[i]);
	MEccLock();
	
	mecc = RdNFMEcc();
		
	tmp[0] = mecc&0xff;
    tmp[1] = (mecc>>8)&0xff;
    tmp[2] = (mecc>>16)&0xff;
    tmp[3] = (mecc>>24)&0xff;
    tmp[5] = 0xff;	//mark good block
    
    SEccUnlock();
	WrNFDat(tmp[0]);
	WrNFDat(tmp[1]);
	WrNFDat(tmp[2]);
	WrNFDat(tmp[3]);
	SEccLock();
	WrNFDat(tmp[4]);
	WrNFDat(tmp[5]);
    	
	WrNFCmd(PROGCMD1);
	stat = WaitNFBusy();
	NFChipDs();
	
#ifdef	WR_BAD_BLK_TEST
	if((addr&0xff)==0x17) stat = 1;	//just for test bad block
#endif
		
	if(stat)
		Uart_Printf("Write nand flash 0x%x fail\n", addr);
	else {	
		U8 RdDat[512];
		
		ReadPage(addr, RdDat);		
		for(i=0; i<512; i++)
			if(RdDat[i]!=buf[i]) {
				Uart_Printf("Check data at page 0x%x, offset 0x%x fail\n", addr, i);
				stat = 1;
				break;
			}
	}
		
	return stat;	
}

static void MarkBadBlk(U32 addr)
{
	addr &= ~0x1f;
	
	NFChipEn();
	
	WrNFCmd(READCMD2);	//point to area c
	
	WrNFCmd(PROGCMD0);
	WrNFAddr(4);		//mark offset 4,5,6,7
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	WrNFDat(0);			//mark with 0
	WrNFDat(0);
	WrNFDat(0);			//mark with 0
	WrNFDat(0);
	WrNFCmd(PROGCMD1);
	WaitNFBusy();		//needn't check return status
	
	WrNFCmd(READCMD0);	//point to area a
		
	NFChipDs();
}

static int CheckBadBlk(U32 addr)
{
	U8 dat;
	
	addr &= ~0x1f;
	
	NFChipEn();
	
	WrNFCmd(READCMD2);	//point to area c
	WrNFAddr(5);		//mark offset 4,5,6,7
	WrNFAddr(addr);
	WrNFAddr(addr>>8);
	if(NandAddr)
		WrNFAddr(addr>>16);
	WaitNFBusy();
	dat = RdNFDat();
	
	WrNFCmd(READCMD0);	//point to area a
	
	NFChipDs();

	return (dat!=0xff);
}

/************************************************************/
struct Partition{
	U32 offset;
	U32 size;
	char *name;
};

static struct Partition NandPart[] = {
	{0, 		 0x00030000, "boot"},		//256K
	{0x00030000, 0x001d0000, "kernel"},
	{0x00200000, 0x02000000, "rootfs"},		//30M
	{0x02000000, 0x02000000, "ext-fs1"},	//32M
	{0,			 0         , 0}
};
/*
static void TestFunc(void)
{
	U32 i;
	U8 buf[512];
	
	if(EraseBlock(0x180))
		return;
	
	for(i=0; i<512; i++)
		buf[i] = i;
		
	WritePage(0x180, buf);	
	for(i=0; i<512; i++)
		buf[i] = 0;
	ReadPage(0x180, buf);
	
	for(i=0; i<512; i++)
		Uart_Printf("%4x", buf[i]);
}
*/
static U32 StartPage, BlockCnt;
extern U32 downloadAddress; 
extern U32 downloadFileSize;


static int NandSelPart(char *info)
{
	U16 i, max_sel;
	struct Partition *ptr = NandPart;
	
	Uart_Printf("Please select which region to %s : Esc to abort\n", info);
	
	for(i=0; ptr->size!=0; i++, ptr++)
		Uart_Printf("%d : offset 0x%-8x, size 0x%-8x [%s]\n", i, ptr->offset, ptr->size, ptr->name);
		
	max_sel = i;
	
	while(1) {
		i = Uart_Getch();
		if(i==0x1b)
			return -1;
		if((i>='0')&&(i<(max_sel+'0'))) {
			i -= '0';
			StartPage = NandPart[i].offset>>9;
			BlockCnt  = NandPart[i].size>>14;
			return i;
		}
	}	
}

static void WrFileToNF(void)
{
	int nf_part, i ,size, skip_blks;
	U32 ram_addr;
	
	nf_part = NandSelPart("write");
	if(nf_part<0)
		return;	
	
	if(downloadFileSize>NandPart[nf_part].size) {
		Uart_Printf("Download file size is more large than selected partition size!!!\n");
//		return;
	}
	
	Uart_Printf("Now write nand flash page 0x%x from ram address 0x%x, filesize = %d\n", StartPage, downloadAddress, downloadFileSize);
	Uart_Printf("Are you sure? [y/n]\n");
	while(1) {
		char c = Uart_Getch();
		if((c=='y')||(c=='Y'))
			break;
		if((c=='n')||(c=='N'))			
			return;
	}
	
	skip_blks = 0;
	ram_addr = downloadAddress;
	size = downloadFileSize;
	for(i=0; size>0; )	{	
		if(!(i&0x1f)) {
			if(EraseBlock(i+StartPage)) {
				NandPart[nf_part].size -= 32<<9;	//partition available size - 1 block size
				if(downloadFileSize>NandPart[nf_part].size) {
					Uart_Printf("Program nand flash fail\n");
					return;
				}
				MarkBadBlk(i+StartPage);
				skip_blks++;				
				i += 32;				
				continue;
			}
		}
		if(WritePage(i+StartPage, (U8 *)ram_addr)) {
			ram_addr -= (i&0x1f)<<9;
			size += (i&0x1f)<<9;
			i &= ~0x1f;
			NandPart[nf_part].size -= 32<<9;	//partition available size - 1 block size
			if(downloadFileSize>NandPart[nf_part].size) {
				Uart_Printf("Program nand flash fail\n");
				return;
			}			
			MarkBadBlk(i+StartPage);
			skip_blks++;			
			i += 32;			
			continue;
		}
		ram_addr += 512;
		size -= 512;
		i++;
	}

	Uart_Printf("Program nand flash partition success\n");
	if(skip_blks)
		Uart_Printf("Skiped %d bad block(s)\n", skip_blks);
}

#define LINUX_PAGE_SHIFT	12
#define LINUX_PAGE_SIZE		(1<<LINUX_PAGE_SHIFT)
#define COMMAND_LINE_SIZE 	1024

struct param_struct {
    union {
	struct {
	    unsigned long page_size;			/*  0 */
	    unsigned long nr_pages;				/*  4 */
	    unsigned long ramdisk_size;			/*  8 */
	    unsigned long flags;				/* 12 */
#define FLAG_READONLY	1
#define FLAG_RDLOAD		4
#define FLAG_RDPROMPT	8
	    unsigned long rootdev;				/* 16 */
	    unsigned long video_num_cols;		/* 20 */
	    unsigned long video_num_rows;		/* 24 */
	    unsigned long video_x;				/* 28 */
	    unsigned long video_y;				/* 32 */
	    unsigned long memc_control_reg;		/* 36 */
	    unsigned char sounddefault;			/* 40 */
	    unsigned char adfsdrives;			/* 41 */
	    unsigned char bytes_per_char_h;		/* 42 */
	    unsigned char bytes_per_char_v;		/* 43 */
	    unsigned long pages_in_bank[4];		/* 44 */
	    unsigned long pages_in_vram;		/* 60 */
	    unsigned long initrd_start;			/* 64 */
	    unsigned long initrd_size;			/* 68 */
	    unsigned long rd_start;				/* 72 */
	    unsigned long system_rev;			/* 76 */
	    unsigned long system_serial_low;	/* 80 */
	    unsigned long system_serial_high;	/* 84 */
	    unsigned long mem_fclk_21285;       /* 88 */
	} s;
	char unused[256];
    } u1;
    union {
	char paths[8][128];
	struct {
	    unsigned long magic;
	    char n[1024 - sizeof(unsigned long)];
	} s;
    } u2;
    char commandline[COMMAND_LINE_SIZE];
};

extern void  call_linux(U32 a0, U32 a1, U32 a2);

/*************************************************************/
static __inline void cpu_arm920_cache_clean_invalidate_all(void)
{
	__asm{
		mov	r1, #0		
		mov	r1, #7 << 5			  	/* 8 segments */
cache_clean_loop1:		
		orr	r3, r1, #63UL << 26	  	/* 64 entries */
cache_clean_loop2:	
		mcr	p15, 0, r3, c7, c14, 2	/* clean & invalidate D index */
		subs	r3, r3, #1 << 26
		bcs	cache_clean_loop2		/* entries 64 to 0 */
		subs	r1, r1, #1 << 5
		bcs	cache_clean_loop1		/* segments 7 to 0 */
		mcr	p15, 0, r1, c7, c5, 0	/* invalidate I cache */
		mcr	p15, 0, r1, c7, c10, 4	/* drain WB */
	}
}
void cache_clean_invalidate(void)
{
	cpu_arm920_cache_clean_invalidate_all();
}

static __inline void cpu_arm920_tlb_invalidate_all(void)
{
	__asm{
		mov	r0, #0
		mcr	p15, 0, r0, c7, c10, 4	/* drain WB */
		mcr	p15, 0, r0, c8, c7, 0	/* invalidate I & D TLBs */
	}
}

void tlb_invalidate(void)
{
	cpu_arm920_tlb_invalidate_all();
}

void disable_irq(void);

void call_linux(U32 a0, U32 a1, U32 a2)
{
	void (*goto_start)(U32, U32);
	cache_clean_invalidate();
	tlb_invalidate();	

	__asm{
//		mov	r0, a0//%0
//		mov	r1, a1//%1
//		mov	r2, a2//%2
		mov	ip, #0
		mcr	p15, 0, ip, c13, c0, 0	/* zero PID */
		mcr	p15, 0, ip, c7, c7, 0	/* invalidate I,D caches */
		mcr	p15, 0, ip, c7, c10, 4	/* drain write buffer */
		mcr	p15, 0, ip, c8, c7, 0	/* invalidate I,D TLBs */
		mrc	p15, 0, ip, c1, c0, 0	/* get control register */
		bic	ip, ip, #0x0001			/* disable MMU */
		mcr	p15, 0, ip, c1, c0, 0	/* write control register */
		//mov	pc, r2
		//nop
		//nop
		/* no outpus */
		//: "r" (a0), "r" (a1), "r" (a2)
	}
//	SetClockDivider(1, 1);
//	SetSysFclk(FCLK_200M);		//start kernel, use 200M
	SET_IF();
	goto_start = (void (*)(U32, U32))a2;
	(*goto_start)(a0, a1);	
}

static void LoadRun(void)
{
	U32 i, ram_addr, buf = boot_params.run_addr.val;
	struct param_struct *params = (struct param_struct *)0x30000100;
	int size;
	
	memset(params, 0, sizeof(struct param_struct));
	/*{
		U32 *pD = (U32 *)0x30000100;
		
		for(i=0; i<0x1000; i++)
			pD[i] = 0;
	}*/
	if(boot_params.start.val) {
		char parameters[512];
		char *rootfs;
		char initrd[32];
		char *tty_sel;
		char *devfs_sel;
		
		switch (boot_params.root_sel.val) {
		case 0:
			rootfs = "/dev/ram";
			break;
		case 1:
			rootfs = "nfs";
			break;
		case 2:
			rootfs = "/dev/mtdblock2";
			break;
		case 3:
			rootfs = "/dev/mtdblock3";
			break;
		case 4:
			rootfs = "/dev/hda1";
		default:
			rootfs="";	//user define
			break;
		}
	
		if(boot_params.root_sel.val)
			Uart_Printf(initrd, "load_ramdisk=0");
		else
			Uart_Printf(initrd, "initrd=0x%08x,0x%08x",
						boot_params.initrd_addr.val,
						boot_params.initrd_len.val);
	
		switch (boot_params.tty_sel.val) {
		case 0:
			tty_sel="ttyS0";
			break;
		case 1:
			tty_sel="ttyS1";
			break;
		case 2:
			tty_sel="tty1";
			break;
		default:
			tty_sel="";	//user define
			break;
		}
	
		if(boot_params.devfs_sel.val)
			devfs_sel = "devfs=mount";
		else
			devfs_sel = "";
		
		memset(parameters, 0, sizeof(parameters));
		Uart_Printf(parameters,
					"root=%s init=/linuxrc %s console=%s,%d mem=%dK %s %s",
					rootfs,
					initrd,
					tty_sel,
					boot_params.serial_baud.val,
					boot_params.mem_cfg.val>>10,
					devfs_sel,
					boot_params.string);
		
		params->u1.s.page_size = LINUX_PAGE_SIZE;
		params->u1.s.nr_pages = (boot_params.mem_cfg.val >> LINUX_PAGE_SHIFT);
		memcpy(params->commandline, parameters, strlen(parameters));
	
		Uart_Printf("Set boot params = %s\n", params->commandline);
	}
	
	StartPage = NandPart[1].offset>>9;
	size = NandPart[1].size;
	ram_addr = buf;	
	
	Uart_Printf("Load Kernel...\n");
	for(i=0; size>0; ) {
		
		if(!(i&0x1f)) {
			if(CheckBadBlk(i+StartPage)) {
				Uart_Printf("Skipped bad block at 0x%x\n", i+StartPage);
				i += 32;
				size -= 32<<9;
				continue;
			}
		}
		ReadPage((i+StartPage), (U8 *)ram_addr);
		i++;
		size -= 512;
		ram_addr += 512;
	}
	
	if(!boot_params.root_sel.val) {
		int ramdisk_sz;
		
		Uart_Printf("Load Ramdisk...\n");
		
		StartPage = NandPart[2].offset>>9;
		size = NandPart[1].size;
		
		ram_addr = boot_params.initrd_addr.val;
		ramdisk_sz = boot_params.initrd_len.val;
		
		for(i=0; size>0&&ramdisk_sz>0; ) {
			if(!(i&0x1f)) {
				if(CheckBadBlk(i+StartPage)) {
					Uart_Printf("Skipped bad block at 0x%x\n", i+StartPage);
					i += 32;
					size -= 32<<9;
					continue;
				}
			}
			ReadPage((i+StartPage), (U8 *)ram_addr);
			i++;
			size -= 512;
			ram_addr += 512;
			ramdisk_sz -= 512;
		}
	}
	
	DsNandFlash();

		//ChangeClockDivider(13,12);
		//ChangeMPllValue(67,1,1);	//300MHz,2440A!
		//rCLKCON = 0x7fff0;
	call_linux(0, boot_params.machine.val, buf);
}

/************************************************************/
static void InitNandFlash(int info)
{	
	U32 i;
	
	InitNandCfg();
	i = ReadChipId();
	if(info)
		Uart_Printf("Read chip id = %x\n", i);	
	if((i==0x9873)||(i==0xec75))	
		NandAddr = 0;
	else if(i==0xec76)
		NandAddr = 1;
	else {	
		Uart_Printf("Chip id error!!!\n");
		return;
	}
	if(info)
		Uart_Printf("Nand flash status = %x\n", ReadStatus());
}

void NandErase(void)
{
	int i, err = 0;
	
	InitNandFlash(1);
	
	i = NandSelPart("erase");
	if(i<0)
		return;	
	
	Uart_Printf("Are you sure to erase nand flash from page 0x%x, block count 0x%x ? [y/n]\n", StartPage, BlockCnt);
	while(1) {
		char c;
		
		c = Uart_Getch();
		if((c=='y')||(c=='Y'))
			break;
		if((c=='n')||(c=='N'))
			return;
	}	
	
	for(i=0; BlockCnt; BlockCnt--, i+=32) {
		if(EraseBlock(i+StartPage)) {
			err ++;
			Uart_Printf("Press any key to continue...\n");
			Uart_Getch();
		}
	}	

	DsNandFlash();		//disable nand flash interface
	Uart_Printf("Erase Nand partition completed ");
	if(err)
		Uart_Printf("with %d bad block(s)\n", err);
	else
		Uart_Printf("success\n");
}

void NandWrite(void)
{
	InitNandFlash(1);
	WrFileToNF();
	DsNandFlash();		//disable nand flash interface
}

void NandLoadRun(void)
{
	InitNandFlash(1);
	LoadRun();
}

/*
void TestNandFlash(void)
{		
	InitNandFlash();	
	
	while(1)
	{
		U8 key = '2';
		
		Uart_Printf("\nNand flash operations, press ESC to exit\n");
		Uart_Printf("1.Write nand flash with download file\n");
		Uart_Printf("2.Load progam from nand flash and run\n");
		Uart_Printf("3.Erase nand flash partition\n");
		Uart_Printf("4.Test nand flash erase, write, read\n");
	
		key = Uart_Getch();		
		
		if(key==0x1b)
			goto TestNandExit;
		else if(key=='1')
			WrFileToNF();
		else if(key=='2')
			LoadRun();
		else if(key=='3')
			NandErase();
		else if(key=='4')	
			TestFunc();
	}			
TestNandExit:	
	DsNandFlash();		//disable nand flash interface
}*/

int search_params(void)
{
	U32 page, page_cnt;
	U8 dat[528];
	BootParams *pBP = (BootParams *)dat;
	int ret=-1;
	
	InitNandFlash(0);	//don't show info in InitNandFlash!
	
	page     = NandPart[0].offset>>9;
	page_cnt = NandPart[0].size>>9;
	
	//search from the last page	
	while(page_cnt--) {
		ReadPage(page+page_cnt, dat);
		if(!strncmp(boot_params.start.flags, pBP->start.flags, 10)) {
			ret = 0;
			break;
		}
	}
	
	if(!ret) {
		ParamItem *pPIS = &pBP->start, *pPID = &boot_params.start;
		
		for(; pPID<=&boot_params.user_params; pPIS++, pPID++)
			if(!strncmp(pPID->flags, pPIS->flags, sizeof(pPID->flags)))
				pPID->val = pPIS->val;
		strncpy(boot_params.string, pPIS->flags, boot_params.user_params.val+1);
		if(boot_params.user_params.val!=strlen(pPID->flags)) {
			memset(boot_params.string, 0, sizeof(boot_params.string));
			boot_params.user_params.val = 0;
		}
	} else {
		//Uart_Printf("Fail to find boot params! Use Default parameters.\n");
		//don't Uart_Printf anything before serial initialized!
	}
		
	DsNandFlash();
	
	return ret;
}

//保存在最后两个block,应确保程序(包括出现坏块时)不会覆盖到最后两块
int save_params(void)
{
	U32 page, page_cnt;
	U8 dat[528];
	
	InitNandFlash(1);
	
	memset(dat, 0, sizeof(dat));
	memcpy(dat, &boot_params, sizeof(boot_params));
	
	page = (NandPart[1].offset>>9)-64;
	
	for(page_cnt=0; page_cnt<64; page_cnt++) {
		if(!(page_cnt%32))
			EraseBlock(page);
		if(!WritePage(page+page_cnt, dat))
			break;
	}
	
	DsNandFlash();
	
	Uart_Printf("Save boot params %s.\n", (page_cnt>=64)?"fail":"success");
	
	return page_cnt>=64;
}

int set_params(void)
{
	int i, key, chg=0;
	ParamItem *pPID;
	
	Uart_Printf("\nConfig parameters\n");
	
	do {
		pPID = &boot_params.start;
		for(i=0; pPID<=&boot_params.user_params; pPID++, i++)
			Uart_Printf("[%d] : %s%-8s is 0x%08x (%d)\n",
						i, (i>9)?"":" ", pPID->flags, pPID->val, pPID->val);
		Uart_Printf("[%d] : Exit\n", i);
		if(boot_params.user_params.val)
			Uart_Printf("User parameters is : \"%s\"\n", boot_params.string);
		
		Uart_Printf("\nplease select item:");
		key = Uart_GetIntNum();
		if(key>=0&&key<i) {
			chg = 1;
			Uart_Printf("please enter value:");
			i = key;
			if((&boot_params.start + i)==&boot_params.user_params) {
				//确保输入的字节数不超过127!
				char cmd[128];
				memset(cmd, 0, sizeof(cmd));
				Uart_GetString(cmd);
				strncpy(boot_params.string, cmd, strlen(cmd)+1);
				boot_params.user_params.val = strlen(cmd);
			} else {
				key = Uart_GetIntNum();
				(&boot_params.start + i)->val = key;
			}
		} else
			break;
	} while(1);
	
	if(chg) {
		Uart_Printf("Do you want to save parameters? press y or Y for save.\n");
		key = Uart_Getch();
		if(key=='y'||key=='Y')
			save_params();
	}
	
	return 0;
}

