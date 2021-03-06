/*
 * Copyright (c) 2015 Martin Sidaway
 * 
 * A perpetual, worldwide, non-exclusive, no-charge, royalty-free, irrevocable
 * copyright and patent license is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation (and in any combination) the rights to use, copy, modify,
 * merge, reimplement, publish, distribute, sublicense, and/or sell copies of
 * the Software (and/or portions thereof), and to permit persons to whom the
 * Software is furnished to do so. This license includes a waiver of the
 * authors', copyright-holders' and (where applicable) patent-holders' rights
 * to exclude such activities on the basis of present or future patent
 * ownership and/or confer such rights of exclusion onto others, insofar as
 * such patents pertain to the patent-holders' contributions to the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT OR PATENT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_atexit.h"

typedef struct my_atexit_data {
    void (*func)(void *);
    void *data;
    struct my_atexit_data *next;
} my_atexit_data;

static my_atexit_data *my_atexit_handlers = NULL;

static void my_atexit_callback() {
    my_atexit_data *handler = NULL;
    while (my_atexit_handlers != NULL) {
        handler = my_atexit_handlers;
        my_atexit_handlers = my_atexit_handlers->next;
        handler->func(handler->data);
        free(handler);
    }
}

static int my_atexit_initialized = 0;

void my_atexit(void (*func)(void *), void *data) {
    if (my_atexit_initialized == 0) {
        my_atexit_initialized = 1;
        atexit(my_atexit_callback);
    }
    my_atexit_data *new_handler =
        (my_atexit_data *) malloc(sizeof(my_atexit_data));
    if (new_handler == NULL) {
        return;
    }
    new_handler->func = func;
    new_handler->data = data;
    new_handler->next = my_atexit_handlers;
    my_atexit_handlers = new_handler;
}
