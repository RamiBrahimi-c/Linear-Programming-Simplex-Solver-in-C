#!/bin/sh


set -xe


gcc src/linear_program.c src/main.c src/color_printf.c src/parser.c src/fraction.c -lm -o bin/main