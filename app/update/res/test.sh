#!/usr/bin/env bash

cd "$(dirname "$0")"

./gen.sh
./apply.sh
