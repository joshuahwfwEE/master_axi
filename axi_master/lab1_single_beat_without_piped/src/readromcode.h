#ifndef READROMCODE_H_
#define READROMCODE_H_
#define CODE_SIZE 1024*4


void readromcode(
//ps side interface
		int ps_to_romcode[CODE_SIZE/sizeof(int)],
		int internal_bram[CODE_SIZE/sizeof(int)],
		int romcode_to_ps[CODE_SIZE/sizeof(int)],
        int cmd
		);
#endif
