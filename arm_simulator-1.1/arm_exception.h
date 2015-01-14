#ifndef __ARM_EXCEPTION_H__
#define __ARM_EXCEPTION_H__


#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void arm_exception(arm_core p, unsigned char exception);

#endif
