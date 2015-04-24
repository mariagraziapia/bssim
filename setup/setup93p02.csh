# ---- Local definitions -------------------------------------------------
#
# ---- Geant4 definitions ------------------------------------------------
#
# ---- Geant4 data
setenv G4PIIDATA $HOME/Documents/g4dev/dev/data/EMDATA
setenv G4RDDATA $HOME/Documents/g4dev/dev/data/G4RDDATA1.5
#
setenv G4LEDATA $HOME/Documents/g4dev/dev/data/G4EMLOW6.9
setenv G4LEVELGAMMADATA $HOME/Documents/g4dev/dev/data/PhotonEvaporation2.3
setenv G4RADIOACTIVEDATA $HOME/Documents/g4dev/dev/data/RadioactiveDecay3.6
setenv NeutronHPCrossSections $HOME/Documents/g4dev/dev/data/G4NDL4.2
setenv G4ABLA $HOME/Documents/g4dev/dev/data/G4ABLA3.0
setenv G4ELASTIC $HOME/Documents/g4dev/dev/data/G4ELASTIC1.1
#
# ---- Geant4 source
setenv G4INSTALL $HOME/Documents/g4dev/g4_svn/geant4.9.3.p02
#
# ---- Linux setup ----
#
if ( `uname` == Linux ) then
  setenv G4SYSTEM Linux-g++
  echo "You are working on a $G4SYSTEM system"
  setenv G4LIB $HOME/Documents/g4dev/work93/lib/
  setenv G4WORKDIR $HOME/Documents/g4dev/work93
#
# ---- CLHEP directory
setenv CLHEP_BASE_DIR $HOME/Documents/g4dev/dev/CLHEP/2.0.3.2/$G4SYSTEM
#
# ---- set up OpenGL or Mesa
setenv G4VIS_BUILD_OPENGLX_DRIVER   1
setenv G4VIS_USE_OPENGLX            1
#setenv OGLHOME $HOME/dev/Mesa/pro/$G4SYSTEM
setenv OGLHOME /usr
#
# ---- Analysis
#setenv PATH ${PATH}:"/afs/cern.ch/sw/lhcxx/specific/redhat61/gcc-2.95.2/4.0.3/bin"
#source /afs/cern.ch/sw/lhcxx/share/LHCXX/4.0.3/install/sharedstart.csh
#setenv G4ANALYSIS_AIDA_CONFIG_CFLAGS `aida-config --noLicense --include`
#setenv G4ANALYSIS_AIDA_CONFIG_LIBS   `aida-config --noLicense --libs`

eval `aida-config -r csh`
# ---- add to the LD_LIBRARY_PATH 
setenv LD_LIBRARY_PATH "$OGLHOME/lib64:$CLHEP_BASE_DIR/lib:${LD_LIBRARY_PATH}"
#setenv LD_LIBRARY_PATH $CLHEP_BASE_DIR/lib
#setenv LD_LIBRARY_PATH "/users/pia/lib_missing:${LD_LIBRARY_PATH}"

#
# ---- Analysis
#setenv PI_DIR /data/dev/PI/PiUltraLite-1.3.12
#setenv PATH ${PATH}:/data/dev/PI/PiUltraLite-1.3.12/bin
#setenv PATH ${PATH}:"/usr/local/bin/"
#setenv PATH ${PATH}:"$HOME/install_sl6/iAIDA/bin/:$HOME/bin/:$HOME/g4dev/g4_svn/g4svn/:."
#
#eval `aida-config -r csh`
#
# The following should be uncommented
#eval `/usr/local/bin/aida-config -r csh`
#

endif


if ( `uname` == Darwin ) then

# Platform
  setenv G4SYSTEM Darwin-g++
  echo "You are working on a $G4SYSTEM system"

  setenv G4LIB $HOME/Documents/g4dev/work93/lib/
  setenv G4WORKDIR $HOME/Documents/g4dev/work93

# ---- CLHEP directory
  setenv CLHEP_BASE_DIR $HOME/Documents/g4dev/dev/CLHEP/2.0.3.2/$G4SYSTEM

# Enable visualisation
  setenv G4VIS_BUILD_OPENGLX_DRIVER   1
  setenv G4VIS_USE_OPENGLX            1
  setenv OGLHOME /opt/X11
  setenv OGLLIBS "-L/opt/X11/lib -lGL -lGLU"
  eval `/usr/local/bin/aida-config -r csh`
  setenv DYLD_LIBRARY_PATH  $CLHEP_BASE_DIR/lib:$OGLHOME/lib/${DYLD_LIBRARY_PATH}

endif



setenv G4ANALYSIS_USE 1

# ---- Settings to compile physics in different Geant4 versions
setenv G4VERSION_94 1
unsetenv G4VERSION_96
unsetenv G4VERSION_91
unsetenv G4VERSION_101
#

