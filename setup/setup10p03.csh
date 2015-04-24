#!/bin/tcsh
# This script defines some environment variables needed for the exercise

# ---- Location of Geant4 data
setenv G4PIIDATA $HOME/Documents/g4dev/dev/data/G4PIIDATA1.3
setenv G4LEDATA $HOME/Documents/g4dev/dev/data/G4EMLOW6.35
setenv G4LEVELGAMMADATA $HOME/Documents/g4dev/dev/data/PhotonEvaporation3.0
setenv G4RADIOACTIVEDATA $HOME/Documents/g4dev/dev/data/RadioactiveDecay4.0
setenv G4NEUTRONHPDATA $HOME/Documents/g4dev/dev/data/G4NDL4.4
#
# ---- Location of Geant4 source files
setenv G4INSTALL $HOME/Documents/g4dev/g4_svn/geant4.10.00.p03

# Location of work directory where libraries, executables etc. are placed
  setenv G4WORKDIR $HOME/Documents/g4dev/work103
  setenv G4LIB $HOME/Documents/g4dev/work103/lib/
#
# ---- Linux setup ----
#
if ( `uname` == Linux ) then

# Platform
  setenv G4SYSTEM Linux-g++
  echo "You are working on a $G4SYSTEM system"

setenv CLHEP_BASE_DIR /home/pia/Documents/g4dev/dev/CLHEP/2.1.3.1/$G4SYSTEM

# Disable analysis
#setenv G4ANALYSIS_USE 0

# Enable visualisation
  setenv G4VIS_BUILD_OPENGLX_DRIVER   1
  setenv G4VIS_USE_OPENGLX            1
  setenv OGLHOME /usr

  eval `aida-config -r csh`
  setenv LD_LIBRARY_PATH "$OGLHOME/lib64:$CLHEP_BASE_DIR/lib:${LD_LIBRARY_PATH}"
#  setenv LD_LIBRARY_PATH "$OGLHOME/lib64"

endif

if ( `uname` == Darwin ) then

# Platform
  setenv G4SYSTEM Darwin-g++
  echo "You are working on a $G4SYSTEM system"

# ---- CLHEP directory
#setenv CLHEP_BASE_DIR $HOME/Documents/g4dev/dev/CLHEP/2.1.3.1/$G4SYSTEM

# Enable visualisation
  setenv G4VIS_BUILD_OPENGLX_DRIVER   1
  setenv G4VIS_USE_OPENGLX            1
  setenv OGLHOME /opt/X11
  setenv OGLLIBS "-L/opt/X11/lib -lGL -lGLU"
  eval `/usr/local/bin/aida-config -r csh`
  #setenv DYLD_LIBRARY_PATH  $CLHEP_BASE_DIR/lib:$OGLHOME/lib/${DYLD_LIBRARY_PATH}
  setenv DYLD_LIBRARY_PATH  $OGLHOME/lib/${DYLD_LIBRARY_PATH}


endif


setenv G4ANALYSIS_USE 1

setenv G4VERSION_96 1
unsetenv G4VERSION_91
unsetenv G4VERSION_94
unsetenv G4VERSION_101

