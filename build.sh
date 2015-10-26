#!/bin/bash
gcc `pkg-config --cflags gtk+-3.0` -Wall -o bifidGUI bifidGUI.c `pkg-config --libs gtk+-3.0`
