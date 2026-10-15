#pragma once
#include <string>
#include <vector>

/// <summary>
/// CSVファイルを読み、セルごとにデータを返します
/// データは、string型、int型、float型で返します
/// 3つの横並びのセルで、VECTOR型を返すこともできます
/// 範囲外を指定した場合は、assertします
/// </summary>
class CsvReader
{
public:
    /// <summary>
    /// このクラスは、コンストラクタでファイルを読み、
    /// CSVファイルを読んで、値を保持します。
    /// コンストラクタ終了時には、ファイルを閉じています。
    /// </summary>
    /// <param name="filename">ファイル名</param>
    CsvReader(std::string filename);
    ~CsvReader();

    /// <summary>
    /// CSVの行数を取得します
    /// </summary>
    /// <returns></returns>
    int GetLines();

    /// <summary>
    /// 指定した行のカラム数を取得します
    /// </summary>
    /// <param name="line">行番号</param>
    /// <returns>カラム数</returns>
    int GetColumns(int line);

    /// <summary>
    /// 指定した行・列のデータを文字列で返します
    /// </summary>
    /// <param name="line">行番号</param>
    /// <param name="column">列番号</param>
    /// <returns>文字列</returns>
    std::string GetString(int line, int column);

    /// <summary>
    /// 指定した行・列のデータをint型の数値で返します
    /// データが入ってない場合は0を返します
    /// </summary>
    /// <param name="line">行番号</param>
    /// <param name="column">列番号</param>
    /// <returns>数値</returns>
    int GetInt(int line, int column);

    /// <summary>
    /// 指定した行・列のデータをint型の数値で返します
    /// データが入ってない場合は0を返します
    /// </summary>
    /// <param name="line"></param>
    /// <param name="column"></param>
    /// <param name="line">行番号</param>
    /// <param name="column">列番号</param>
    /// <returns>小数値</returns>
    float GetFloat(int line, int column);
private:
    struct LINEREC
    {
        std::vector<std::string> record;
    };
    std::vector<LINEREC> all;
};
