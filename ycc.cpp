#include <algorithm>
#include <iostream>
#include <preprocessor/preprocessor.h>

using std::string, std::cout, std::cerr, std::endl;

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <list of input files>" << endl;
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    cout << "Processing file: " << argv[i] << endl;
    string ifile = argv[i];

    // ofile = ifile_basename_noext.i
    string ofile = ifile;
    ofile.erase(find(ofile.rbegin(), ofile.rend(), '.').base() - ofile.begin());
    ofile += "i";

    preprocessor::preprocess_file(ifile, ofile);
  }

  return 0;
}
