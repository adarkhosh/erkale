/*
 *                This source code is part of
 * 
 *                     E  R  K  A  L  E
 *                             -
 *                       DFT from Hel
 *
 * Written by Jussi Lehtola, 2010-2011
 * Copyright (c) 2010-2011, Jussi Lehtola
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */



#include "settings.h"
#include "stringutil.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>


Settings::Settings() {
  // Set default Settings
}


Settings::~Settings() {
}

void Settings::add_scf_settings() {
  // Dummy functional: this will be set to HF or a X-C combination
  sset.push_back(gens("Method", "Method used in calculation (HF or a DFT functional)", "Dummy"));

  // Default basis set
  sset.push_back(gens("Basis", "Basis set used in calculation", "aug-cc-pVTZ"));

  // Input system
  sset.push_back(gens("System", "System as an xyz file", "atoms.xyz"));

  // Log file
  sset.push_back(gens("Logfile", "File to print out full information, stdout for screen", "erkale.log"));

  // Use spherical harmonics.
  bset.push_back(genb("UseLM", "Use a spherical harmonics basis set by default?", true));
  
  // Decontract basis set?
  sset.push_back(gens("Decontract","Indices of atoms to decontract basis set for",""));

  // Use DIIS.
  bset.push_back(genb("UseDIIS", "Use Pulay's Direct Inversion in the Iterative Subspace?", true));
  // Use old version of DIIS?
  bset.push_back(genb("C1-DIIS", "Use older version of DIIS (C1-DIIS instead of C2-DIIS)?", false));
  // Number of DIIS matrices to use?
  iset.push_back(geni("DIISOrder", "How many DIIS iterations to keep in memory?", 20));
  // DIIS threshold
  dset.push_back(gend("DIISThr", "DIIS error threshold for DIIS updates", 0.05));

  // Use ADIIS?
  bset.push_back(genb("UseADIIS", "Use ADIIS for Fock matrix interpolation? (DIIS takes preference)", true));
  // Use Broyden mixing?
  bset.push_back(genb("UseBroyden", "Use Broyden mixing of Fock matrices?", false));
  
  // Total charge of system
  iset.push_back(geni("Charge", "Total charge of system", 0));
  // Multiplicity
  iset.push_back(geni("Multiplicity", "Spin multiplicity", 1));
  // Occupancies
  sset.push_back(gens("Occupancies", "Orbital occupancies", ""));

  // Use core guess? Default is atomic.
  bset.push_back(genb("CoreGuess","Use core guess instead of atomic guess",false));

  // Verbose run?
  bset.push_back(genb("Verbose", "Verbose calculation?", true));

  // Direct calculation?
  bset.push_back(genb("Direct", "Calculate two-electron integrals (or density fitting) on-the-fly?", false));

  // Default orthogonalization method
  sset.push_back(gens("BasisOrth", "Method of orthonormalization of basis set", "Can"));

  // Default cutoff for orthogonalization
  dset.push_back(gend("BasisLinTol", "Cutoff for linearly dependent basis functions", 1e-5));

  // Convergence criteria
  dset.push_back(gend("DeltaPrms", "Maximum allowed RMS difference of density matrix", 1e-8));
  dset.push_back(gend("DeltaPmax", "Maximum allowed maximum difference of density matrix", 1e-6));
  dset.push_back(gend("DeltaEmax", "Maximum allowed change of energy", 1e-6));

  // Maximum iterations
  iset.push_back(geni("MaxIter", "Maximum number of iterations in SCF cycle", 100));
}

void Settings::add_dft_settings() {
  // Store full DFT grid in memory?
  bset.push_back(genb("DFTDirect", "Save memory by not storing values of basis functions in memory", false));
  // Store full DFT grid in memory?
  bset.push_back(genb("DFTLobatto", "Use Lobatto quadrature instead of Lebedev quadrature?", false));
  
  // Initial and final tolerances of DFT grid
  dset.push_back(gend("DFTInitialTol", "Tolerance of initial DFT grid", 1e-3));
  dset.push_back(gend("DFTFinalTol", "Tolerance of final DFT grid", 1e-5));
  // Relative factor for initialization
  dset.push_back(gend("DFTDelta", "Switch to final DFT grid, relative to deltaE and deltaP", 100.0));
  
  // Use density fitting if possible?
  bset.push_back(genb("DFTFitting", "Use density fitting if possible? (Pure DFT functionals)", true));
}
  
void Settings::add_double(std::string name, std::string comment, double val) {
  dset.push_back(gend(name,comment,val));
}

void Settings::add_bool(std::string name, std::string comment, bool val) {
  bset.push_back(genb(name,comment,val));
}

