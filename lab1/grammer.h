#include <iostream>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <memory>

using namespace std;

string IdentifierStr; // Filled in if tok_identifier
string NumStr;

enum token{
  IDENT = 257, NUM, RETURN, INT, EOF_TOKEN
};

map<string, int> keywords;
set<char> symbol{';', '(', ')', '{', '}'};
int token;
fstream char_stream;