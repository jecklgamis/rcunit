#!/bin/bash
touch NEWS README AUTHORS ChangeLog
autoreconf --force --install
[ ! -d ./build ] && mkdir ./build
