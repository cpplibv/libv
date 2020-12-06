#!/usr/bin/env bash

cd "$(dirname "$0")"

function create {
	echo "$2 -> $1";
	../../../bin/create_patch --force --new $1 --old $2 --patch $3
	echo '';
}

# Forward patches
# create v01 v00 p01.00.patch
#
# create v02 v00 p02.00.patch
# create v02 v01 p02.01.patch
#
# create v03 v00 p03.00.patch
# create v03 v01 p03.01.patch
# create v03 v02 p03.02.patch
#
# create v04 v00 p04.00.patch
# create v04 v01 p04.01.patch
# create v04 v02 p04.02.patch
# create v04 v03 p04.03.patch

# ALL
create v00 v00 p00.00.patch
create v01 v00 p01.00.patch
create v02 v00 p02.00.patch
create v03 v00 p03.00.patch
create v04 v00 p04.00.patch

create v00 v01 p00.01.patch
create v01 v01 p01.01.patch
create v02 v01 p02.01.patch
create v03 v01 p03.01.patch
create v04 v01 p04.01.patch

create v00 v02 p00.02.patch
create v01 v02 p01.02.patch
create v02 v02 p02.02.patch
create v03 v02 p03.02.patch
create v04 v02 p04.02.patch

create v00 v03 p00.03.patch
create v01 v03 p01.03.patch
create v02 v03 p02.03.patch
create v03 v03 p03.03.patch
create v04 v03 p04.03.patch

create v00 v04 p00.04.patch
create v01 v04 p01.04.patch
create v02 v04 p02.04.patch
create v03 v04 p03.04.patch
create v04 v04 p04.04.patch