void Settings::add_int(std::string name, std::string comment, int val) {
  iset.push_back(geni(name,comment,val));
}

void Settings::add_string(std::string name, std::string comment, std::string val) {
  sset.push_back(gens(name,comment,val));
}

void Settings::set_double(std::string name, double val) {
  if(val<0.0) {
    std::ostringstream oss;
    oss << "Error: settings must have positive value.\n";
    throw std::runtime_error(oss.str());
  }

  // Find setting in table
  for(size_t i=0;i<dset.size();i++)
    if(stricmp(name,dset[i].name)==0) {
      dset[i].val=val;
      return;
    }

  ERROR_INFO();
  std::ostringstream oss;
  oss << "\nThe setting "<<name<<" was not found!\n"; 
  throw std::runtime_error(oss.str());
}

void Settings::set_bool(std::string name, bool val) {
  // Find setting in table
  for(size_t i=0;i<bset.size();i++)
    if(stricmp(name,bset[i].name)==0) {
      bset[i].val=val;
      return;
    }

  ERROR_INFO();
  std::ostringstream oss;
  oss << "\nThe setting "<<name<<" was not found!\n"; 
  throw std::runtime_error(oss.str());
}

void Settings::set_int(std::string name, int val) {
  // Find setting in table
  for(size_t i=0;i<iset.size();i++)
    if(stricmp(name,iset[i].name)==0) {
      iset[i].val=val;
      return;
    }

  ERROR_INFO();
  std::ostringstream oss;
  oss << "\nThe setting "<<name<<" was not found!\n"; 
  throw std::runtime_error(oss.str());
}


void Settings::set_string(std::string name, std::string val) {
  // Find setting in table
  for(size_t i=0;i<sset.size();i++)
    if(stricmp(name,sset[i].name)==0) {
      sset[i].val=val;
      return;
    }

  ERROR_INFO();
  std::ostringstream oss;
  oss << "\nThe setting "<<name<<" was not found!\n"; 
  throw std::runtime_error(oss.str());
}



double Settings::get_double(std::string name) const {
  // Find setting in table
  for(size_t i=0;i<dset.size();i++)
    if(name==dset[i].name) {
      return dset[i].val;
    }

  //  ERROR_INFO();
  std::ostringstream oss;
  oss << "\nThe setting "<<name<<" was not found!\n"; 
  throw std::runtime_error(oss.str());

  return 0.0;
}

bool Settings::get_bool(std::string name) const {
  // Find setting in table
  for(size_t i=0;i<bset.size();i++)
    if(name==bset[i].name) {
      return bset[i].val;
    }

  //  ERROR_INFO();
  std::ostringstream oss;
  oss << "\nThe setting "<<name<<" was not found!\n"; 
  throw std::runtime_error(oss.str());

  return 0;
}

int Settings::get_int(std::string name) const {
  // Find setting in table
  for(size_t i=0;i<iset.size();i++)
    if(name==iset[i].name) {
      return iset[i].val;
    }

  //  ERROR_INFO();
  std::ostringstream oss;
  oss << "\nThe setting "<<name<<" was not found!\n"; 
  throw std::runtime_error(oss.str());

  return 0;
}

std::string Settings::get_string(std::string name) const {
 // Find setting in table
  for(size_t i=0;i<sset.size();i++)
    if(name==sset[i].name) {
      return sset[i].val;
    }

  //  ERROR_INFO();
  std::ostringstream oss;
  oss << "\nThe setting "<<name<<" was not found!\n"; 
  throw std::runtime_error(oss.str());

  return "";
}

size_t Settings::is_double(std::string name) const {
  for(size_t i=0;i<dset.size();i++)
    if(stricmp(name,dset[i].name)==0)
      return i+1;

  return 0;
}

size_t Settings::is_int(std::string name) const {
  for(size_t i=0;i<iset.size();i++)
    if(stricmp(name,iset[i].name)==0)
      return i+1;

  return 0;
}

size_t Settings::is_bool(std::string name) const {
  for(size_t i=0;i<bset.size();i++)
    if(stricmp(name,bset[i].name)==0)
      return i+1;

  return 0;
}

size_t Settings::is_string(std::string name) const {
  for(size_t i=0;i<sset.size();i++)
    if(stricmp(name,sset[i].name)==0)
      return i+1;

  return 0;
}


