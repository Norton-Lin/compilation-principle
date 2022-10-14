#include "parser.h"
int main()
{
    string parserFileName; //待分析文件名
    string outputFileName; //分析结果输出文件名
    stringstream stream;   //输出流
    int mode;
    cout << "Please enter the file name to be analyzed: " << endl;
    cin >> parserFileName;
    cout << "Please enter the information input mode" << endl
         << "1.detailed message" << endl
         << "2.simple message"<< endl
         << endl;
    cin >> mode;
    Parser *parser = new Parser(parserFileName,mode);
    cout << "Please enter the saving method of analysis results: " << endl
         << "1.Display on the screen " << endl
         << "2.Save in file" << endl;
    cin >> mode;
    if (parser->file.fail())
    {
        cout << endl
             << "Error: File to parse open failed." << endl;
        return -1;
    }
    // string ans = parser->parserFile();
    stream << parser->parserFile();
    if (mode == 2)
    {
        ofstream output;
        output.open("Analysis_Results.txt", ios::out);
        output << stream.str() << endl;
        output.close();
        output.open("ID_Record.txt", ios::out);
        for (auto it = parser->symbolTable.begin(); it != parser->symbolTable.end(); it++)
            output << "<" << it->first << "," << it->second << ">" << endl;
        output.close();
        output.open("Digit_Record.txt", ios::out);
        for (auto it = parser->digitTable.begin(); it != parser->digitTable.end(); it++)
            output << "<" << it->first << "," << it->second << ">" << endl;
        output.close();
        output.open("Num_Record.txt", ios::out);
        for (auto it = parser->numTalbe.begin(); it != parser->numTalbe.end(); it++)
            output << "<" << it->first << "," << it->second << ">" << endl;
        output.close();
    }
    else
    {
        cout << stream.str();
    }
    cin.get();
    cin.get();
    return 0;
}