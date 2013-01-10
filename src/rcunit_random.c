/*
 * RCUNIT - A unit testing framework for C.
 * Copyright (C) 2006 Jerrico L. Gamis
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 *
 */

#include <rcunit.h>

#define ENABLE_RECOMPUTE_CHECKSUM_HERE 1

/**
 * @brief Initialize random number generator
 */
RCU_INT rcu_init_random_number(){
time_t t;
RCU_U4 seed;
    time(&t);
    seed =  t;
    srand(seed);
    return(RCU_E_OK);
}

/**
 * @brief Generates random integer number
 * @param[in] min - Lower limit
 * @param[in] max - Upper limit
 */
RCU_INT rcu_gen_random_number(RCU_U4 min, RCU_U4 max){
RCU_INT j;
    j=min+(RCU_INT) (((double)max)*rand()/(RAND_MAX+((double)min)));
    return(j);
}

