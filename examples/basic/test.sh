#!/bin/bash

python3 -m momos build \
        -i ../src/states.c \
        -o ../test/test_states.cpp \
        --data-type int \
        --state-var ../src/states.c:current_state \
        --fn-init logic.h:logic_init \
        --fn-update logic.h:logic_run
