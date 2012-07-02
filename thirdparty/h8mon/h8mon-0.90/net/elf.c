/** @file
 * @brief	ELFローダ
 *
 * ELF フォーマットのバイナリをロードします。
 *
 * @date	2008.02.19
 * @author	Takashi SHUDO
 */

#include "elf.h"

//#define DEBUG
#ifdef DEBUG
#define EXECPRINTF	tprintf
#else
#define EXECPRINTF(x, ...)
#endif

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

static unsigned char *elf_start;
static long seekaddr;

static long seek_file(unsigned long offset, int whence)
{
	seekaddr = offset;
	
	return offset;
}

static long read_file(void *buf, unsigned long count)
{
	long i;
	unsigned char *ptr = (unsigned char *)buf;

	for(i=0; i<count; i++) {
		ptr[i] = elf_start[seekaddr];
		seekaddr ++;
	}
	
	return count;
}


typedef unsigned long	Elf32_Addr;
typedef unsigned short	Elf32_Half;
typedef unsigned long	Elf32_Off;
typedef unsigned long	Elf32_Word;
typedef long Elf32_Sword;
typedef unsigned short Elf32_Section;

struct Elf32_Ehdr {
	unsigned char e_ident[16];
	Elf32_Half	e_type;		// ELF形式
	Elf32_Half	e_machine;	// マシンアーキテクチャ
	Elf32_Word	e_version;	// ELFフォーマットバージョン
	Elf32_Addr	e_entry;	// エントリアドレス
	Elf32_Off	e_phoff;	// ヘッダテーブルオフセット
	Elf32_Off	e_shoff;	// 
	Elf32_Word	e_flags;	// プロセッサ固有フラグ
	Elf32_Half	e_ehsize;	// ELFヘッダサイズ
	Elf32_Half	e_phentsize;	// ヘッダテーブルサイズ
	Elf32_Half	e_phnum;	// ヘッダテーブル要素数
	Elf32_Half	e_shentsize;
	Elf32_Half	e_shnum;
	Elf32_Half	e_shstrndx;
};

// e_ident[]
#define ELFMAG0	0x7f	// e_ident[0]
#define ELFMAG1	'E'	// e_ident[1]
#define ELFMAG2	'L'	// e_ident[2]
#define ELFMAG3	'F'	// e_ident[3]
#define ELFMAG	"\177ELF"	// magic
#define SELFMAG	4		// size of magic

// e_ident[4] file class
#define ELFCLASSNONE	0	// invalid
#define ELFCLASS32	1	// 32-bit objs
#define ELFCLASS64	2	// 64-bit objs
#define ELFCLASSNUM	3	// number of classes

// e_ident[5] data encoding
#define ELFDATANONE	0	// invalid
#define ELFDATA2LSB	1	// Little-Endian
#define ELFDATA2MSB	2	// Big-Endian
#define ELFDATANUM      3	// number of data encode defines

// e_type
#define ET_NONE	0	// No file type
#define ET_REL	1	// relocatable file
#define ET_EXEC	2	// executable file
#define ET_DYN	3	// shared object file
#define ET_CORE	4	// core file
#define ET_NUM	5	// number of types

// e_machine
#define EM_H8300H	0x002E	// H8/300H

struct Elf32_Phdr {
	Elf32_Word p_type;	// エントリのタイプ
	Elf32_Off  p_offset;	// セグメントのファイル先頭からのオフセット
	Elf32_Addr p_vaddr;	// セグメントの第一バイトの仮想アドレス
	Elf32_Addr p_paddr;	// 予約
	Elf32_Word p_filesz;	// セグメントのファイルでのサイズ
	Elf32_Word p_memsz;	// セグメントのメモリサイズ
	Elf32_Word p_flags;	// フラグ
	Elf32_Word p_align;	// 
};

// Segment types - p_type
#define PT_NULL		0	// unused
#define PT_LOAD		1	// loadable segment
#define PT_DYNAMIC	2	// dynamic linking section
#define PT_INTERP	3	// the RTLD
#define PT_NOTE		4	// auxiliary information
#define PT_SHLIB	5	// reserved - purpose undefined
#define PT_PHDR		6	// program header
#define PT_NUM		7	// Number of segment types

