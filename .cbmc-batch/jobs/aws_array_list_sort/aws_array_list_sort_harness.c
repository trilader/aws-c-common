/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <aws/common/array_list.h>
#include <proof_helpers/make_common_data_structures.h>

/**
 * Standard implementation of compare function for qsort
 */
size_t item_size;
int compare(const void *a, const void *b) {
    __CPROVER_precondition(__CPROVER_r_ok(a, item_size), "first element readable in compare function");
    __CPROVER_precondition(__CPROVER_r_ok(b, item_size), "second element readable in compare function");
    return nondet_int();
}

/**
 * Runtime: 12s
 */
void aws_array_list_sort_harness() {
    /* data structure */
    struct aws_array_list list;

    /* assumptions */
    __CPROVER_assume(aws_array_list_is_bounded(&list, MAX_INITIAL_ITEM_ALLOCATION, MAX_ITEM_SIZE));
    ensure_array_list_has_allocated_data_member(&list);
    __CPROVER_assume(aws_array_list_is_valid(&list));

    /* save current state of the data structure */
    struct aws_array_list old = list;
    struct store_byte_from_buffer old_byte;
    save_byte_from_array((uint8_t *)list.data, list.current_size, &old_byte);

    /* perform operation under verification */
    item_size = list.item_size;
    aws_array_list_sort(&list, compare);

    /* assertions */
    assert(aws_array_list_is_valid(&list));
    assert_array_list_equivalence(&list, &old, &old_byte);
}
