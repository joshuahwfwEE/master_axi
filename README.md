# axi_lite_master
axi_lite_master_single_beat ip that can be used for control axi_lite_slave interface :
axi_lite_master_continous_beat ip that can be used for control axi_lite_slave interface :

the difference of both of them is the throughput of data, in the same time continous beat can process multiple read or write operation each beat while single beat process 1 operation each beat

note:  
single beat master only issues one request at a time  
![alt text](https://zipcpu.com/img/wbm2axisp/single-master-reads.svg)  
This master uses the RREADY and BREADY signals as states in a state machine to know whether or not it is in the middle of a read or write cycle.   
Once the last acknowledgment is returned, the core returns to idle, lowers RREADY and BREADY, and is then ready to accept a new burst request.  

The key to this being a single beat master is that both AWLEN and ARLEN are held at zero.   
This drastically simplifies the logic of the master:  
1: you can hold WLAST high and ignore RLAST  
2: you don’t need to worry about the maximum burst length, whether or not your burst addresses are incrementing or not, or whether or not the burst will cross a 4kB boundary.  

this example will provide a simple single beat transfer, and single beat with pipeline  
and then we will discuss the brusting with single channel and then the brusting with multiple channel






what is the axi 4k boundry:  
each axi brust can not over 4kB, because the minimum page size of axi slave is 4kB  
the reason is if a single axi brust that is over than 4kB, it may access across different slaves ( because all slave is 4k/1k aligned)  
ex: if a single axi brust address to slave A and its brust size across to slave B, what is the expected behavior?  
only slave A receive the adress and control message while slave B can't, and then A should resp but B should not resp to master, it will cause the brust transfer fail.  

if you have the base address like:  
32’h44a001000, 32’h44a02000, 32’h44a03000…  
it is a 4k align address assignment.
because the address [31:12] is the same which means that they are in the same page, the axi brust size should not exceed 4096 bytes  

a single write/read operation contain many brust transfer, each brust transfer contain multiple beats transfer or you can use a single beat transfer with the large brust length while it must less than 4096 bytes  

and then combine with aw operation and r/w data can form a transcation  
hence if a read operaion: mrd 0x44a12000 fff,  it will use a single brust to complete this transfer
if a read operaion: mrd 0x44a12000 1fff,  it will use 2 single brust to complete this transfer  

and so on if you have the different bus width, it still can use the same brust size to complete the transaction  

the limitation of single beat master is the lower throughput, the purpose focus on the comprehension of how it works  



if you search for another high throughput, you can use 2 method:  
method1: xilinx axi traffic generator  
method2: axi vip  
method3: hls axi_lite_master

in axi traffic generator:  
using coe generator to generate the addr.coe and data.coe, follow the tutorial of pg058 to see how to generate the coe file, or you can run vitis platform project with spm to setup the ATG coe   

in axi vip:  
you can use the pkt in your testbench which is suit for runung simulation to config the register of your slave  

in hls axi_lite_config_master has 2 part:  
reg_write:  
reg_read:  
