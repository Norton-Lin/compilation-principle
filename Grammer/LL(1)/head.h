#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <set>
#include <cmath>
#include <unordered_map>
#include <exception>
using namespace std;
//词法分析器（分析算术表达式）
class Lexer
{
public:
    string token;
    string expression;
    string nextToken();          //获取下一个字符
    string getToken();           //获取字符
    int anaylseToken(string &s); //解析获取字符
    Lexer()
    {
    }
    Lexer(const string &s)
    {
        this->expression = s + "$";
        for (auto it = expression.begin(); it != expression.end();)
        {
            if (*it == ' ')
                expression.erase(it++);
            else
                it++;
        }
    }
};
//语法分析器
class Grammer
{
public:
    string startSymbol = "E";
    set<string> nonTerminalSymbol;                                //非终结符
    set<string> terminalSymbol;                                   //终结符
    unordered_map<string, vector<string>> generateExpression;     //生成式
    unordered_map<string, bool> nullJudge;                        //空值判断
    unordered_map<string, set<string>> firstSet;                  // First集
    unordered_map<string, set<string>> followSet;                 // Follow集
    unordered_map<string, unordered_map<string, string>> LLtable; //预测分析表
    string getNonterminalSymbol(string s);                        //获取一个新的非终结符
    void removeLeftRecusion();                                    //消除左递归
    void getNullSet();                                            //获取空值集
    void getFirstSet();                                           //获取first集
    void getFollowSet();                                          //获取follow集
    void normalizeSet();                                          // First集，Follow集规范化
    void printMessage();                                          //输出消除左递归后的文法、follow集、first集
    vector<string> getFirst(string &expression);                  //获取对应first集
    void buildLLTable();                                          //建立预测分析表
    void printLLTable();
    Grammer()
    {
    }
};
//表达式分析器
class LLParser
{
public:
    Grammer grammer;  //语法分析器
    Lexer lexer;      //词法分析器
    stack<string> st; //分析栈
    LLParser(Grammer g, string expression)
    {
        grammer = g;
        lexer = Lexer(expression);
        st.push("$");
        st.push(g.startSymbol);
    }
    void parse(); //逐步分析
};