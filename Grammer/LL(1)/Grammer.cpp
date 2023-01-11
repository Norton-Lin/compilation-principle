#include "head.h"
string Grammer::getNonterminalSymbol(string s) //获取一个新的非终结符
{
    nonTerminalSymbol.insert(s + "'");
    return s + "'";
}
void Grammer::removeLeftRecusion() //消除左递归
{
    for (auto &key : generateExpression)
    {
        vector<string> value = key.second;
        vector<string> left;
        for (auto &s : value) //构建左递归表达式集合
        {
            if (s.substr(0, key.first.size()) == key.first)
                left.emplace_back(s);
        }
        if (!left.empty())
        {
            string newSymbol = getNonterminalSymbol(key.first);
            vector<string> temp;
            for (auto &x : value)
            {
                if (x.substr(0, key.first.size()) != key.first)
                    temp.emplace_back(x + newSymbol);
            }
            generateExpression[key.first] = temp;
            temp.clear();
            for (auto &x : left)
                temp.emplace_back(x.substr(key.first.size()) + newSymbol);
            temp.emplace_back("");
            generateExpression[newSymbol] = temp;
        }
    }
}
void Grammer::getNullSet() //获取空值集
{
    unordered_map<string, bool> oldNullJedge; //控制判断
    for (auto &x : nonTerminalSymbol)
        nullJudge[x] = false;
    while (oldNullJedge != nullJudge)
    {
        oldNullJedge = nullJudge;
        for (auto &x : generateExpression)
        {
            nullJudge[x.first] = false;
            for (auto &expression : x.second)
            {
                bool allowNull = true;
                int i = 0;
                if (expression.empty()) //为空
                {
                    nullJudge[x.first] = true;
                }
                while (i < expression.size())
                {
                    string s;
                    bool judge = true;
                    s.push_back(expression[i]);
                    if (i + 1 < expression.size() && expression[i + 1] == '\'')
                    {
                        s.push_back(expression[i + 1]);
                        i++;
                    }
                    i++;
                    if (nonTerminalSymbol.find(s) == nonTerminalSymbol.end()) // s是终结符
                    {
                        allowNull = false;
                    }
                    else //是非终结符
                    {
                        for (auto &t : generateExpression[s])
                        {
                            if (t == "")
                            {
                                judge = false;
                                break;
                            }
                        }
                        if (judge)
                            allowNull = false;
                    }
                    nullJudge[x.first] |= allowNull;
                }
            }
        }
    }
}
void Grammer::getFirstSet() //获取first集
{
    unordered_map<string, set<string>> oldFirstSet;
    for (auto &x : nonTerminalSymbol)
        firstSet[x] = set<string>();
    for (auto &x : generateExpression)
    {
        for (auto &expression : x.second)
        {
            int i = 0;
            while (i < expression.size())
            {
                string s;
                bool judge = true;
                s.push_back(expression[i]);
                if (i + 1 < expression.size() && expression[i + 1] == '\'')
                {
                    s.push_back(expression[i + 1]);
                    i++;
                }
                i++;
                if (nonTerminalSymbol.find(s) == nonTerminalSymbol.end()) // s是终结符
                {
                    firstSet[s] = set<string>();
                    firstSet[s].insert(s);
                }
            }
        }
    }
    while (oldFirstSet != firstSet)
    {
        oldFirstSet = firstSet;
        for (auto &x : generateExpression)
        {
            for (auto &expression : x.second)
            {
                bool addNew = true; //添加新字符
                int i = 0;
                if (expression.empty())
                {
                    firstSet[x.first].insert("");
                }
                while (i < expression.size())
                {
                    string s;
                    bool judge = true;
                    s.push_back(expression[i]);
                    if (i + 1 < expression.size() && expression[i + 1] == '\'')
                    {
                        s.push_back(expression[i + 1]);
                        i++;
                    }
                    i++;
                    if (addNew)
                    {
                        firstSet[x.first].insert(firstSet[s].begin(), firstSet[s].end());
                        addNew = addNew && (nonTerminalSymbol.find(s) != nonTerminalSymbol.end() && nullJudge[s]);
                    }
                }
            }
        }
    }
}
void Grammer::getFollowSet() //获取follow集
{
    unordered_map<string, set<string>> oldFollowSet;
    for (auto &x : nonTerminalSymbol)
        followSet[x] = set<string>();
    for (auto &x : generateExpression)
    {
        for (auto &expression : x.second)
        {
            int i = 0;
            while (i < expression.size())
            {
                string s;
                bool judge = true;
                s.push_back(expression[i]);
                if (i + 1 < expression.size() && expression[i + 1] == '\'')
                {
                    s.push_back(expression[i + 1]);
                    i++;
                }
                i++;
                if (nonTerminalSymbol.find(s) == nonTerminalSymbol.end()) // s是终结符
                {
                    followSet[s] = set<string>();
                    followSet[s].insert(s);
                }
            }
        }
    }
    followSet[startSymbol].insert("$");
    while (oldFollowSet != followSet)
    {
        oldFollowSet = followSet;
        for (auto &x : generateExpression)
        {
            for (auto &expression : x.second)
            {
                for (int i = 0; i < expression.size(); ++i)
                {
                    bool addNew = true;
                    string s1, s2;
                    s1.push_back(expression[i]);
                    if (i + 1 < expression.size() && expression[i + 1] == '\'')
                    {
                        s1.push_back(expression[i + 1]);
                        ++i;
                    }
                    if (nonTerminalSymbol.find(s1) == nonTerminalSymbol.end())
                    {
                        continue;
                    }
                    for (int j = i + 1; j < expression.size();)
                    {
                        s2.clear();
                        s2.push_back(expression[j]);
                        if (j + 1 < expression.size() && expression[j + 1] == '\'')
                        {
                            s2.push_back(expression[j + 1]);
                            ++j;
                        }
                        addNew = addNew && (nonTerminalSymbol.find(s2) != nonTerminalSymbol.end() && nullJudge[s2]);
                        ++j;
                    }
                    if (addNew)
                    {
                        followSet[s1].insert(followSet[x.first].begin(), followSet[x.first].end());
                    }
                    for (int j = i + 1; j < expression.size(); ++j)
                    {
                        addNew = true;
                        string s3;
                        s3.push_back(expression[j]);
                        if (j + 1 < expression.size() && expression[j + 1] == '\'')
                        {
                            s3.push_back(expression[j + 1]);
                            ++j;
                        }
                        for (int k = i + 1; k < j;)
                        {
                            s2.clear();
                            s2.push_back(expression[k]);
                            if (k + 1 < expression.size() && expression[k + 1] == '\'')
                            {
                                s2.push_back(expression[k + 1]);
                                ++k;
                            }
                            ++k;
                            addNew = addNew && (nonTerminalSymbol.find(s2) != nonTerminalSymbol.end() && nullJudge[s2]);
                        }
                        if (addNew)
                        {
                            followSet[s1].insert(firstSet[s3].begin(), firstSet[s3].end());
                        }
                    }
                }
            }
        }
    }
}
void Grammer::normalizeSet() //follow集、first集规范化
{
    for (auto it = firstSet.begin(); it != firstSet.end();)
    {
        if (nonTerminalSymbol.find(it->first) == nonTerminalSymbol.end())
            firstSet.erase(it++);
        else
            it++;
    }
    for (auto it = followSet.begin(); it != followSet.end();)
    {
        if (nonTerminalSymbol.find(it->first) == nonTerminalSymbol.end())
            followSet.erase(it++);
        else
        {
            it->second.erase("");
            it++;
        }
    }
}
void Grammer::printMessage() //输出消除左递归后的文法、follow集、first集
{
    cout << "Grammer: " << endl;
    for (auto &x : generateExpression)
    {
        for (auto &y : x.second)
            cout << x.first << " -> " << y << endl;
    }
    cout << "First Set:" << endl;
    for (auto &x : firstSet)
    {
        cout << x.first << ": { ";
        int count = 0;
        for (auto &y : x.second)
        {
            cout << "'" << y << "'";
            if (++count < x.second.size())
                cout << " ,";
        }
        cout << " }" << endl;
    }
    cout << "Follow Set:" << endl;
    for (auto &x : followSet)
    {
        cout << x.first << ": { ";
        int count = 0;
        for (auto &y : x.second)
        {
            cout << "'" << y << "'";
            if (++count < x.second.size())
                cout << " ,";
        }
        cout << " }" << endl;
    }
}
vector<string> Grammer::getFirst(string &expression) //获取对应first集字符
{
    vector<string> ans;
    for (int i = 0; i < expression.size(); ++i)
    {
        string s1;
        s1.push_back(expression[i]);
        if (i + 1 < expression.size() && expression[i + 1] == '\'')
        {
            s1.push_back(expression[i + 1]);
            ++i;
        }
        if (nonTerminalSymbol.find(s1) != nonTerminalSymbol.end()) //这是非终结符
        {
            for (auto &x : firstSet[s1])
            {
                if (!x.empty())
                    ans.emplace_back(x);
            }
            if (!nullJudge[s1])
                return ans;
        }
        else //这是终结符
        {
            ans.emplace_back(s1);
            break;
        }
    }
    return ans;
}
void Grammer::buildLLTable() //建立预测分析表
{
    for (auto &x : nonTerminalSymbol)
        LLtable[x] = {};
    for (auto &x : generateExpression)
    {
        for (auto &expression : x.second)
        {
            if (expression.empty())
            {
                for (auto &fo : followSet[x.first])
                    LLtable[x.first][fo] = expression;
            }
            else
            {
                for (auto &fi : getFirst(expression))
                    LLtable[x.first][fi] = expression;
            }
        }
    }
}
void Grammer::printLLTable() //输出预测分析表
{
    cout<<"Forecast Analysis Table:"<<endl;
    cout.width(10);
    cout << ""<<left;
    for (auto &x : terminalSymbol)
    {
        cout.width(10);
        cout << x;
    }
    cout.width(10);
    cout << "$" << endl;
    for (auto &x : nonTerminalSymbol)
    {
        cout.width(10);
        cout << x;
        for (auto &y : terminalSymbol)
        {
            cout.width(10);
            if (LLtable[x].find(y) != LLtable[x].end())
                cout << x + "->" + LLtable[x][y];
            else
                cout << "-";
        }
        cout.width(10);
        if (LLtable[x].find("$") != LLtable[x].end())
                cout << x + "->" + LLtable[x]["$"];
        else
            cout << "-";
        cout << endl;
    }
}