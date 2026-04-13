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


#ifndef RCUNIT_MEM_H
#define RCUNIT_MEM_H

#include <stdio.h>
#include <stdlib.h>
#include "rcunit_types.h"

extern RCU_API void *rcu_malloc(size_t size);

extern RCU_API void rcu_free(void *addr);

void *rcu_native_malloc(size_t size);

void rcu_native_free(void *addr);

rcu_memcell *rcu_alloc_mem_cell(size_t nr_mem_cell);

void rcu_free_mem_cell(rcu_memcell **cell);

#endif /* RCUNIT_MEM_H */


