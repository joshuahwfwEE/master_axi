# axi_lite_master_single_beat
an axi_lite_master_single_beat ip that can be used for control axi_lite_slave interface : 

note:  
single beat master only issues one request at a time  
![alt text](https://zipcpu.com/img/wbm2axisp/single-master-reads.svg)  
This master uses the RREADY and BREADY signals as states in a state machine to know whether or not it is in the middle of a read or write cycle.   
Once the last acknowledgment is returned, the core returns to idle, lowers RREADY and BREADY, and is then ready to accept a new burst request.  

The key to this being a single beat master is that both AWLEN and ARLEN are held at zero.   
This drastically simplifies the logic of the master:  
1: you can hold WLAST high and ignore RLAST  
2: you don’t need to worry about the maximum burst length, whether or not your burst addresses are incrementing or not, or whether or not the burst will cross a 4kB boundary.
