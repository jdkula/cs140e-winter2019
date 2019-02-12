//
// Created by jonak on 2/11/19.
//

#ifndef PIOS_ARM_H
#define PIOS_ARM_H

#include "armtimer.h"

extern volatile rpi_arm_timer_t* get_arm_timer(void);

// TODO: This isn't implemented??
//  return from exception in terms of privilege
//  void lower_privilege(void);

#endif //PIOS_ARM_H
