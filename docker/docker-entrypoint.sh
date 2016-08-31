#!/bin/bash
set -e

export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/liblldb-3.8.so

cd /home/llnode/test

node lldbdemo.js
