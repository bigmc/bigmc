#!/bin/sh
cat ../../config.h | sed -n 's/^#define PACKAGE_VERSION "\(.*\)"/\1/p'

