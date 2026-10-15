#include "csvReader.hpp"
#include <fstream>
#include <assert.h>

CsvReader::CsvReader(std::string filename)
{
    all.clear();
    std::ifstream ifs(filename);
    if (!ifs) return;
    // BOM Skipする
    unsigned char BOMS[] = {0xEF, 0xBB, 0xBF};
    bool found = true;
    for (int i = 0; i < 3; i++)
    {
        if (ifs.get() != BOMS[i])
        {
            found = false;
            break;
        }
    }
    if (!found)
        ifs.seekg(std::ios_base::beg);

    // データを読む
    std::string lineStr;
    while (getline(ifs, lineStr))
    {
        while (true)
        {
            int dq = 0;
            for (int i = 0; i < lineStr.size(); i++)
            {
                if (lineStr[i] == '"')
                    dq++;
            }
            if (dq % 2 == 0)
                break;
            std::string s;
            getline(ifs, s);
            lineStr += "\n" + s;
        }
        for (auto it = lineStr.begin(); it != lineStr.end();)
        {
            if (*it == '"')
                it = lineStr.erase(it);
            if (it != lineStr.end())
                it++;
        }

        // 行内を,で切り分ける
        LINEREC lineRecord;
        int top = 0;
        bool indq = false;
        for (int n = 0; n < lineStr.size(); n++)
        {
            if (lineStr[n] == ',')
            {
                if (!indq)
                {
                    lineRecord.record.emplace_back(lineStr.substr(top, (size_t)(n - top)));
                    top = n + 1;
                }
            }
            else if (lineStr[n] == '"')
                indq = !indq;
        }
        lineRecord.record.emplace_back(lineStr.substr(top, lineStr.size() - top));
        all.emplace_back(lineRecord);
    }
    ifs.close();
}

CsvReader::~CsvReader()
{
    for (auto rec : all)
        rec.record.clear();
    all.clear();
}

int CsvReader::GetLines()
{
    return (int)all.size();
}

int CsvReader::GetColumns(int line)
{
    assert(line < GetLines());
    return (int)all[line].record.size();
}

std::string CsvReader::GetString(int line, int column)
{
    assert(line < GetLines());
    if (column >= GetColumns(line))
        return "";
    return all[line].record[column];
}

int CsvReader::GetInt(int line, int column)
{
    std::string str = GetString(line, column);
    if (str == "")
    {
        return 0;
    }
    return std::stoi(str);
}

float CsvReader::GetFloat(int line, int column)
{
    std::string str = GetString(line, column);
    if (str == "")
    {
        return 0.0f;
    }
    return std::stof(str);
}
