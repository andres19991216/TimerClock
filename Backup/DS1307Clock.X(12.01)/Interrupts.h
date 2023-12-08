/* 
 * File:   Interrupts.h
 * Author: Boris
 *
 * Created on September 3, 2023, 6:45 AM
 */

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    unsigned char int_cnt = 0;

    void EnableAllInterrupts();
    void DisableAllInterrupts();
    void EnablePeripheralInterrupts();
    void DisablePeripheralInterrupts();
    void EnablePriorityInterrupts();
    void DisablePriorityInterrupts();

#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPTS_H */

