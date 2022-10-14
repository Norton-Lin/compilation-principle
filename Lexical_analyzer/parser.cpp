#include "parser.h"
int Parser::get_char() //从文件读取一个字符
{
    if (!file.eof())
    {
        C = file.get();
        colCount++;
        charCount++;
        if (C == '\n')
        {
            lineCount++;
            colCount = 0;
        }
        if (file.fail())
            return -1;
        return 0;
    }
}
int Parser::get_nbc() //检查C中字符是否为空格，是则反复调用get_char直到不是空格
{
    while (!file.eof() && (C == ' ' || C == '\n' || C == '\t' || C == '\r'))
    {
        get_char();
        if (file.fail())
            return -1;
    }
    return 0;
}
void Parser::cat() //将C中的字符连接到token后
{
    token.push_back(C);
}
bool Parser::letter() //判断字符是否是字符
{
    return (C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z') || C == '_';
}
bool Parser::digit() //判断字符是否是数字
{
    return (C >= '0' && C <= '9');
}
void Parser::retract() //读取位置回退
{
    file.seekg(-1, ios_base::cur);
    char temp;
    temp = file.get();
    if (temp == '\n')
        lineCount--;
    file.seekg(-1, ios_base::cur);
    colCount--;
    charCount--;
}
KeyWord Parser::reserve() //查询单词是否是关键字
{
    unordered_map<string, int> keyword = {
        {"auto", 0},
        {"short", 1},
        {"int", 2},
        {"long", 3},
        {"float", 4},
        {"double", 5},
        {"char", 6},
        {"struct", 7},
        {"union", 8},
        {"enum", 9},
        {"typedef", 10},
        {"const", 11},
        {"unsigned", 12},
        {"signed", 13},
        {"extern", 14},
        {"register", 15},
        {"static", 16},
        {"volatile", 17},
        {"void", 18},
        {"if", 19},
        {"else", 20},
        {"switch", 21},
        {"case", 22},
        {"default", 23},
        {"for", 24},
        {"do", 25},
        {"while", 26},
        {"continue", 27},
        {"break", 28},
        {"goto", 29},
        {"sizeof", 30},
        {"return", 31}};
    if (keyword.find(token) != keyword.end())
        return (KeyWord)keyword[token];
    return KeyWord::not_key;
}
long long Parser::SToI() //将token中的字符串转换为整数
{
    return stoi(token);
}
double Parser::SToF() //将token中的字符串转换为浮点数
{
    return stod(token);
}
void Parser::returnWord(Token type, string str) //识别出单词、关键词、符号、字符、字符串、分隔符、错误
{
    if (type == Token::id)
    {
        if (symbolTable.find(str) == symbolTable.end())
            symbolTable[str] = symbolTable.size();
        out << "< id, " << symbolTable[str] << " >" << endl;
    }
    else if (type == Token::oper || type == Token::delimiter || type == Token::keyword)
        out << "< " << str << ", "
            << "- >" << endl;
    else if (type == Token::unknown)
    {
        out << "Error!  Position:  line: " << line << "  col: " << col << endl;
        printToken(type);
    }
    else if (type == Token::character)
        out << "< " << str << ", "
            << "char >" << endl;
    else
        out << "< " << str << ", "
            << "string >" << endl;
    if (mode)
    {
        out << "Position:  line: " << line << "  col: " << col << endl;
        out << "Value: " << str << endl
            << endl;
    }
    wordCount[int(type)]++;
}
void Parser::returnDigit(Token type, long long int num) //识别出常数
{
    if (type == Token::digit)
    {
        if (digitTable.find(num) == digitTable.end())
            digitTable[num] = digitTable.size();
        out << "< id, " << digitTable[num] << " >" << endl;
    }
    wordCount[int(type)]++;
    if (mode)
    {
        out << "Position:  line: " << line << "  col: " << col << endl;
        printToken(type);
        out << "Value: " << num << endl
            << endl;
    }
}
void Parser::returnNumber(Token type, double num)//识别出浮点数或指数
{
    if (type == Token::number)
    {
        if (numTalbe.find(num) == numTalbe.end())
            numTalbe[num] = numTalbe.size();
        out << "< num, " << numTalbe[num] << " >" << endl;
    }
    wordCount[int(type)]++;
    if (mode)
    {
        out << "Position:  line: " << line << "  col: " << col << endl;
        printToken(type);
        out << "Value: " << num << endl
            << endl;
    }
}
string Parser::parserFile()
{
    Token type;
    while (!file.eof() && !file.fail())
    {
        switch (state)
        {
        case State::begin:
        {
            token.erase();
            get_char();
            get_nbc();
            line = lineCount;
            col = colCount;
            switch (C)
            {
            case '_':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
                state = State::word;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                state = State::digit_i; //注：C语言中，数字不可作为变量开头
                break;
            case '+':
                state = State::oper_1;
                break;
            case '-':
                state = State::oper_2;
                break;
            case '/':
                state = State::oper_3;
                break;
            case '*':
                state = State::oper_4;
                break;
            case '%':
                state = State::oper_5;
                break;
            case '&':
                state = State::oper_6;
                break;
            case '|':
                state = State::oper_7;
                break;
            case '!':
                state = State::oper_8;
                break;
            case '>':
                state = State::oper_9;
                break;
            case '<':
                state = State::oper_11;
                break;
            case '^':
                state = State::oper_13;
                break;
            case '=':
                state = State::oper_14;
                break;
            case '\'': //字符
            {
                state = State::char_start;
                //....此处需要输出字符信息
                returnWord(Token::delimiter, "'");
                C = 0;
                col++;
                // token[0] = 0;
            }
            break;
            case '"': //字符串
            {
                state = State::str_start;
                //...此处需要输出字符串信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
                C = 0;
                col++;
                token[0] = 0;
            }
            break;
            case '(':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case ')':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case '{':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case '}':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case '[':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case ']':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case ':':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case ';':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case ',':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case '.':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case '#':
            {
                state = State::begin;
                //...此处输出分割符信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            case '?':
            {
                state = State::begin;
                //...此处输出?信息
                type = Token::delimiter;
                cat();
                returnWord(type, token);
            }
            break;
            default:
                state = State::error;
                break;
            }
        }
        break;
        case State::word:
        {
            cat();
            get_char();
            if (letter() || digit())
                state = State::word;
            else //当前字符不是字母/数字，单词读到头
            {
                retract();
                state = State::begin;
                key = reserve();
                if (key == KeyWord::not_key)
                {
                    //...返回单词信息
                    type = Token::id;
                    returnWord(type, token);
                }
                else
                {
                    //...返回关键字信息
                    type = Token::keyword;
                    returnWord(type, token);
                }
            }
        }
        break;
        case State::digit_i:
        {
            cat();
            get_char();
            if (digit())
                state = State::digit_i;
            else if (C == '.')
                state = State::digit_f1;
            else if (C == 'e' || C == 'E')
                state == State::digit_f2;
            else
            {
                retract();
                state = State::begin;
                //...输出数字信息
                type = Token::digit;
                returnDigit(type, SToI());
            }
        }
        break;
        case State::digit_f1:
        {
            cat();
            get_char();
            if (digit())
                state = State::digit_f2;
            else
            {
                retract();
                state = State::error;
            }
        }
        break;
        case State::digit_f2:
        {
            cat();
            get_char();
            if (digit())
                state = State::digit_f2;
            else if (C == 'e' || C == 'E')
                state = State::digit_e1;
            else
            {
                retract();
                state = State::begin;
                //...此处输出浮点数
                type = Token::number;
                returnNumber(type, SToF());
            }
        }
        break;
        case State::digit_e1:
        {
            cat();
            get_char();
            if (C == '+' || C == '-') //带符号的指数
                state = State::digit_e2;
            else if (digit()) //不带符合的指数
                state = State::digit_e3;
            else
            {
                retract();
                state = State::error;
            }
        }
        break;
        case State::digit_e2:
        {
            cat();
            get_char();
            if (digit())
                state = State::digit_e3;
            else
            {
                retract();
                state = State::error;
            }
        }
        break;
        case State::digit_e3:
        {
            cat();
            get_char();
            if (digit())
                state = State::digit_e3;
            else
            {
                retract();
                state = State::begin;
                //...输出指数
                type = Token::number;
                returnNumber(type, SToF());
            }
        }
        break;
        case State::oper_1:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号+=
                type = Token::oper;
                returnWord(type, token);
            }
            else if (C == '+')
            {
                //...输出符号++
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出符号+
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_2:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号-=
                type = Token::oper;
                returnWord(type, token);
            }
            else if (C == '-')
            {
                //...输出符号--
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出符号-
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_3:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号/=
                type = Token::oper;
                returnWord(type, token);
            }
            else if (C == '*')
                state = State::note_start;
            else if (C == '/')
            {
                state = State::begin;
                string temp;
                getline(file, temp);
                lineCount++;
                colCount = 0;
            }
            else
            {
                retract();
                //...输出符号/
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_4:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号*=
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出符号*
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_5:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号%=
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出符号%
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_6:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号&=
                type = Token::oper;
                returnWord(type, token);
            }
            else if (C == '&')
            {
                //...输出符号&&
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出符号&
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_7:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号|=
                type = Token::oper;
                returnWord(type, token);
            }
            else if (C == '|')
            {
                //...输出符号||
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出符号|
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_8:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号！=
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出符号！
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_9:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号>=
                type = Token::oper;
                returnWord(type, token);
            }
            else if (C == '>')
                state = State::oper_10;
            else
            {
                retract();
                //...输出符号>
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_10:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出>>=
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出>>
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_11:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出符号<=
                type = Token::oper;
                returnWord(type, token);
            }
            else if (C == '<')
                state = State::oper_12;
            else
            {
                retract();
                //...输出符号<
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_12:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出<<=
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出<<
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_13:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出^=
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出^
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::oper_14:
        {
            cat();
            get_char();
            state = State::begin;
            if (C == '=')
            {
                //...输出==
                type = Token::oper;
                returnWord(type, token);
            }
            else
            {
                retract();
                //...输出=
                type = Token::oper;
                returnWord(type, token);
            }
        }
        break;
        case State::char_start:
        {
            cat();
            get_char();
            if (C == '\'')
            {
                state = State::begin;
                //...输出 单字符信息
                type = Token::character;
                returnWord(type, token);
                col = colCount;
                line = lineCount;
                returnWord(Token::delimiter, "\'");
            }
            else if (C == '\\')
                state = State::char_end;
            else
                state = State::char_start;
        }
        break;
        case State::char_end:
        {
            cat();
            get_char();
            state = State::char_start;
        }
        break;
        case State::str_start:
        {
            cat();
            get_char();
            if (C == '"')
            {
                state = State::begin;
                //...输出字符串信息
                type = Token::str;
                returnWord(type, token);
                col = colCount;
                line = lineCount;
                returnWord(Token::delimiter, "\"");
            }
            else if (C == '\\')
                state = State::str_end;
            else
                state = State::str_start;
        }
        break;
        case State::str_end:
        {
            cat();
            get_char();
            state = State::str_start;
        }
        break;
        case State::note_start:
        {
            get_char();
            if (C == '*')
                state = State::note_end;
        }
        break;
        case State::note_end:
        {
            get_char();
            if (C == '/')
                state = State::begin;
            else if (C == '*')
                state = State::note_end;
            else
                state = State::str_start;
        }
        break;
        case State::error:
        {
            //...输出错误信息
            state = State::begin;
            type = Token::unknown;
            returnWord(type, token);
        }
        break;
        }
    }
    out << "Word count:" << endl;
    out << "ID:        " << wordCount[0] << endl;
    out << "Digit:     " << wordCount[1] << endl;
    out << "Number:    " << wordCount[2] << endl;
    out << "Char:      " << wordCount[3] << endl;
    out << "String:    " << wordCount[4] << endl;
    out << "Operater:  " << wordCount[5] << endl;
    out << "Delimiter: " << wordCount[6] << endl;
    out << "Keyword:   " << wordCount[7] << endl;
    out << "Total:     " << wordCount[0] + wordCount[1] + wordCount[2] + wordCount[3] + wordCount[4] + wordCount[5] + wordCount[6] + wordCount[7] << endl;
    out << "Error:     " << wordCount[8] << endl;
    out << "LineCount: " << lineCount << endl;
    out << "CharacterCount:   " << charCount << endl;
    return this->out.str();
}
void Parser::printToken(Token type)//输出记号信息
{
    out << "The type of token :";
    switch (type)
    {
    case Token::id:
        out << "id " << endl;
        break;
    case Token::digit:
        out << "digit " << endl;
        break;
    case Token::number:
        out << "number " << endl;
        break;
    case Token::character:
        out << "character " << endl;
        break;
    case Token::str:
        out << "str " << endl;
        break;
    case Token::oper:
        out << "oper " << endl;
        break;
    case Token::delimiter:
        out << "delimiter " << endl;
        break;
    case Token::keyword:
        out << "keyword " << endl;
        break;
    default:
        out << "error " << endl;
        break;
    }
}
