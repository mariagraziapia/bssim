/////////////////////////////////////////////////////////////////////////
//
// Summary.cc
// 
// Dump summary information for further analysis 
//
/////////////////////////////////////////////////////////////////////////


#include "Summary.hh"

#include <sstream>
#include <iomanip>

Summary* Summary::instance = 0;

Summary* Summary::Instance()
{
  // A new instance of Summary is created if it does not exist:
  if (instance == 0) 
    {
      instance = new Summary();     
    }
  
  // The instance of AnalysisManager is returned:
  return instance;
}


Summary::Summary()
{
  Initialize();
}

Summary::~Summary()
{
}

void Summary::ResetData() 
{
  nEvents = 0; 
  nBsOverThreshold = 0;
  fileIsOpen = false;
}

void Summary::Initialize() 
{
  nEvents=0;
  nBsOverThreshold = 0;
  fileIsOpen = false;
}


void Summary::Destroy() 
{
  //fileDump.close();
  if (instance != 0) 
    {
      delete instance;
      instance = 0;
    }
}


void Summary::AddBsOverThreshold()
{
  nBsOverThreshold++;  
}


void Summary::OpenFile()
{
  if (!fileIsOpen)
    {   
      char* home;
      G4String pathOut;
      home = getenv("HOME");
      pathOut = "/Documents/reference/g4/backscatter/bsprod/";
      
      std::ostringstream fileName;                   
      
      fileName << home
	       << pathOut
	       << "bsout.txt";
      
      G4String outFileName(fileName.str());
      fileDump.open(outFileName);
      fileIsOpen = true;
    }
}  


void Summary::Dump() 
{
  if (!fileIsOpen)
    {
      OpenFile();
    }

  OneLine();
}


void Summary::OneLine()
{
  fileDump << std::setw(12) << std::setprecision(0) << nEvents << " "
	   << std::setw(12) << std::setprecision(0) << nBsOverThreshold << " "
	   << std::endl;
  
  std::cout << "nEvents = " << nEvents
	    << ", nBackscattered = " << nBsOverThreshold
	    << std::endl;
}
  


