#include"head.h"
int main()
{
    Grammer* g = new Grammer();
    g->nonTerminalSymbol.insert("E");
    g->generateExpression["E"] = {"E+T","E-T","T"};
    g->nonTerminalSymbol.insert("T");
    g->generateExpression["T"] = {"T*F","T/F","F"};
    g->nonTerminalSymbol.insert("F");
    g->generateExpression["F"] = {"(E)","n"};
    g->terminalSymbol.insert("n");
    g->terminalSymbol.insert("*");
    g->terminalSymbol.insert("/");
    g->terminalSymbol.insert("+");
    g->terminalSymbol.insert("-");
    g->terminalSymbol.insert("(");
    g->terminalSymbol.insert(")");
    g->removeLeftRecusion();
    g->getNullSet();
    g->getFirstSet();
    g->getFollowSet();
    g->normalizeSet();
    g->printMessage();
    g->buildLLTable();
    g->printLLTable();
    string s;
    cout<<"Please input the expression"<<endl;
    cin>>s;
    LLParser* ll= new LLParser(*g,s);
    ll->parse();
    if(ll->lexer.expression == "$")
        cout<<"Accept!"<<endl;
    else
        cout<<"Can't accept!"<<endl;
    cin.get();
    cin.get();
    delete g,ll;
    return 0;
}