#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <unordered_map>
using namespace std;
/**
 * @brief 状态
 *
 */
enum class State
{
    begin,    // 初始状态
    word,     // 单词
    digit_i,  // 整数
    digit_f1, // 浮点数第一阶段
    digit_f2, // 浮点数第二阶段 可能转为指数
    digit_e1, // 指数第一阶段 后续可能分正负
    digit_e2,
    digit_e3, // 指数部带上符号
    oper_1,   // +,++,+=
    oper_2,   // -,--,-=
    oper_3,   // /,/=
    oper_4,   // *,*=
    oper_5,   // %,%=
    oper_6,   //& && &=
    oper_7,   //| || |=
    oper_8,   //! !=
    oper_9,   // >,>=,>>
    oper_10,  //>>=
    oper_11,  //<,<=,<<
    oper_12,  //<<=
    oper_13,  //^,^=
    oper_14,  //=,==
    // oper_14,      //? :   这个存疑 事实上难以实现
    char_start, //字符结束
    char_end,   //字符开始
    str_start,  //字符串开始
    str_end,    //字符串结束
    note_start, // 多行注释开始
    note_end,   // 多行注释结束
    error       //错误

};
/**
 * @brief 记号
 *
 */
enum class Token
{
    id,        //单词
    digit,     //常数
    number,    //浮点数/指数
    character, //单一字符
    str,       //字符串
    oper,      //操作符
    delimiter, //分隔符
    keyword,   //关键字
    unknown    //未知符
};
/**
 * @brief 关键字
 *
 */
enum class KeyWord
{
    not_key = -1,
    key_auto,
    key_short,
    key_int,
    key_long,
    key_float,
    key_double,
    key_char,
    key_struct,
    key_union,
    key_enum,
    key_typedef,
    key_const,
    key_unsigned,
    key_signed,
    key_extern,
    key_register,
    key_static,
    key_volatile,
    key_void,
    key_if,
    key_else,
    key_switch,
    key_case,
    key_default,
    key_for,
    key_do,
    key_while,
    key_continue,
    key_break,
    key_goto,
    key_sizeof,
    key_return
};
/**
 * @brief 单词属性
 *
 */
class Word
{
public:
    Token token = Token::unknown; //类型
    long long num = 0;            //整数
    double dig = 0;               //浮点数
    string str = "";              //字符串
};
/**
 * @brief 词法分析器
 *
 */
class Parser
{
    int lineCount;                                   //行数
    int colCount;                                   //字符数量
    int charCount;
    int wordCount[9];                                //八种类型记号数量
    int line;                                        //记录符号行位置
    int col;                                         //记录符号列位置
    char C;                                          //当前字符
    int mode;                                        //信息输出模式
    string token;                                    //正在识别的字符串
    string inputName;                                //待读取文件名
    string outputName;                               //数据输出文件命名
    string buffer;                                   //输入缓冲区
    State state = State::begin;                      //当前状态
    KeyWord key;                                     //关键字类型
    stringstream out;                                //文件输出流
    int get_char();                                  //从文件读取一个字符
    int get_nbc();                                   //检查C中字符是否为空格，是则反复调用get_char直到不是空格
    void cat();                                      //将C中的字符连接到token后
    bool letter();                                   //判断字符是否是字符
    bool digit();                                    //判断字符是否是数字
    void retract();                                  //读取位置回退
    KeyWord reserve();                               //查询单词是否是关键字
    long long SToI();                                //将token中的字符串转换为整数
    double SToF();                                   //将token中的字符串转换为浮点数
    void returnWord(Token type, string str);         //识别出单词
    void returnDigit(Token type, long long int num); //识别出常数
    void returnNumber(Token type, double num);       //识别出浮点数、指数
    void printToken(Token type);                     //输出记号类型
    void table_insert(string id);

public:
    string parserFile();                    //分析文本
    ifstream file;                          //目的分析文件
    unordered_map<string, int> symbolTable; //符号表
    unordered_map<int, int> digitTable;     //整数表
    unordered_map<double, int> numTalbe;    //浮点数及整数表
    Parser(string filename, int mode)
    {
        this->inputName = filename;
        file.open(inputName, ios::in);
        memset(wordCount, 0, sizeof(int) * 8);
        lineCount = 1;
        colCount = 0;
        this->mode = (mode == 1) ? 1 : 0;
    }
    ~Parser()
    {
        file.close();
    }
};
