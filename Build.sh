#! /usr/bin/sh
# Run this make to build the project

cd external
make MT
cd ../SC
make -j10
cd ../Sandbox-Script-Assembly
make
cd ../SCEditor
make -j2

