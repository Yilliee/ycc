#include <algorithm>
#include <iostream>

using std::string, std::cout, std::cerr, std::endl;

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <list of input files>" << endl;
    return 1;
  }
  string ifile = argv[1];

  // ofile = ifile_basename_noext.i
  string ofile = ifile;
  ofile.erase(find(ofile.rbegin(), ofile.rend(), '.').base() - ofile.begin());
  ofile += "i";

  return 0;
}
