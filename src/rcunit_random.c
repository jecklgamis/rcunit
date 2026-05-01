/*
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rcunit_random.h"

void rcu_init_random_number() {
    time_t t;
    int seed;
    time(&t);
    seed = t;
    srand(seed);
}

int rcu_gen_random_number(int min, int max) {
    if (min > max) {
        return min;
    }
    return min + (int) ((double)(max - min + 1) * rand() / (RAND_MAX + 1.0));
}

