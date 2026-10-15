#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "Direct3D.hpp"
#include "SafeCleaning.hpp"
#include "RootObject.hpp"
#include "Model.hpp"
#include "Image.hpp"

#include "CameraSet.hpp"

#include "Input.hpp"
#include "Audio.hpp"
#include "VFX.hpp"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#pragma comment(lib,"Winmm.lib")

using namespace SafeCleaning;

//定数宣言
const wchar_t* WIN_CLASS_NAME = L"SampleGame";	//ウィンドウクラス名


//プロトタイプ宣言
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
    CameraSet* pCameraSet{nullptr};

    int centerX;
    int centerY;
}

// エントリーポイント
int WINAPI WinMain(_In_     HINSTANCE hInstance,
                   _In_opt_ HINSTANCE hPrevInstance,
                   _In_     LPSTR lpCmdLine,
                   _In_     int nCmdShow)
{
    ShowCursor(FALSE);

    srand((unsigned)time(NULL));
    SetCurrentDirectory(L"Assets");

    //初期化ファイル（setup.ini）から必要な情報を取得
    int screenWidth = GetPrivateProfileInt(L"SCREEN", L"Width", 800, L".\\setup.ini");		//スクリーンの幅
    int screenHeight = GetPrivateProfileInt(L"SCREEN", L"Height", 450, L".\\setup.ini");	//スクリーンの高さ
    int fpsLimit = GetPrivateProfileInt(L"GAME", L"Fps", 60, L".\\setup.ini");				//FPS（画面更新速度）
    int isDrawFps = GetPrivateProfileInt(L"DEBUG", L"ViewFps", 0, L".\\setup.ini");		//キャプションに現在のFPSを表示するかどうか

    centerX = screenWidth / 2;
    centerY = screenHeight / 2;
    //ウィンドウを作成
    HWND hWnd = InitApp(hInstance, screenWidth, screenHeight, nCmdShow);

    //Direct3D準備
    Direct3D::Initialize(hWnd, screenWidth, screenHeight);

    //カメラを準備
    pCameraSet = new CameraSet();
    pCameraSet->Init();

    //入力処理（キーボード、マウス、コントローラー）の準備
    Input::Initialize(hWnd);

    //オーディオ（効果音）の準備
    Audio::Initialize();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags or_eq ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags or_eq ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(Direct3D::pDevice_, Direct3D::pContext_);

    //ルートオブジェクト準備
    //すべてのゲームオブジェクトの親となるオブジェクト
    RootObject* pRootObject = new RootObject;
    pRootObject->Init();


    //メッセージループ（何か起きるのを待つ）
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        //メッセージあり（こっちが優先）
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //メッセージなし（ここでゲームの処理）
        else
        {
            //時間計測
            timeBeginPeriod(1);	//時間計測の制度を上げる
            static int FPS = 0;								//画面更新回数のカウンタ
            static DWORD lastFpsResetTime = timeGetTime();	//最後にキャプションにFPSを表示した
            static DWORD lastUpdateTime = timeGetTime();	//最後に画面を更新した時間
            DWORD nowTime = timeGetTime();					//現在の時間

            //キャプションに現在のFPSを表示する
            if (isDrawFps)
            {
                //1秒（1000ミリ秒）経ったら
                if (nowTime - lastFpsResetTime > 1000)
                {
                    //FPSの値を表示
                    wchar_t string[16];
                    wsprintf(string, L"FPS:%d", FPS);
                    SetWindowText(GetActiveWindow(), string);
                    FPS = 0;
                    lastFpsResetTime = nowTime;
                }
            }


            //指定した時間（FPSを60に設定した場合は60分の1秒）経過していたら更新処理
            if ((nowTime - lastUpdateTime) * fpsLimit > 1000.0f)
            {
                ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();
                //時間計測関連
                lastUpdateTime = nowTime;	//現在の時間（最後に画面を更新した時間）を覚えておく
                FPS++;						//画面更新回数をカウントする

                //入力（キーボード、マウス、コントローラー）情報を更新
                Input::Update();

                //全オブジェクトの更新処理
                //ルートオブジェクトのUpdateを呼んだあと、自動的に子、孫のUpdateが呼ばれる
                pRootObject->UpdateSub();

                pCameraSet->GetCurrent()->Update();

                //エフェクトの更新
                VFX::Update();

                //このフレームの描画開始
                Direct3D::BeginDraw();

                //全オブジェクトを描画
                //ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
                pRootObject->DrawSub();
                //エフェクトの描画
                VFX::Draw();

                ImGui::Render();
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

                //描画終了
                Direct3D::EndDraw();
                
                //ちょっと休ませる
                Sleep(1);
            }
            timeEndPeriod(1);	//時間計測の制度を戻す
        }
    }

    

    //いろいろ解放
    VFX::Release();
    Audio::AllRelease();
    Model::AllRelease();
    Image::AllRelease();
    pRootObject->ReleaseSub();
    SafeDelete(pRootObject);
    Direct3D::Release();

    pCameraSet->Release();
    SafeDelete(pCameraSet);

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    ShowCursor(TRUE);

    return 0;
}


//ウィンドウの作成
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow)
{
    //ウィンドウクラス（設計図）を作成
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);					//この構造体のサイズ
    wc.hInstance = hInstance;						//インスタンスハンドル
    wc.lpszClassName = WIN_CLASS_NAME;				//ウィンドウクラス名
    wc.lpfnWndProc = WndProc;						//ウィンドウプロシージャ
    wc.style = CS_VREDRAW | CS_HREDRAW;				//スタイル（デフォルト）
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//アイコン
    wc.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);	//小さいアイコン
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);	//マウスカーソル
    wc.lpszMenuName = nullptr;						//メニュー（なし）
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//背景（白）
    RegisterClassEx(&wc);

    //ウィンドウサイズの計算
    RECT winRect = { 0, 0, screenWidth, screenHeight };
    AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);

    //タイトルバーに表示する内容
    wchar_t caption[64];
    GetPrivateProfileString(L"SCREEN", L"Caption", L"***", caption, 64, L".\\setup.ini");

    //ウィンドウを作成
    HWND hWnd = CreateWindow(
        WIN_CLASS_NAME,					//ウィンドウクラス名
        caption,						//タイトルバーに表示する内容
        WS_OVERLAPPEDWINDOW,			//スタイル（普通のウィンドウ）
        CW_USEDEFAULT,					//表示位置左（おまかせ）
        CW_USEDEFAULT,					//表示位置上（おまかせ）
        winRect.right - winRect.left,	//ウィンドウ幅
        winRect.bottom - winRect.top,	//ウィンドウ高さ
        nullptr,						//親ウインドウ（なし）
        nullptr,						//メニュー（なし）
        hInstance,						//インスタンス
        nullptr							//パラメータ（なし）
    );

    //ウィンドウを表示
    ShowWindow(hWnd, nCmdShow);

    return hWnd;
}


//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                                 UINT msg,
                                                                 WPARAM wParam,
                                                                 LPARAM lParam);
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
    //ウィンドウを閉じた
    case WM_DESTROY:
        PostQuitMessage(0);	//プログラム終了
        return 0;

    //マウスが動いた
    case WM_MOUSEMOVE:
        Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));

        SetCursorPos(centerX, centerY);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}