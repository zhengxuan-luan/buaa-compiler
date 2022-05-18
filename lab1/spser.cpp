#include "AST.h"

/*
首先统一读取直到空格，先标记为id，再去匹配关键词set
= ? == 判断时都生成等号，在输出时进行区别
*/

inline int isnondigit(int c){
  if(isalpha(c) || c=='_')
    return 1;
  else
    return 0;
}

inline int isnonzerodigit(int c){
  if(isdigit(c) && c)
    return 1;
  else
    return 0;
}

inline int ishexalpha(int c){
  if((c >= 'a' && c<= 'f') || (c <= 'F' && c >= 'A'))
    return 1;
  else 
    return 0;
}

inline int isoctalpha(int c){
  if( c >= '1' && c<= '9')
    return 1;
  else 
    return 0;
}

// ! scanner

int gettok(){   //返回TOKEN，再由TOKEN配合全局变量指导输出
  static int LastChar = ' ';
  //static 不会每次都执行
  // Skip any whitespace.

  //handle with coment
  while (isspace(LastChar)){
    LastChar = char_stream.get();
  }
  if(LastChar == '/'){
    LastChar = char_stream.get();
    if(LastChar == '/'){
      do{
        LastChar = char_stream.get();
      }  while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');
      return gettok();
    }
    if(LastChar == '*'){
      int flag = 0;
      do{
        LastChar = char_stream.get();
        if(LastChar == '*'){
          LastChar = char_stream.get();
          if(LastChar == '/')
            flag = 1;
        }
      }while(!flag);
      return gettok();
    }  
    return '/';
  }

  if(symbol.count(LastChar)){
    char c = LastChar;
    LastChar = char_stream.get();
    return c;
  }

  if(isnondigit(LastChar)){ //标识符：是否为关键词
    IdentifierStr.clear();
    do{
      IdentifierStr += LastChar;
      LastChar = char_stream.get();
    }while(isdigit(LastChar) | isnondigit(LastChar));

    if(keywords.count(IdentifierStr)){
      return keywords[IdentifierStr];
    }
    return IDENT;
  }
  // hex or oct
  if(LastChar == '0'){
    NumStr.clear();
    LastChar = char_stream.get();
    if(LastChar == 'x'){
      do {
        NumStr += LastChar;
        LastChar = char_stream.get();
      } while (ishexalpha(LastChar));
    }
    else{
      do {
        NumStr += LastChar;
        LastChar = char_stream.get();
      } while (isoctalpha(LastChar));
    }
    
    return NUM;
  }

  if(isnonzerodigit(LastChar)){
    NumStr.clear();
    do {
      NumStr += LastChar;
      LastChar = char_stream.get();
    } while (isdigit(LastChar));
    return NUM;
  }

  if(LastChar == EOF)
    return EOF_TOKEN;
  
  exit(3);
}

void match(int tk) {
  if (token != tk) {
      printf("expected token: %d, got: %d\n", tk, token);
      exit(3);
  }
  token = gettok();
}


// ! recuisive descent 
/*
每次读取一个token， 再根据token进行递归下降
基类的handle根据预测进行switch
一个AST的成员应该尽量是基类AST
派生类的handle构建AST
*/
int handle_num(){
  if(NumStr[0] != 0){
    return stoi(NumStr, 0, 10);
  }
  else{
    if(NumStr[1] == 'x'){      
      return stoi(NumStr.substr(2), 0, 16);
    }
    else{
      return stoi(NumStr.substr(1), 0, 8);
    }
  } 
}

unique_ptr<Expr> handle_Expr(){  //一般表达式
  if(token == NUM){
    int value =  handle_num();
    // int token = gettok();
    auto result = make_unique<NumExpr>(value);
    return move(result);
  }
  exit(3);
}

unique_ptr<ReturnStmt> handle_ReturnStmt(){
  match(RETURN);
  unique_ptr<Expr> expr = handle_Expr();
  match(NUM);
  match(';');
  auto result = make_unique<ReturnStmt>(expr);
  return move(result);
}

unique_ptr<Stmt> handle_Stmt(){
  switch(token){
    case RETURN:
      return handle_ReturnStmt();
    default:
      break;
  }
  exit(3);
}

unique_ptr<Block> handle_Block(){
  match('{');
  auto pStmt = handle_Stmt();
  match('}');
  return move(make_unique<Block>(pStmt));
}


unique_ptr<FuncDef> handle_FuncDef(){
  match(INT);
  auto functype = INT;  
  match(IDENT);
  auto funcname = IdentifierStr;
  match('(');
  match(')');
  auto block = handle_Block();
  return move(make_unique<FuncDef>(functype, funcname, move(block)));
}

// 这里首token必然是TYPE，直接Handle_funcdef，token不动（作为基类handle，他本来就应该不动）
unique_ptr<CompUnit> handle_CompUnit(){
  auto def = handle_FuncDef();
  return move(make_unique<CompUnit>(def));
}

int main(int argc, char *argv[]){
  keywords["int"] = INT;
  keywords["return"] = RETURN;
  char_stream.open(argv[1], ios::in);
  token = gettok();
  auto result = handle_CompUnit();
  result->codegen();
  char_stream.close();
  return 0;
}
