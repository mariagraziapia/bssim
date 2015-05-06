/////////////////////////////////////////////////////////////////////////
//
// Summary.hh
// 
// Dump summary information for further analysis 
//
/////////////////////////////////////////////////////////////////////////


#ifndef SUMMARY_HH
#define SUMMARY_HH

#include "globals.hh"
#include <fstream>

class Summary  {

public:

  static Summary* Instance();

  void Initialize();
  void ResetData();

  // The class instance can be deleted by calling the Destroy method
  // NOTE: The class destructor is protected, therefore it cannot be called directly

  static void Destroy();

  void AddBsOverThreshold();
  void SetEvents(int nGenerated) { nEvents = nGenerated; }
 
  void Dump();

protected:

  // Non publicly accessible constructor and destructor
  Summary();
  virtual ~Summary();

  // Prevent copying
  Summary(const Summary& only);
  const Summary& operator=(const Summary& only);

private:
  // The static instance of the DataFrameDetecor class
  static Summary* instance;

  void OneLine();
  void OpenFile();
  
  int nEvents;
  int nBsOverThreshold;
  
  bool fileIsOpen;
  std::ofstream fileDump;
};

#endif // SUMMARY_HH
