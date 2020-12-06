#!/usr/bin/env bash

cd "$(dirname "$0")"

function apply {
	echo "$2 -> $1";
	mkdir ./tmp
	cp -rT $2 ./tmp
	../../../bin/apply_patch --path ./tmp --patch $3
	diff --brief --recursive $1 ./tmp \
			&& echo 'Verification: SUCCESS' \
			|| echo 'Verification: FAILURE';
	rm -rf ./tmp
	echo '';
}

# Forward patches
# apply v01 v00 p01.00.patch
#
# apply v02 v00 p02.00.patch
# apply v02 v01 p02.01.patch
#
# apply v03 v00 p03.00.patch
# apply v03 v01 p03.01.patch
# apply v03 v02 p03.02.patch
#
# apply v04 v00 p04.00.patch
# apply v04 v01 p04.01.patch
# apply v04 v02 p04.02.patch
# apply v04 v03 p04.03.patch

# ALL
apply v00 v00 p00.00.patch
apply v01 v00 p01.00.patch
apply v02 v00 p02.00.patch
apply v03 v00 p03.00.patch
apply v04 v00 p04.00.patch

apply v00 v01 p00.01.patch
apply v01 v01 p01.01.patch
apply v02 v01 p02.01.patch
apply v03 v01 p03.01.patch
apply v04 v01 p04.01.patch

apply v00 v02 p00.02.patch
apply v01 v02 p01.02.patch
apply v02 v02 p02.02.patch
apply v03 v02 p03.02.patch
apply v04 v02 p04.02.patch

apply v00 v03 p00.03.patch
apply v01 v03 p01.03.patch
apply v02 v03 p02.03.patch
apply v03 v03 p03.03.patch
apply v04 v03 p04.03.patch

apply v00 v04 p00.04.patch
apply v01 v04 p01.04.patch
apply v02 v04 p02.04.patch
apply v03 v04 p03.04.patch
apply v04 v04 p04.04.patch
