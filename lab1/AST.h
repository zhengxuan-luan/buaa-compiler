#include "grammer.h"
// ! parser AST
/*
先定义出非终结符对应的类，再定义出构造这些类的parse函数
对于一个nonterminal有多个产生式，是否应该使用继承？
*/

class Expr{
public:
  virtual ~Expr() = default;
  virtual void codegen() = 0;
};

class NumExpr: public Expr{ //一个数字组成的表达式
 int value;
public:
  NumExpr(int value): value(value) {}
  void codegen();
};

class Stmt{ 
public:
  virtual ~Stmt() = default;
  virtual void codegen() = 0;
};

class ReturnStmt: public Stmt{ //return语句
  unique_ptr<Expr> expr;
public:
  ReturnStmt(unique_ptr<Expr>& expr): expr(move(expr)) {}
  void codegen();
};

class Block{
  unique_ptr<Stmt> stmt;
public:
  Block(unique_ptr<Stmt>& stmt): stmt(move(stmt)) {}
  void codegen();
};

class FuncDef{
  int functype;
  string funcname;
  unique_ptr<Block> block;
public:
  FuncDef(int type, const string& name, unique_ptr<Block> block): \
  functype(type), funcname(name), block(move(block)) {}
  void codegen();
};

class CompUnit{
  unique_ptr<FuncDef> funcdef;
public:
  CompUnit(unique_ptr<FuncDef>& funcdef): funcdef(move(funcdef)) {}
  void codegen();
};

void NumExpr::codegen(){
  cout<<"i32 "<<value;
}

void ReturnStmt::codegen(){
  cout<<"ret ";
  expr->codegen();
  cout<<endl;
}

void Block::codegen(){
  cout<<"{"<<endl;
  stmt->codegen();;
  cout<<"}";
}

void FuncDef::codegen(){
  cout<<"define dso_local ";
  if(functype == INT)
    cout<<"i32 ";
  cout<<"@"<<funcname<<"(";
  cout<<")";
  block->codegen();
}

void CompUnit::codegen(){
  funcdef->codegen();
}