#ifndef _MAKEFILE_
#define _MAKEFILE_
#include <string>
#include <iostream>
#include "target.h"

const int maxTargets = 20;
struct Makefile {
  std::ostream &out;
  Target **targets;
  int numTargets = 0;
  int time = 0;
  
  bool existTarget(const std::string &t) {  // helper that returns true if the Target t exists in **targets
    for (int i = 0; i < numTargets; ++i) {  // false otherwise
      if (targets[i]->name == t) {
        return true;
      }
    }
    return false;
  }

  int findTarget(const std::string &t) {  // helper that returns the index of Target t exists in **targets
    for (int i = 0; i < numTargets; ++i) {  // -1 otherwise. Meant to be used after existTarget so -1 never gets returned
      if (targets[i]->name == t) {
        return i;
      }
    }
    return -1;
  }

  // Constructor
  Makefile(std::ostream &out = std::cout): out{out} {
    targets = new Target*[maxTargets];
  }

  // Destructor
  ~Makefile() {
    for (int i = 0; i < numTargets; ++i) {
      delete targets[i];
    }
    delete [] targets;
  }

  // adds a dependant to a Target if it does not exceed the Maximum Targets
  void addDepend(const std::string &t, const std::string &d) {
    Target *tar;
    Target *dep;
    if (numTargets == maxTargets) {  // if max has been reached
      std::cout << "Max targets exceeded" << std::endl;
    } else if (existTarget(t)) {          // if Target exists
      if (existTarget(d)) {              // if Target and Dependant exist
        targets[findTarget(t)]->addDepend(targets[findTarget(d)]); 
      } else {                  // if Target exists but Dependant does not exist
        dep = insertTarget(d);  // create dependant
        targets[findTarget(t)]->addDepend(dep); 
      }
    } else if (!existTarget(t)) {  
      if (existTarget(d)) {     // if Target does not exist but Dependant exists
        tar = insertTarget(t);  // create Target
        tar->addDepend(targets[findTarget(d)]); 
      } else {  // if Target nor Dependant exist
        if (!(numTargets + 1 == maxTargets)) { // check if there is space for two Targets
          tar = insertTarget(t);  // create both
          dep = insertTarget(d);
          tar->addDepend(dep);
        } else {
          std::cout << "Max targets exceeded" << std::endl; // Max targets if at 19 files and user adds
        }                                                   // two unique targets
      }
    }
  }

  void changeTarget(const std::string &t) {  // touches/updates a leaf object
    ++time;
    if (existTarget(t)) {
      if (targets[findTarget(t)]->isLeaf()) {
        targets[findTarget(t)]->update(time);
        std::cout << t << " updated at time " << time << std::endl;  // prints updated with time
      } else {
        std::cout << "Cannot update non-leaf object" << std::endl;  // prints if not a leaf object
      }
    }
  }

  void rebuild(const std::string &t) {  // starts the updating the selected file
    if (existTarget(t)) {
      targets[findTarget(t)]->build();
    }
  }

  Target *insertTarget(const std::string &t) { // inserts Target t
    targets[numTargets] = new Target(t);
    ++numTargets;  // add to the count
    return targets[numTargets-1]; // return the recently added Target
  }
};

#endif
