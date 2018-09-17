#!/bin/bash

# bash script to set paths for installed poreScale library
# set PORESCALE_ROOT before sourcing

if [ -z "${PORESCALE_ROOT+xxx}" ]; then
  echo PORESCALE_ROOT is not set
elif [ -z "${PORESCALE_ROOT}" ] && [ "${PORESCALE_ROOT+xxx}" = "xxx" ]; then
  echo PORESCALE_ROOT is set but empty
else
  export LIBRARY_PATH=$LIBRARY_PATH:$PORESCALE_ROOT/lib
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PORESCALE_ROOT/lib
  export CMAKE_LIBRARY_PATH=$CMAKE_LIBRARY_PATH:$PORESCALE_ROOT/lib

  export INCLUDE_PATH=$INCLUDE_PATH:$PORESCALE_ROOT/include
  export C_INCLUDE_PATH=$C_INCLUDE_PATH:$PORESCALE_ROOT/include
  export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$PORESCALE_ROOT/include
  export CMAKE_INCLUDE_PATH=$CMAKE_INCLUDE_PATH:$PORESCALE_ROOT/include

  echo poreScale paths set
fi


