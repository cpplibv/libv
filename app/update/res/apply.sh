#!/usr/bin/env bash

cd "$(dirname "$0")"

function apply {
	echo "$2 -> $1";
	mkdir ./tmp
	cp -rT $2 ./tmp
	../../../bin/apply_update --path ./tmp --patch $3
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
apply version/v00 version/v00 update/test-dev-00-00.update
apply version/v01 version/v00 update/test-dev-01-00.update
apply version/v02 version/v00 update/test-dev-02-00.update
apply version/v03 version/v00 update/test-dev-03-00.update
apply version/v04 version/v00 update/test-dev-04-00.update

apply version/v00 version/v01 update/test-dev-00-01.update
apply version/v01 version/v01 update/test-dev-01-01.update
apply version/v02 version/v01 update/test-dev-02-01.update
apply version/v03 version/v01 update/test-dev-03-01.update
apply version/v04 version/v01 update/test-dev-04-01.update

apply version/v00 version/v02 update/test-dev-00-02.update
apply version/v01 version/v02 update/test-dev-01-02.update
apply version/v02 version/v02 update/test-dev-02-02.update
apply version/v03 version/v02 update/test-dev-03-02.update
apply version/v04 version/v02 update/test-dev-04-02.update

apply version/v00 version/v03 update/test-dev-00-03.update
apply version/v01 version/v03 update/test-dev-01-03.update
apply version/v02 version/v03 update/test-dev-02-03.update
apply version/v03 version/v03 update/test-dev-03-03.update
apply version/v04 version/v03 update/test-dev-04-03.update

apply version/v00 version/v04 update/test-dev-00-04.update
apply version/v01 version/v04 update/test-dev-01-04.update
apply version/v02 version/v04 update/test-dev-02-04.update
apply version/v03 version/v04 update/test-dev-03-04.update
apply version/v04 version/v04 update/test-dev-04-04.update
