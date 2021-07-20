#!/bin/bash

# auto input the "123"
passwd anonymous << ENTER_PWD
123
123
ENTER_PWD

# or when using gdisk/fdisk, etc.
gdis /dev/sdX << PART_CMD
x
c
5
${ROOT_UUID}
w
y
PART_CMD
