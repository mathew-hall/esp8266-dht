/*
 StellarisDS18B20.h - Yes I know it's a long name.

This library works with both 430 and Stellaris using the Energia IDE.

 Not sure who the original author is, as I've seen the core code used in a few different One Wire lib's.
 Am sure it came from Arduino folks.  
 There are no processor hardware dependant calls.
 Had troubles with delayMicrosecond() exiting early when the library used by Stellaris.
 So replaced them with a micros() while loop. The timing loop was tuned to suit the Stellaris
 It was disconcerting that a difference of 1 micro second would make the routines intermittingly fail.
 Anyway after nearly two weeks think I have solved all the delay problems.
 
 History:
 29/Jan/13 - Finished porting MSP430 to Stellaris

 Grant.forest@live.com.au

-----------------------------------------------------------
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

struct sensor_reading* readDS18B20(void);