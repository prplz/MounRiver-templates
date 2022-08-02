#ifndef _WIRING_PULSE_
#define _WIRING_PULSE_

#include <CH32V103.h>

#ifdef __cplusplus
 extern "C" {
#endif

/*
 * \brief Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 */
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
	unsigned long start = micros();
    while (digitalRead(pin) != state) {
        if ((micros() - start) >= timeout) {
            return 0;
        }
    }
    start = micros();
    while (digitalRead(pin) == state) {
        if ((micros() - start) >= timeout) {
            return 0;
        }
    }
    return micros() - start;
	// // cache the port and bit of the pin in order to speed up the
	// // pulse width measuring loop and achieve finer resolution.  calling
	// // digitalRead() instead yields much coarser resolution.
	
	// uint32_t width = 0; // keep initialization out of time critical area
	
	// // convert the timeout from microseconds to a number of times through
	// // the initial loop; it takes 22 clock cycles per iteration.
	// uint32_t numloops = 0;
	// uint32_t maxloops = (timeout*34)/25;
	// // wait for any previous pulse to end
	// while (digitalRead(pin) == state)
	// 	if (numloops++ == maxloops)
	// 		return 0;
	
	
	// // wait for the pulse to start
	// while (digitalRead(pin) != state)
	// 	if (numloops++ == maxloops)
	// 		return 0;
	
	// // wait for the pulse to stop
	// while (digitalRead(pin) == state) {
	// 	if (numloops++ == maxloops)
	// 		return 0;
	// 	width++;
	// }

	// // convert the reading to microseconds. The loop has been determined
	// // to be 52 clock cycles long and have about 16 clocks between the edge
	// // and the start of the loop. There will be some error introduced by
	// // the interrupt handlers
	// return uint32_t((width*25)/34);
}


#ifdef __cplusplus
}
#endif

#endif /* _WIRING_PULSE_ */