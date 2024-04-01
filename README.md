# master_axi
master_axi
axi is a common used interface in ARM and Xilinx,     
it can be used in implementation of various control path like axi_lite and data path like axi4_mm     

this topic we will focus on axi4_memorymap,  
in this repos I will implement 4 types transfer in a bi-directional "DDR4 to Xilinx BLOCK RAM master" and compare their difference in detail:      
here using vitis HLS 2022.2 for implementation the following 4 types:  

################################################################################################################# 
1. single beats in a single transaction that consist of one burst transfer:                                      
   an AXI master that issues requests, one at a time, and then waits for the response from that request before issuing a second request, don’t need to keep track of how many transactions are outstanding at all.

2. single beats in a single transaction that consist of one burst transfer with transaction piped:    
   to imporve 1. performace, you will need to issue multiple transaction without waiting for their responses.  
   This is the purpose of the single beat pipelined master, but it need to use AXID for keeping track of how many transactions are outstanding.     
   A single beat pipelined master will potentially issue multiple single beat requests before ever getting the first response.

3. Bursting without pipelined, single bus:    
   this master can issue the single transaction with multiple data beats in a single bus.    
   
4. Bursting with pipelined, single bus:      
   this master can issue the multiple transaction with each contains multiple data beats in a single bus.    
   
5. (optional)Bursting, multiple bus:    
   this master can deal with multiple bus's different requset like axi crossbar  
   

the difference between 1 and 2 is the throughput of data because of the number of transaction outstanding,  
1 only process 1 transaction and waiting its response while 2 process multiple transaction.  
#######################################################################################################################     
                                                                                                                      
###### notice that using bursting will increasing the number of transaction outstanding, something should beware #############  

1.  Deal with burst addressing and length calculations.
    Burst types: Fixed Address:  
    If the burst is to or from a fixed address, the maximum allowed burst length is limited to 16 beats.  
    Each beat represents a single data transfer of the specified data width
    
    Burst types: Incremental Addressing:  
    If the address increments within the burst, larger burst lengths are permitted, with a maximum burst length of 256 beat are allowed.
    
2.  Bursts are not allowed to cross 4kB boundaries.  
    what is the axi 4kB boundaries: " each axi brust can not over 4kB, because the minimum page size of axi slave is 4kB "  
    the reason is if a single axi brust that is over than 4kB, it may access across different slaves ( because all slave is 4k/1k aligned)    

    question: if a single axi brust address to slave A and its brust size across to slave B, what is the expected behavior?      
         ans: only slave A receive the adress and control message while slave B can't, and then A should resp but B should not resp to master, it will cause the brust transfer fail.
        
    example1:
            if you have the base address like:    
              32’h44a001000, 32’h44a02000, 32’h44a03000…  
      
    they are all 4k align address assignment.  
    because the address [31:12] is the same which means that they are in the same page, the axi brust size should not exceed 4096 bytes.
    
    example2:
            if a read operaion: mrd 0x44a12000 fff,  it can use a single transaction with brusting length 4096 data beats for complete this request.  
    
    example3:
            if a read operaion: mrd 0x44a12000 1fff,  it will use 2 single transaction with each brusting length 4096 data beats for complete this request.  

    
    conclusion:  
               a single write/read operation contain many brust transfer, each brust transfer contain multiple beats transfer or you can use a single beat transfer with the large brust length but it must less than 4096 bytes    
    


in advanced:  
we need to consider about data dependency issue while we need to make sure that the data which assign to the output is updated or not:    
1. RAW
2. WAR
3. WAW
4. RAR
   

note:  
1. single beats in a single transaction only transfer 1 data beats at a time  
![alt text](https://zipcpu.com/img/wbm2axisp/single-master-reads.svg)  
This master uses the RREADY and BREADY signals as states in a state machine to know whether or not it is in the middle of a read or write cycle.   
Once the last acknowledgment is returned, the core returns to idle, lowers RREADY and BREADY, and is then ready to accept a new burst request.  

The key to this being a single beat master is that both AWLEN and ARLEN are held at zero.   
This drastically simplifies the logic of the master:  
1: you can hold WLAST high and ignore RLAST  
2: you don’t need to worry about the maximum burst length, whether or not your burst addresses are incrementing or not, or whether or not the burst will cross a 4kB boundary.  


I'm try to implement this form the reference of  http://zipcpu.com/blog/2020/03/23/wbm2axisp.html  
which is a very well blog for cpu design  





others method for as the axi master you can use 2 method:  
method1: xilinx axi traffic generator  
method2: axi vip  


in axi traffic generator:  
using coe generator to generate the addr.coe and data.coe, follow the tutorial of pg058 to see how to generate the coe file, or you can run vitis platform project with spm to setup the ATG coe   

in axi vip:  
you can use the pkt in your testbench which is suit for runung simulation to config the register of your slave  

