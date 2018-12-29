# PIC-16f877a-SPI-master-to-slave


There are many examples of this on the web, but most don't seem to work well. The attached examples of C code are used for "master" and "slave" PICF887A circuits, and data transmission works reliably for me.

I think this code is best utilised for ad hoc data downloading from a "master" to "slave". 
Later on. I hope to publish code for data going the other way i.e. a "master" device signals to a "slave" that it wants to read data from it (i.e. data comes back from the "slave" after being "kicked").

One interesting side effect of this code, is that the "Slave" does not require a separate power supply because it receives its power from the  "clock" line (mostly from when it idles "high"). 

The programs were generated using the MPLAB X IDE with the XC8 compiler. 

The pictures below show how the "Master" and "Slave" circuits are wired up on small breadboards.

The "Master" and "slave" programs are loaded into separte 16f877a devices and it enables data to be communicated between them using  SPI. What makes this code a bit different to others I've found, is the use of a watchdog timer to check if the "BF" flag has changed state in an acceptable period of time (Set as 1/8th of a second). The inclusion of a watchdog timer in the code acts as "unlocking housekeeping" i.e. after 1/8 second, it enables the system to recover itself from a lockup which coudl have happened becuase of a data transmission error.  

If for some reason this "unlocking" housekeeping is not done, then in certain situations the code is locked because:
- the system is waiting for the "BF" to change 
- the system received rubbish data (half of previous byte transmission mangled with current byte). This happens mostly when the devices are first powered on in differing sequences. 

N.B. There is no attempt to rescue the data here - if it is deemed to be mangled by the interupt firing, then the data is discarded because the SPI module is refreshed. (SSPEN = 0 and then SSPEN = 1).
Also, I think the packet speed of data constellations should be low here, so that there is time for any unfinished data in SSPBUF to be discarded/flushed if the interupt fires (after a 1/8 second timeout). One could use this code to quickly fire a sentence of several bytes across the SPI (with little pause between them), but to pause before sending the next sentence (e.g. 1/2 second).

Hope this code proves useful for you - Enjoy!

![overall](https://cloud.githubusercontent.com/assets/11596479/7616550/1ca92f42-f99d-11e4-9ff4-5d42fb73bb45.JPG)
![a bit closer](https://cloud.githubusercontent.com/assets/11596479/7616551/1eb78248-f99d-11e4-892b-3eb5dba142f5.JPG)
![master](https://cloud.githubusercontent.com/assets/11596479/7616552/20fbba92-f99d-11e4-9212-e5de540faa5b.JPG)
![slave](https://cloud.githubusercontent.com/assets/11596479/7616555/28d18684-f99d-11e4-969c-db8ff1e0fd5b.JPG)
