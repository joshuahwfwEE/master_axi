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

the limitation of single beat master is the lower throughput, the purpose focus on the comprehension of how it works

if you search for high throughput, you can use 2 method:  
method1: xilinx axi traffic generator  
method2: axi vip  
method3: hls axi_lite_master

in axi traffic generator:  
using coe generator to generate the addr.coe and data.coe  

in hls axi_lite_config_master has 2 part:  
reg_write:  
reg_read:  
