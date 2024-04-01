#include "readromcode.h"


void readromcode(

		int ps_to_romcode[CODE_SIZE/sizeof(int)], //axi master bus0 load ddr data  //4096/4
		int internal_bram[CODE_SIZE/sizeof(int)], //bram master bus access to bram
		int romcode_to_ps[CODE_SIZE/sizeof(int)], //axi master bus1 store to ddr
        int cmd
		)
{




#pragma HLS TOP name=readromcode
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS INTERFACE mode=s_axilite bundle=control port=ps_to_romcode
#pragma HLS INTERFACE mode=m_axi bundle=bus max_read_burst_length=4 max_widen_bitwidth=32 num_read_outstanding=16 port=ps_to_romcode offset=slave

#pragma HLS INTERFACE mode=s_axilite bundle=control port=romcode_to_ps
#pragma HLS INTERFACE mode=m_axi bundle=bus max_widen_bitwidth=32 max_write_burst_length=4 num_write_outstanding=16 port=romcode_to_ps offset=slave

#pragma HLS INTERFACE mode=bram port=internal_bram
#pragma HLS INTERFACE mode=s_axilite bundle=control port=cmd


int i;
    if(cmd ==0){
	   read_ddr_to_rom: for(i=0; i<CODE_SIZE/sizeof(int); i++){
#pragma HLS PIPELINE II=1
		   internal_bram[i] =ps_to_romcode[i];
	   }
    }else if(cmd ==1){
 	   read_rom_to_ddr: for(i=0; i<CODE_SIZE/sizeof(int); i++){
#pragma HLS PIPELINE II=1
 		   romcode_to_ps[i] =internal_bram[i];
 	   }
    }
    return;
};
