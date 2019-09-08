# Core Rope Memory

The 50th anniversary of the Apollo mission brought around lots of projects and talks about the technology that made the mission possible. One item that always fascinated me is "magnetic core memory", a type of RAM made of tiny ferrite beads that store a bit by being magnetized in one direction or another. A close relative of "magnetic core memory" is "core rope memory" which, instead, is a ROM.

I started to experiment with a toroid core I had in my parts bin and, after a bit of fumbling, came up with a single bit that could be driven directly by a microcontroller, whithout additional driver circuitry. Let's be clear, this is a bit stretching things as the drive line is a dead short to ground and relies solely on the current limiation of your microcontroller output circuitry. While this might not be best practice, and it surely isn't, it works beautifully at least on the Atmega 328. With other microcontrollers your mileage might vary from either not having enough current through the drive line to actually releasing a puff of smoke of death. Experiment at your own peril!

![bit](docs/bit.png)

The concept in depicted in the diagram above). When Drive0 is excited with a pulse a few volts pulse is generated in the sense coil. The peak detector formed by the diode and the capacitor gives the microcontroller time to read the pulse after it's done driving the circuit. In this case, because Drive0 passes inside the core, the bit is detected as a "1". The second drive line in the example instead doesn't pass the core and so will not generate a pulse on the sense coil when it's excited, in this case the bit will be read as a "0".

