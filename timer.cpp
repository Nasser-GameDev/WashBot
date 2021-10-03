#include <avr/io.h>




void initTimer0() {
    // set timer to be in CTC mode(page 128 in the data sheet)
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= 1 << WGM01;
    TCCR0B &= ~(1 << WGM02);
}

/**
 * This delays the program an amount specified by unsigned int delay, in
 * milliseconds.
 *
 * Use timer 0. Keep in mind that you need to choose your prescalar wisely
 * such that your timer is precise to 1 millisecond and can be used for
 * 100-200 milliseconds.
 *
 * Note: This function was taken directly from the class slides from lecture 9,
 * written by Dr. Hetherington.
 */
void delayMs(unsigned int delay) {
    unsigned int delayCnt = 0;

    // start the timer at 0 instead of some random junk number
    TCNT0 = 0;

    // set the value that the timer counts up to in order to trigger the
    // interrupt;
    // (16MHz / (1000Hz * 1024)) - 1 = 14.625;
    // we will just use 14;
    // this link is a good resource to help determine what formula to use:
    // https://www.instructables.com/Arduino-Timer-Interrupts
    OCR0A = 16;

    // set compare flag to start the timer
    TIFR0 |= 1 << OCF0A;

    // turn on the clock with the CS bits and start counting;
    // sets the prescaler to 1024
    TCCR0B |= (1 << CS02) | (1 << CS00);

    while (delayCnt < delay) {
        if (TIFR0 & (1 << OCF0A)) {
            // increment only when the flag is set, which means the timer
            // reached the max value of OCR0A
            delayCnt++;

            // restart the timer;
            // the bit will go to 0 before reaching the if statement above
            TIFR0 |= 1 << OCF0A;
        }
    }

    // remember to turn off the clock, by clearing the CS bits that we used
    TCCR0B &= ~((1 << CS02) | (1 << CS00));
}

/**
 * Initialize timer 1, you should not turn the timer on here. Use CTC mode.
 */
void initTimer1() {
	// set timer to be in CTC mode(page 128 in the data sheet)
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B |= 1 << WGM12;
    TCCR1B &= ~(1 << WGM13);
}

/**
 * This delays the program an amount of microseconds specified by unsigned int
 * delay.
 *
 * Use timer 1. Keep in mind that you need to choose your prescalar wisely
 * such that your timer is precise to 1 mircosecond and can be used for
 * 100-200 microseconds.
 *
 * Note: This function was taken directly from the class slides from lecture 9,
 * written by Dr. Hetherington.
 */
void delayUs(unsigned int delay) {
    //delayMs(delay);
    //return;
    unsigned int delayCnt = 0;

    // start the timer at 0 instead of some random junk number
    TCNT1 = 0;

    // set the value that the timer counts up to in order to trigger the
    // interrupt;
    // we will use a prescaler of 1;
    // (16MHz / (1000000Hz * 1)) - 1 = 16 - 1 = 15;
    // this link is a good resource to help determine what formula to use:
    // https://www.instructables.com/Arduino-Timer-Interrupts
    OCR1A = 15;

    // set compare flag to start the timer
    TIFR1 |= 1 << OCF1A;

    // turn on the clock with the CS bits and start counting;
    // sets the prescaler to 1
    TCCR1B |= 1 << CS10;

    while (delayCnt < delay) {
        if (TIFR1 & (1 << OCF1A)) {
            // increment only when the flag is set, which means the timer
            // reached the max value of OCR1A
            delayCnt++;

            // restart the timer;
            // the bit will go to 0 before reaching the if statement above
            TIFR1 |= 1 << OCF1A;
        }
    }

    // remember to turn off the clock, by clearing the CS bits that we used
    TCCR1B &= ~(1 << CS10);
}
