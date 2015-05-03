# PIC-16f877a-SPI-master-to-slave
There are many examples of this on the web, but most don't seem to work. 
Here is some C code for the MPLAB X IDE using the XC8 compiler. 
It enables two 16f877a devices to communicate with each other via SPI. 
What makes this code different to others, is the use of timer0 to check if the "BF" flag
has changed state in an acceptable period of time (Set as 1/8th of a second). 
If this housekeeping is not done, then in certain situations, the code will either:
- Lock at this point
- Start transmitting rubbish data (half of previous byte transmission mangled with current byte). This happens mostly when the devices are first powered on in differing sequences. 

N.B. There is no attempt here to rescue the data, if it is mangled then it is ignored.
Also , it relies on the packet speed of data to be greater than the timeout period.
This code is best utilised for ad hoc data downloading of data from a "master" to "slave".
One interesting side effect is that the slave does not require a power supply - it is powered by the "clock" line because it idles "high".
Later on. I hope to publish other code - for a master to signal to a slave that it wants to read from it i.e. data comes back from the slave.

Hope this code proves useful for you - Enjoy!
