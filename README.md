# Cipher
### A Secure encrypted method to transmit data in Wireless Area Network.<br>
#### In the repository:<br>
1.[points.cpp](points.cpp) : Testing program implementation of the simple ECC Algo. Algorithm referenced from book "Cryptography and Network Security by Forouzan,Mukhopadl 2nd Edition,10.5"<br>
2.[test.cpp](test.cpp) : Program implementing RSA and ECC Algorithm along with performation of each Algorithms for data security and transmission . Referenced from a [phd thesis submitted by Mohammed Adam Aldod Ibrahim ](https://www.slideshare.net/MohammedAldod/completethesis-52784039)<br>
3.[getratio.awk](getratio.awk) : AWK script the read the information from out0.tr,that will be generated during the execution of program and process the data to find the following:
* Packets Send
* packets Received
* Fraction delivered
* Throughput of System<br>

4.[wrls-aodv.tcl](wrls-aodv.tcl) : Tcl script to simulate the 3 Node (Sender, Receiver, Loss of Packets) for packets transfering ns2 and displaying the packet path with time in xgraph.<br>
5.[ECC_CIPHER.sh](ECC_CIPHER.SH) : Shell script to run all of the above programs.<br>
