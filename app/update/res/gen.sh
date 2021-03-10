#!/usr/bin/env bash

cd "$(dirname "$0")"

function create {
	echo "$2 -> $1";
	../../../bin/create_update --force --new $1 --old $2 --patch $3
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
create version/v00 version/v00 update/test-dev-00-00.update
create version/v01 version/v00 update/test-dev-01-00.update
create version/v02 version/v00 update/test-dev-02-00.update
create version/v03 version/v00 update/test-dev-03-00.update
create version/v04 version/v00 update/test-dev-04-00.update

create version/v00 version/v01 update/test-dev-00-01.update
create version/v01 version/v01 update/test-dev-01-01.update
create version/v02 version/v01 update/test-dev-02-01.update
create version/v03 version/v01 update/test-dev-03-01.update
create version/v04 version/v01 update/test-dev-04-01.update

create version/v00 version/v02 update/test-dev-00-02.update
create version/v01 version/v02 update/test-dev-01-02.update
create version/v02 version/v02 update/test-dev-02-02.update
create version/v03 version/v02 update/test-dev-03-02.update
create version/v04 version/v02 update/test-dev-04-02.update

create version/v00 version/v03 update/test-dev-00-03.update
create version/v01 version/v03 update/test-dev-01-03.update
create version/v02 version/v03 update/test-dev-02-03.update
create version/v03 version/v03 update/test-dev-03-03.update
create version/v04 version/v03 update/test-dev-04-03.update

create version/v00 version/v04 update/test-dev-00-04.update
create version/v01 version/v04 update/test-dev-01-04.update
create version/v02 version/v04 update/test-dev-02-04.update
create version/v03 version/v04 update/test-dev-03-04.update
create version/v04 version/v04 update/test-dev-04-04.update