// Segment flags - p_flags
#define PF_X		0x1	// Executable
#define PF_W		0x2	// Writable
#define PF_R		0x4	// Readable

struct Elf32_Shdr {
	Elf32_Word	sh_name;	// 名称
	Elf32_Word	sh_type;	// タイプ
	Elf32_Word	sh_flags;	//
	Elf32_Addr	sh_addr;	//
	Elf32_Off	sh_offset;	//
	Elf32_Word	sh_size;	//
	Elf32_Word	sh_link;	//
	Elf32_Word	sh_info;	//
	Elf32_Word	sh_addralign;	//
	Elf32_Word	sh_entsize;	//
};

// sh_type
#define SHT_NULL	0	// inactive
#define SHT_PROGBITS	1	// program defined information
#define SHT_SYMTAB	2	// symbol table section
#define SHT_STRTAB	3	// string table section
#define SHT_RELA	4	// relocation section with addend
#define SHT_HASH	5	// symbol hash table section
#define SHT_DYNAMIC	6	// dynamic section
#define SHT_NOTE	7	// note section
#define SHT_NOBITS	8	// no space section
#define SHT_REL		9	// relation section without addends
#define SHT_SHLIB	10	// reserved - purpose unknown
#define SHT_DYNSYM	11	// dynamic symbol table section
#define SHT_NUM		12	// number of section types

// Section Attribute Flags - sh_flags
#define SHF_WRITE	0x1	// Writable
#define SHF_ALLOC	0x2	// occupies memory
#define SHF_EXECINSTR	0x4	// executable

typedef struct {
	Elf32_Addr	r_offset;	// Address
	Elf32_Word	r_info;		// Relocation type and symbol index
	Elf32_Sword	r_addend;	// Addend
} Elf32_Rela;

#define ELF32_R_SYM(val)		((val) >> 8)
#define ELF32_R_TYPE(val)		((val) & 0xff)
#define ELF32_R_INFO(sym, type)		(((sym) << 8) + ((type) & 0xff))

// Symbol table entry.
typedef struct {
	Elf32_Word	st_name;	// Symbol name (string tbl index)
	Elf32_Addr	st_value;	// Symbol value
	Elf32_Word	st_size;	// Symbol size
	unsigned char	st_info;	// Symbol type and binding
	unsigned char	st_other;	// Symbol visibility
	Elf32_Section	st_shndx;	// Section index
} Elf32_Sym;

////////

#ifdef ED_LITTLE
#define E_LONG(x)	((((long)x & 0xff000000)>>24) | \
			 (((long)x & 0xff0000)>>8) | \
			 (((long)x & 0xff00)<<8) | \
			 (((long)x & 0xff)<<24))

#define E_SHORT(x)	((((short)x & 0xff00)>>8) | \
			 (((short)x & 0xff)<<8))
#else
#define E_LONG(x)	(x)
#define E_SHORT(x)	(x)
#endif

#include "tprintf.h"
#define SYSERR_PRINT tprintf
#define MEM_START	0x400000
#define STATIC_MEM_SIZE	0x200000

static const char elf_mag[] = ELFMAG;

static int load_section(void *dest, unsigned long offset,
			unsigned long size)
{
	long fr;

	EXECPRINTF(".text/.data - Addr:%08lX Offset:%08lX Size:%ld\n",
		   (unsigned long)dest,
		   offset, size);

	tprintf("Load : %08lX - %08lX (%08lX)\n", (unsigned long)dest,
		(unsigned long)(dest + size - 1), size);

	if(((unsigned long)dest) < (unsigned long)MEM_START) {
		SYSERR_PRINT("ELF start address(%06lX) error.\n",
			     (unsigned long)dest);
		return -1;
	}

#if 0
	if(((unsigned long)dest + size) >
	   ((unsigned long)MEM_START + STATIC_MEM_SIZE)) {
		SYSERR_PRINT("ELF end address(%06lX) error.\n",
			     (unsigned long)dest + size);
		return -1;
	}
#endif

	fr = seek_file(offset, SEEK_SET);
	if(fr == -1) {
		SYSERR_PRINT("File seek error\n");
		return -1;
	}

	fr = read_file(dest, size);
	if(fr != size) {
		SYSERR_PRINT("File read error\n");
		return -1;
	}

	return 0;
}

