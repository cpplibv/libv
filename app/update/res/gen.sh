#!/usr/bin/env bash

function create {
	echo "$2 -> $1";
	../../../bin/create_patch --force --new $1 --old $2 --patch $3
	echo '';
}


create v01 v00 p01.00.patch

create v02 v00 p02.00.patch
create v02 v01 p02.01.patch

create v03 v00 p03.00.patch
create v03 v01 p03.01.patch
create v03 v02 p03.02.patch

create v04 v00 p04.00.patch
create v04 v01 p04.01.patch
create v04 v02 p04.02.patch
create v04 v03 p04.03.patch
