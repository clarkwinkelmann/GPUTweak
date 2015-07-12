#!/bin/bash
#
# Compilation script for Linux 64bit
# This file is part of the GPUTweak project, see README
# Copyright (C) 2015 Clark Winkelmann
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
echo 'Starting compilation script...'

profile='src/GPUTweak.pro'
exefile='GPUTweak'
compiledir='linux-compile-64'
distdir='GPUTweak'
archivename='GPUTweak_Linux_64bit.tar.gz'

echo 'Cleaning directories...'
rm -rf $compiledir
rm -rf $distdir
mkdir $compiledir
mkdir $distdir
cd $compiledir

echo 'Running qmake...'
qmake ../"$profile" -spec linux-g++-64 "CONFIG+=release"

echo 'Running make...'
make

echo 'Copying files...'
cd ..
cp $compiledir/$exefile $distdir/$exefile
cp README.md  $distdir/README.md
cp LICENSE  $distdir/LICENSE

echo 'Packaging app...'
tar czvf $archivename $distdir

echo 'Finished.'