static int load_relocation_addend(unsigned long offset,
				  unsigned long size)
{
	long fr;
	int i;
	Elf32_Rela rela;
	
	EXECPRINTF("RELA - Offset:%08lX Size:%ld\n", offset, size);

	fr = seek_file(offset, SEEK_SET);
	if(fr == -1) {
		SYSERR_PRINT("File read error\n");
		return -1;
	}

	for(i=0; i<(size / sizeof(rela)); i++) {
		fr = read_file(&rela, sizeof(rela));
		if(fr != sizeof(rela)) return -1;
		EXECPRINTF("offset:%08lX ", E_LONG(rela.r_offset));
		EXECPRINTF("sym:%06lX ", ELF32_R_SYM(E_LONG(rela.r_info)));
		EXECPRINTF("type:%02lX ", ELF32_R_TYPE(E_LONG(rela.r_info)));
		EXECPRINTF("addend:%08lX\n", E_LONG(rela.r_addend));
	}

	return 0;
}

static int load_symtab(unsigned long offset,
		       unsigned long size)
{
	long fr;
	int i;
	Elf32_Sym sym;
	
	EXECPRINTF("SYMTAB - Offset:%08lX Size:%ld\n", offset, size);

	fr = seek_file(offset, SEEK_SET);
	if(fr == -1) {
		SYSERR_PRINT("File seek error\n");
		return -1;
	}

	for(i=0; i<(size / sizeof(sym)); i++) {
		fr = read_file(&sym, sizeof(sym));
		if(fr != sizeof(sym)) return -1;
		EXECPRINTF("%4d ", i);
		EXECPRINTF("name:%08lX ", E_LONG(sym.st_name));
		EXECPRINTF("value:%08lX ", E_LONG(sym.st_value));
		EXECPRINTF("size:%08lX ", E_LONG(sym.st_size));
		EXECPRINTF("info:%02X ", sym.st_info);
		EXECPRINTF("other:%02X ", sym.st_other);
		EXECPRINTF("shndx:%04X\n", E_SHORT(sym.st_shndx));
	}

	return 0;
}

/** 
 * @brief	ELFファイルをロードする
 * 
 * @return エントリーアドレス
 */
