#pragma once

#ifndef UNIT_TEST
    #include <avr/io.h>
#else
    #include "../../extras/forced_include.h"
    #include "../../extras/include/avr/io.h"
#endif

void init_spi();

