#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <fstream>

using namespace std;

/*
首先统一读取直到空格，先标记为id，再去匹配关键词set
= ? == 判断时都生成等号，在输出时进行区别
*/

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

enum token{
  ERR=255, ID, NUM, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN, EOF_TOKEN, EQ
};
set<int> symbol{'+', ';', '(', ')', '{', '}', '*', '/', '<', '>', '='};
map<string, int> keywords;

int isnondigit(int c){
  if(isalpha(c) | c=='_')
    return 1;
  else
    return 0;
}

int gettok(fstream &cf){   //返回TOKEN，再由TOKEN配合全局变量指导输出
  static int LastChar = ' ';
  //static 不会每次都执行
  // Skip any whitespace.

  while (isspace(LastChar)){
    LastChar = cf.get();
  }
  
  if(LastChar == '='){
    LastChar = cf.get();
    if(LastChar == '='){
      LastChar = cf.get();
      return EQ;
    } 
    else{
      if(!isspace(LastChar))
        return ERR;
      return '='; //此时LastChar为space
    }
  }
  if(symbol.count(LastChar)){
    char c = LastChar;
    LastChar = cf.get();
    return c;
  }
    
  if(isnondigit(LastChar)){ //标识符：是否为关键词
    IdentifierStr.clear();
    do{
      IdentifierStr += LastChar;
      LastChar = cf.get();
    }while(isdigit(LastChar) | isnondigit(LastChar));

    if(keywords.count(IdentifierStr)){
      return keywords[IdentifierStr];
    }
    return ID;
  }

  if(isdigit(LastChar)){
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = cf.get();
    } while (isdigit(LastChar));
    NumVal = strtod(NumStr.c_str(), nullptr);
    return NUM;
  }

  if(LastChar == EOF)
    return EOF_TOKEN;
  
  return ERR;
}

int main(int argc, char *argv[]){
  keywords["if"] = IF;
  keywords["continue"] = CONTINUE;
  keywords["else"] = ELSE;
  keywords["while"] = WHILE;
  keywords["break"] = BREAK;
  keywords["return"] = RETURN;
  fstream char_stream;
  char_stream.open(argv[1], ios::in);
  int tok;
  do{
    tok = gettok(char_stream);
    switch(tok){
      case ID: 
        cout<<"Ident("<<IdentifierStr<<')'<<endl;break;
        break;
      case NUM:
        cout<<"Number("<<NumVal<<')'<<endl;break;
      case '+':
        cout<<"Plus"<<endl;break;
      case '*':
        cout<<"Mult"<<endl;break;
      case '/':
        cout<<"Div"<<endl;break;
      case '<':
        cout<<"Lt"<<endl;break;
      case '>': 
        cout<<"Gt"<<endl; break;
      case EQ:
        cout<<"Eq"<<endl; break;
      case '=':
        cout<<"Assign"<<endl; break;
      case ';':
        cout<<"Semicolon"<<endl; break;
      case '(':
        cout<<"LPar"<<endl; break;
      case ')':
        cout<<"RPar"<<endl; break;
      case '{':
        cout<<"LBrace"<<endl; break;
      case '}':
        cout<<"RBrace"<<endl; break;
      case IF:
        cout<<"If"<<endl; break;  
      case CONTINUE:
        cout<<"Continue"<<endl; break; 
      case RETURN:
        cout<<"Return"<<endl; break; 
      case ELSE:
        cout<<"Else"<<endl; break; 
      case BREAK:
        cout<<"Break"<<endl; break; 
      case WHILE:
        cout<<"While"<<endl; break;     
      default:
        break;
    }
  }while(tok != EOF_TOKEN && tok != ERR);
  char_stream.close();
  if(tok == ERR)
  cout<<"Err"<<endl;
  return 0;
}