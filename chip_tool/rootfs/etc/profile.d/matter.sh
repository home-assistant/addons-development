#!/bin/sh

# Store Matter fabric data in a persistent location
export TMPDIR="/data"

cat <<"EOF"
                 _   _            
 _ __ ___   __ _| |_| |_ ___ _ __ 
| '_ ` _ \ / _` | __| __/ _ \ '__|
| | | | | | (_| | |_| ||  __/ |   
|_| |_| |_|\__,_|\__|\__\___|_|   
                                  
Use `chip-tool` to run CHIP Tool from command line.

Use `chip-tool interactive start` to start an interactive CHIP Tool shell. Use
`quit()` to exit the CHIP Tool shell.

Check /usr/local/bin for more examples. Fabric data is stored in /data, consider
clearing to reset the state of the Matter fabric.

EOF
