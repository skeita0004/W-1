#pragma once
#include <vector>
#include <string>

//-----------------------------------------------------------
//CSVファイルを扱うクラス
//-----------------------------------------------------------
class CsvReader
{
	//読み込んだデータを入れておく2次元配列
	std::vector<std::vector<std::string>> data_;

	//「,」か「改行」までの文字列を取得
	//引数：reuult	結果を入れるアドレス
	//引数：data	もとの文字列データ
	//引数：index	何文字目から調べるか
	//戻値：なし
	void GetToComma(std::string *result, std::string data, DWORD* index);

public:
	//コンストラクタ
	CsvReader();

	//デストラクタ
	~CsvReader();

	//CSVファイルのロード
	//引数：fileName　ファイル名
	//戻値：成功→true　失敗→false
	bool Load(std::string fileName);


	//指定した位置のデータを文字列で取得
	//引数：x,y　取得したい位置
	//戻値：取得した文字列
	std::string GetString(DWORD x, DWORD y);

	//指定した位置のデータを整数で取得
	//引数：x,y　取得したい位置
	//戻値：取得した値
	int GetValue(DWORD x, DWORD y);

	//ファイルの列数を取得
	//引数：なし
	//戻値：列数
	size_t GetWidth();

	//ファイルの行数を取得
	//引数：なし
	//戻値：行数
	size_t GetHeight();
};

