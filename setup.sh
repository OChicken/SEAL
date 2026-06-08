#!/bin/bash -x

# Compile SEAL.
cd src
make rebuild
cd ..

# Create symbolic links for SEAL Python module.
echo "$PWD/sealpy/src/" > $HOME/.local/lib/python3.13/site-packages/__editable__.seal-0.0.1.pth

# Create symbolic links for SEAL library.
mkdir -p $HOME/.local/include/seal/
ln -fs $PWD/src/*.h           $HOME/.local/include/seal/
ln -fs $PWD/src/emulator.so   $HOME/.local/lib/libsealemulator.so
ln -fs $PWD/src/seal.so       $HOME/.local/lib/libseal.so
ln -fs $PWD/src/sealsym.so    $HOME/.local/lib/libsealsym.so
ln -fs $PWD/src/sealscript.so $HOME/.local/lib/libsealscript.so
