#ifndef _TARGET_H_
#define _TARGET_H_
#include <iostream>
#include <string>

const int maxDepends = 10;

struct Target {
  std::string name;
  std::ostream &out;
  int numDepends = 0;
  Target *depends[maxDepends];
  int updated = 0;
  
  bool existDepend(const std::string &d) {  // helper to see if the dependant d exists in *depends
    for (int i = 0; i < numDepends; ++i) {
      if (depends[i]->name == d) {
        return true;
      }
    }
    return false;
  }

  Target(const std::string &name, std::ostream &out = std::cout): name{name}, out{out} {}

  bool isLeaf() const {  // if there is no dependants its a leaf 
      return (numDepends == 0);
  }

  void addDepend(Target *d) {  // adds dependant up to 10 max dependants
    if (numDepends == maxDepends) {
      std::cout << "Max dependencies exceeded" << std::endl;  // print Max dependencies exceeded
    } else if (!existDepend(d->name)) {
      depends[numDepends] = d;
      ++numDepends;
    }
  }

  void update(int time) {  // updates the file with the current time 
    if (updated != time) {
      updated = time;
    }
  }

  int lastUpdated() const {   // gets the last updated
    return updated;
  }

  void build() {
    if (!isLeaf()) {
      int mostRecentUp = lastUpdated();
      for (int i = 0; i < numDepends; ++i) {
        depends[i]->build();                            // recursively call the dependants to update 
        if (depends[i]->lastUpdated() > mostRecentUp) { // get the most recent latest update
          mostRecentUp = depends[i]->lastUpdated();
        }
      }
      if (mostRecentUp > updated) {   // update if not updated
        for (int i = 0; i < numDepends; ++i) {
          depends[i]->update(mostRecentUp);  // update all dependants
        }
        updated = mostRecentUp;
        std::cout << "Building " << name << std::endl;  // print building and name of file
      }
    }
  }
};

#endif
