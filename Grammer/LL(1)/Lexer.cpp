#include "head.h"
/**
 * @brief 获取下一个字符
 * 
 * @return string 下一个字符（终结符或非终结符）
 */
string Lexer::nextToken()
{
    if (token.empty())
    {
        string s1;
        for (int i = 0; i < expression.size(); ++i)
        {
            if(expression[i]==' '||expression[i]=='\t')
                continue;
            s1.push_back(expression[i]);
            if (i + 1 < expression.size() && expression[i + 1] == '\'')
            {
                s1.push_back(expression[i + 1]);
                ++i;
            }
            break;
        }
        int i = 1;
        double ans = 0;
        if (s1 == "(" || s1 == ")" || s1 == "-" || s1 == "+" || s1 == "*" || s1 == "/")
        {
            expression.erase(0, 1);
            return s1;
        }
        else
        {
            bool isfloat = false; //是否出现过浮点
            while (i <= expression.size())
            {
                if (expression[i - 1] == '.' || isdigit(expression[i - 1]))
                {
                    if (isfloat && expression[i - 1] == '.')
                    {
                        cout << "error input!\n";
                        cin.get();
                        cin.get();
                        exit(-1);
                    }
                    else if (expression[i - 1] == '.')
                        isfloat = true;
                    ++i;
                }
                else if (i == 1&&s1!="$")
                {
                    cout << "error input!\n";
                    cin.get();
                    cin.get();
                    exit(-1);
                }
                else
                    break;
            }
        }
        int len = expression.size();
        expression.erase(0, i - 1);
        return "n";
    }
    string temp = token;
    token.clear();
    return temp;
}
/**
 * @brief 获取当前处理字符
 * 
 * @return string 当前处理字符（终结符或非终结符）
 */
string Lexer::getToken()
{
    if (token.empty()) //当前无符号
        token = nextToken();
    return token;
}