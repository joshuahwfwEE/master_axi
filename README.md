# axi_lite_master_single_beat
an axi_lite_master_single_beat ip that can be used for control axi_lite_slave interface : 

note:  
single beat master only issues one request at a time  
![alt text](https://zipcpu.com/img/wbm2axisp/single-master-reads.svg)  
This master uses the RREADY and BREADY signals as states in a state machine to know whether or not it is in the middle of a read or write cycle.   
Once the last acknowledgment is returned, the core returns to idle, lowers RREADY and BREADY, and is then ready to accept a new burst request.  

