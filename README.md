# PIC-16f877a-SPI-master-to-slave
There are many examples of this on the web, but most don't seem to work. 
Pics of the breadboard used are in the "issues" (on the right)
Here is some C code for the MPLAB X IDE using the XC8 compiler, which seems to be quite stable for me. 
It enables two 16f877a devices to communicate with each other via SPI and what makes this code different to others, is the use of a watchdog timer to check if the "BF" flag has changed state in an acceptable period of time (Set as 1/8th of a second). 
If this housekeeping is not done, then in certain situations, the code can either:
- Lock while waiting for the "BF" to change 
- Start receiving rubbish data (half of previous byte transmission mangled with current byte). This happens mostly when the devices are first powered on in differing sequences. 

N.B. There is no attempt to rescue the data here - if it is deemed to be mangled by the interupt firing, then the data is discarded because the SPI module is refreshed. (SSPEN = 0 and then SSPEN = 1).
Also, I think the packet speed of data constellations should be low here, so that there is time for any unfinished data in SSPBUF to be discarded/flushed if the interupt fires (after a 1/8 second timeout). One could use this code to quickly fire a sentence of several bytes across the SPI (with little pause between them), but to pause before sending the next sentence (e.g. 1/2 second).

One interesting side effect of this code, is that the slave does not really require a power supply because the "clock" line idles "high". This means that the "slave" 16f877A receives its power from the "clock" line !!!

I think this code is best utilised for ad hoc data downloading from a "master" to "slave". 
Later on. I hope to publish code for data going the other way i.e. a "master" device signals to a "slave" that it wants to read data from it (i.e. data comes back from the "slave" after being "kicked").

Hope this code proves useful for you - Enjoy!