int load_elf(unsigned long *entry, unsigned char *elfbin)
{
	long fr;
	int i;
	struct Elf32_Ehdr elf_head;
	struct Elf32_Phdr elf_phdr;
	struct Elf32_Shdr elf_shdr;

	elf_start = elfbin;
	fr = seek_file(0, SEEK_SET);

	// Ehdr
	fr = read_file(&elf_head, sizeof(struct Elf32_Ehdr));
	if(fr != sizeof(struct Elf32_Ehdr)) {
		SYSERR_PRINT("File read error\n");
		return -1;
	}

	for(i=0; i<SELFMAG; i++) {
		if(elf_head.e_ident[i] != elf_mag[i]) {
			EXECPRINTF("Invalid elf_head.e_ident\n");
			return -1;
		}
	}
	
	if(elf_head.e_ident[4] != ELFCLASS32) {
		EXECPRINTF("Invalid elf_head.e_ident[4]\n");
		return -1;
	}
	
	if(elf_head.e_ident[5] != ELFDATA2MSB) {
		EXECPRINTF("Invalid elf_head.e_ident[5]\n");
		return -1;
	}
	
	if(elf_head.e_ident[6] != 0x01) {
		EXECPRINTF("Invalid elf_head.e_ident[6]\n");
		return -1;
	}

	if(((E_SHORT(elf_head.e_type) != ET_EXEC) &&
	    (E_SHORT(elf_head.e_type) != ET_REL))) {
		EXECPRINTF("e_type	%04X\n", E_SHORT(elf_head.e_type));
		return -1;
	}
	
	if(E_SHORT(elf_head.e_machine) != EM_H8300H) {
		EXECPRINTF("Error : e_machine	%04X\n",
			   E_SHORT(elf_head.e_machine));
		return -1;
	}

	if(E_LONG(elf_head.e_version) != 0x00000001) {
		EXECPRINTF("Error : e_version	%08lX\n",
			   E_LONG(elf_head.e_version));
		return -1;
	}

	EXECPRINTF("e_phentsize : %04X\n", E_SHORT(elf_head.e_phentsize));
	EXECPRINTF("e_phnum     : %04X\n", E_SHORT(elf_head.e_phnum));

	/*
	 * Phdr
	 */
	
	for(i=0; i<E_SHORT(elf_head.e_phnum); i++) {
		fr = read_file(&elf_phdr, sizeof(struct Elf32_Phdr));
		if(fr != sizeof(struct Elf32_Phdr)) {
			EXECPRINTF("Error : read error.\n");
			return -1;
		}
		
		if(E_LONG(elf_phdr.p_type) != PT_LOAD) {
			EXECPRINTF("p_type		%08lX\n",
				   E_LONG(elf_phdr.p_type));
//			return 0;
		}

		if((E_LONG(elf_phdr.p_flags) & (PF_X | PF_R)) == 0) {
			EXECPRINTF("p_flags		%08lX\n",
				   E_LONG(elf_phdr.p_flags));
//			return 0;
		}
		

		EXECPRINTF("  filesz : %08lX\n", E_LONG(elf_phdr.p_filesz));
		EXECPRINTF("  memsz  : %08lX\n", E_LONG(elf_phdr.p_memsz));
	}
	
	/*
	 * 各セクション
	 */
	EXECPRINTF("No type     flags     addr   offset    size\n");
	for(i=0; i<E_SHORT(elf_head.e_shnum); i++) {
		fr = seek_file(E_LONG(elf_head.e_shoff)
			   + sizeof(struct Elf32_Shdr) * i, SEEK_SET);
		fr = read_file(&elf_shdr, sizeof(struct Elf32_Shdr));
		if(fr != sizeof(struct Elf32_Shdr)) {
			SYSERR_PRINT("File read error\n");
			return -1;
		}
		EXECPRINTF("%08lX ", E_LONG(elf_shdr.sh_type));
		EXECPRINTF("%08lX ", E_LONG(elf_shdr.sh_flags));
		EXECPRINTF("%08lX ", E_LONG(elf_shdr.sh_addr));
		EXECPRINTF("%08lX ", E_LONG(elf_shdr.sh_offset));
		EXECPRINTF("%08lX\n", E_LONG(elf_shdr.sh_size));

		switch(E_LONG(elf_shdr.sh_type)) {
		case SHT_NULL:		// inactive
			break;
			
		case SHT_PROGBITS:	// program defined information
			EXECPRINTF("PROGBITS\n");
			if(E_LONG(elf_shdr.sh_flags) & SHF_ALLOC) {
				if(load_section(
					(void *)E_LONG(elf_shdr.sh_addr),
					E_LONG(elf_shdr.sh_offset),
						E_LONG(elf_shdr.sh_size)) != 0)
				{
					EXECPRINTF("Invalid PROGBITS\n");
					return -1;
				}
			}
			break;
			
		case SHT_SYMTAB:	// symbol table section
			EXECPRINTF("SYMTAB\n");
			load_symtab(
				    E_LONG(elf_shdr.sh_offset),
				    E_LONG(elf_shdr.sh_size));
			break;
			
		case SHT_STRTAB:	// string table section
			break;
			
		case SHT_RELA:		// relocation section with addend
//			EXECPRINTF("RELA\n");
//			EXECPRINTF("%08lX\n", E_LONG(elf_shdr.sh_size));
			load_relocation_addend(
					       E_LONG(elf_shdr.sh_offset),
					       E_LONG(elf_shdr.sh_size));
			break;
			
		case SHT_HASH:		// symbol hash table section
		case SHT_DYNAMIC:	// dynamic section
		case SHT_NOTE:		// note section
			break;
			
		case SHT_NOBITS:	// no space section
			if(E_LONG(elf_shdr.sh_flags) & SHF_ALLOC) {
				EXECPRINTF(".bss ???\n");
			}
			break;
			
		case SHT_REL:		// relation section without addends
		case SHT_SHLIB:		// reserved - purpose unknown
		case SHT_DYNSYM:	// dynamic symbol table section
		case SHT_NUM:		// number of section types
		default:
			break;
		}
	}

	*(unsigned long *)entry = E_LONG(elf_head.e_entry);
	
	return 0;
}
