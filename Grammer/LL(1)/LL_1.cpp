#include "head.h"
void LLParser::parse()
{
    while (st.top() != "$")
    {
        if (grammer.nonTerminalSymbol.find(st.top()) == grammer.nonTerminalSymbol.end()) //栈顶不是非终结符，消去
        {
            if (st.top() == lexer.getToken())
            {
                st.pop();
                lexer.nextToken();
            }
        }
        else //栈顶是非终结符 规约
        {
            string expression = grammer.LLtable[st.top()][lexer.getToken()];

            cout << st.top() << " ->" ;
            cout.width(10);
            cout<< expression;
            stack<string> temp = st;
            vector<string> temp1 ;
            while(!temp.empty())
            {
                temp1.emplace_back(temp.top());
                temp.pop();
            }
            for(auto& x:temp1)
                cout<<x;
            cout.width(10);
            cout << "\t\tinput:" << this->lexer.expression << endl;
            st.pop();
            for (int i = expression.size() - 1; i >= 0; --i)
            {
                string s1;
                s1.push_back(expression[i]);
                if (expression[i] == '\'')
                {
                    s1.insert(0, 1, expression[i - 1]);
                    --i;
                }
                st.push(s1); //栈加入新项
            }
        }
    }
}