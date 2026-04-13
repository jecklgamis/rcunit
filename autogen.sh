#!/bin/bash
touch NEWS README AUTHORS ChangeLog
autoreconf --force --install
mkdir -p ./build
