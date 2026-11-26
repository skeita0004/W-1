///<summary>
///Player01・Player02の戦績を管理する。
///
///使用方法:
///
///勝敗記録:
///WinCount::Win(WinCount::Player::PLAYER01);
///Player01の勝利、Player02の敗北を記録する。
///
///WinCount::Win(WinCount::Player::PLAYER02);
///Player02の勝利、Player01の敗北を記録する。
///
///引き分け記録:
///WinCount::Draw();
///双方の引き分けを記録する。
///
///戦績初期化:
///WinCount::Reset();
///双方の戦績を初期化する。
///
///戦績参照:
///WinCount::player01.win
///WinCount::player01.lose
///WinCount::player01.draw
///
///WinCount::player02.win
///WinCount::player02.lose
///WinCount::player02.draw
///
///各ミニゲームの勝敗判定から呼び出し、
///GeneralResultSceneで累計戦績を表示する。
/// byじぇみに
///</summary>
#pragma once

namespace WinCount
{
    //プレイヤー
    enum class Player
    {
        PLAYER01,
        PLAYER02
    };

    //戦績
    struct Record
    {
        int win = 0;
        int lose = 0;
        int draw = 0;
    };

    inline Record player01;
    inline Record player02;

    //勝敗記録
    inline void Win(Player winner)
    {
        if (winner == Player::PLAYER01)
        {
            player01.win++;
            player02.lose++;
        }
        else
        {
            player02.win++;
            player01.lose++;
        }
    }

    //引き分け
    inline void Draw()
    {
        player01.draw++;
        player02.draw++;
    }

    //初期化
    inline void Reset()
    {
        player01 = {};
        player02 = {};
    }
}
