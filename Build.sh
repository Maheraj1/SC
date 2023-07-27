#! /usr/bin/sh
# Run this make to build the project

cd Sandbox-Script-Assembly
make &
cd ../SC
make -j4
cd ../SCEditor
make -j2

