#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xreadromcode.h"
#include "sleep.h"
#include "xil_cache.h"

#define read_buffer_addr  (XPAR_PSU_DDR_0_S_AXI_BASEADDR+0x10000000)//allocate the read buffer size 4KB which store the data write to bram
#define write_buffer_Addr (XPAR_PSU_DDR_0_S_AXI_BASEADDR+0x20000000)//allocate the write buffer size 4KB which store the data read from bram


int main()
{
    init_platform();
    printf("initialize read_buffer in ddr4\r\n");
    printf("give a data set 64 bytes in read_buffer in ddr4\r\n");

    int init_data[64];

	for (int i = 0; i < 64; i++)
	{
	 init_data[i] = i;
     Xil_Out32(read_buffer_addr + i * 4, init_data[i]);
	}

	printf("initialize write_buffer in ddr4\r\n");
	for (int i = 0; i < 64; i++)
	{
		Xil_Out32(write_buffer_Addr + i * 4, 0x00000000);
	}

/*
 * according https://support.xilinx.com/s/question/0D52E00006lLh7pSAC/write-to-ddr-from-ps-read-form-pl-with-custom-ip?language=zh_CN
 * suggest that we need to use flush the cache before rw data to ddr4, if don't flush the cache, you will access the data that is store in cache
 *
 * Xil_DCacheFlushRange:
 * Flushing the data cache ensures that any modified cache lines within the specified range are written back to main memory.
 * This helps maintain data coherency between the cache and main memory,
 * ensuring that any changes made to the memory are visible to other processors or devices accessing the same memory.
 * Flushing the cache is typically necessary before transferring data to a device or memory region that is not cache-coherent,
 * such as certain memory-mapped I/O peripherals or DMA buffers.
*/
	Xil_DCacheFlushRange(read_buffer_addr,0x1000);
	Xil_DCacheFlushRange(write_buffer_Addr,0x1000);
/*
 * Xil_DCacheInvalidateRange:
 * Invalidating the cache removes any cached data within the specified memory range,
 * ensuring that subsequent reads from that memory region fetch the most up-to-date data from main memory.
 * Invalidating the cache is typically necessary after data has been written to a memory region by other devices or processors,
 * ensuring that any stale data cached by the CPU is discarded and replaced with fresh data from memory.
 */
	Xil_DCacheInvalidateRange(read_buffer_addr,0x1000);
	Xil_DCacheInvalidateRange(write_buffer_Addr,0x1000);


//set axi master operation base address
	Xil_Out32((XPAR_READROMCODE_0_S_AXI_CONTROL_BASEADDR) + (0x10), read_buffer_addr);
	Xil_Out32((XPAR_READROMCODE_0_S_AXI_CONTROL_BASEADDR) + (0x1c), write_buffer_Addr);


//write ddr data to bram
	Xil_Out32((XPAR_READROMCODE_0_S_AXI_CONTROL_BASEADDR) + (0x28), 0x00);    //write cmd for read ddr data to bram
	Xil_Out32((XPAR_READROMCODE_0_S_AXI_CONTROL_BASEADDR) + (0x00), 0x01);    //ap_start

    while (XReadromcode_ReadReg(XPAR_READROMCODE_0_S_AXI_CONTROL_BASEADDR, 0x04) != 0){
	printf("waiting for reading ddr4 data to bram\r\n");
    }
    printf("reading ddr4 data to bram is complete\r\n");

sleep(2);


////read bram data to ddr
        XReadromcode_WriteReg(XPAR_READROMCODE_0_S_AXI_CONTROL_BASEADDR, 0x28, 1);   //read cmd for read bram data to ddr4
        XReadromcode_WriteReg(XPAR_READROMCODE_0_S_AXI_CONTROL_BASEADDR, 0x00, 1);   //ap_start

        while (XReadromcode_ReadReg(XPAR_READROMCODE_0_S_AXI_CONTROL_BASEADDR, 0x04) != 0)
        {
    	printf("waiting for write bram data to ddr4\r\n");
        }
        printf("writing bram data to ddr4 is complete\r\n");


//compare the data
u32 readdata =0;
u32 writedata =0;

     printf("read_buffer baseaddress:%x\r\n",read_buffer_addr);
     printf("write_buffer baseaddress:%x\r\n",write_buffer_Addr);
    	for (int i = 0; i < 64; i++)
    	{
    	readdata  = Xil_In32(read_buffer_addr + i * 4);
            printf("offset%x:readdata:%x\r\n",i,readdata);
    	writedata = Xil_In32(write_buffer_Addr + i * 4);
            printf("offset%x:writedata:%x\r\n",i,writedata);

    	   if(writedata==readdata)
    	     {
              printf("offset%x is pass\r\n",i);
    	     }
    	}
     printf("end of compare\r\n");
     cleanup_platform();
    return 0;
}
