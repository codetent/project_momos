#!/bin/bash

cd tools

python3 -m momos build \
        -i ../src/states.c \
        -o ../test/test_states.cpp \
        --state-var ../src/states.c:current_state \
        --fn-init logic.h:logic_init \
        --fn-update logic.h:logic_run
