#!/bin/bash

cmake .
make
./NimServer "$@"
