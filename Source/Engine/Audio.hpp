#pragma once
#include <string>


//-----------------------------------------------------------
//サウンドを管理する（XAudio使用）
//-----------------------------------------------------------
namespace Audio
{
	//初期化
	void Initialize();

	//サウンドファイル(.wav）をロード
	//すでに同じ名前のファイルをロード済みの場合は、既存のデータの番号を返す
	//引数：fileName	ファイル名
	//引数：isLoop		ループ再生させたいならtrue
	//引数：svNum　		同時に鳴らす最大数（省略可）
	//戻値：そのデータに割り当てられた番号
	int Load(std::string _fileName, bool isLoop = false, int svNum = 1);

	//再生
	//引数：handle	鳴らしたいサウンドの番号
	void Play(int ID);

	//停止
	//引数：handle	止めたいサウンドの番号
	void Stop(int ID);

	//シーンごとの解放（ロードしたサウンドを解放）
	void Release();

	//本体も解放
	void AllRelease();
};

