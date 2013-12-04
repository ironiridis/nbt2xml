This C program reads NBT tags, and outputs something that sort of looks like XML.

The C code is written with effectively no checks for sanity and makes bad assumptions about the incoming data. NBT data could be easily constructed to exploit this program and do terrible things to one's computer.

It was written more as an exercise in understanding the structure of the data at the time, so I could later reference it when hacking on other utilities.

Pipe raw NBT data (such as level.dat or a player.dat file) into stdin, and stdout will generate an XMLish representation.