void Settings::parse(std::string filename) {
  // Input file
  std::ifstream in(filename.c_str());
  
  if(in.good()) {
    // OK, file was succesfully opened.
    
    while(in.good()) {
      // Read line and split it into words
      std::string line=readline(in);
      std::vector<std::string> words=splitline(line);

      if(words.size()) {
	// Parse keywords

	if(words.size()==1) {
	  ERROR_INFO();
	  std::ostringstream oss;
	  oss << "\nParse error: "<<words[0]<<" has no value!\n"; 
	  throw std::runtime_error(oss.str());
	}
	
	if(stricmp(words[0],"Method")==0) {
	  // Hartree-Fock or DFT?
	  if(stricmp(words[1],"Hartree-Fock")==0 || stricmp(words[1],"HF")==0)
	    set_string("Method","HF");
	  else if(stricmp(words[1],"ROHF")==0)
	    set_string("Method","ROHF");
	  else {
	    // Add dft related settings
	    add_dft_settings();
	    set_string("Method",words[1]);
	  }

	} else {
	  if(is_double(words[0])) {
	    set_double(words[0],readdouble(words[1]));
	  } else if(is_int(words[0])) {
	    set_int(words[0],readint(words[1]));
	  } else if(is_bool(words[0])) {
	    // Was the value given as a number or as a string?
	    if(isalpha(words[1][0])) {

	      // As a string - parse it
	      bool value;
	      if(stricmp(words[1],"true")==0)
		value=true;
	      else if(stricmp(words[1],"false")==0)
		value=false;
	      else {
		value=false;

		std::ostringstream oss;
		oss << "Could not parse the truth value " << words[1] << " for setting "<<words[0]<<"!\n";
		throw std::runtime_error(oss.str());
	      }

	      set_bool(words[0],value);
	    } else
	      set_bool(words[0],readint(words[1]));
	  } else if(is_string(words[0])) {
	    // Concatenate value
	    std::string val=words[1];
	    for(size_t i=2;i<words.size();i++)
	      val+=" "+words[i];
	    // Store value
	    set_string(words[0],val);
	  } else {
	    ERROR_INFO();
	    std::ostringstream oss;
	    oss << "\nCannot recognize keyword "<<words[0]<<"!\n"; 
	    throw std::runtime_error(oss.str());
	  }
	}
      }
    }
  }
}

void Settings::print() const {
  printf("\nCurrent Settings used by ERKALE:\n");

  const std::string bvals[]={"false","true"};

  // First, sort the keywords alphabetically.
  std::vector<std::string> kw;
  for(size_t i=0;i<bset.size();i++)
    kw.push_back(bset[i].name);
  for(size_t i=0;i<iset.size();i++)
    kw.push_back(iset[i].name);
  for(size_t i=0;i<dset.size();i++)
    kw.push_back(dset[i].name);
  for(size_t i=0;i<sset.size();i++)
    kw.push_back(sset[i].name);
  std::stable_sort(kw.begin(),kw.end());

  // and then print the list in alphabetic order.
  for(size_t i=0;i<kw.size();i++) {
    size_t is=is_string(kw[i]);
    size_t id=is_double(kw[i]);
    size_t ii=is_int(kw[i]);
    size_t ib=is_bool(kw[i]);

    if(is>0)
      // Is string!
      printf("%5s%-15s\t%20s\t%s\n","",sset[is-1].name.c_str(),sset[is-1].val.c_str(),sset[is-1].comment.c_str());      
    if(id>0)
      // Is double!
      printf("%5s%-15s\t%20.3e\t%s\n","",dset[id-1].name.c_str(),dset[id-1].val,dset[id-1].comment.c_str());
    if(ii>0)
      // Is integer!
      printf("%5s%-15s\t%20i\t%s\n","",iset[ii-1].name.c_str(),iset[ii-1].val,iset[ii-1].comment.c_str());
    if(ib>0)
      // Is boolean!
      printf("%5s%-15s\t%20s\t%s\n","",bset[ib-1].name.c_str(),bvals[bset[ib-1].val].c_str(),bset[ib-1].comment.c_str());
  }
  printf("\n");
}

doubleset_t gend(std::string name, std::string comment, double val) {
  doubleset_t ret;
  ret.name=name;
  ret.comment=comment;
  ret.val=val;
  return ret;
}

boolset_t genb(std::string name, std::string comment, bool val) {
  boolset_t ret;
  ret.name=name;
  ret.comment=comment;
  ret.val=val;
  return ret;
}

intset_t geni(std::string name, std::string comment, int val) {
  intset_t ret;
  ret.name=name;
  ret.comment=comment;
  ret.val=val;
  return ret;
}

stringset_t gens(std::string name, std::string comment, std::string val) {
  stringset_t ret;
  ret.name=name;
  ret.comment=comment;
  ret.val=val;
  return ret;
}
