//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : TOMOHIRO KOUNO
//
//=============================================================================
#include "sound.h"

Sound* Sound::Self = nullptr;

// 各音素材のパラメータ
const PARAM Sound::Param[MAX_SOUND] =
{
	{ NULL,false }, //空データとして用意
	{ "data/sound/BGM/星の破線.wav",true },

};

Sound::Sound()
{
	XAudio2Device = nullptr;
	MasteringVoice = nullptr;
	for (int cnt = 0;cnt < MAX_SOUND;cnt++)
	{
		SourceVoice[cnt] = nullptr;
		AudioData[cnt] = nullptr;
		AudioSize[cnt] = 0;
	}
}

void Sound::Initialize(void)
{
	Self = new Sound;
	if (Self)
	{
		Self->Init();
	}
}

void Sound::Finalize(void)
{
	if (Self != nullptr)
	{
		Self->Uninit();
		delete Self;
		Self = nullptr;
	}
}
//=============================================================================
// 初期化
//=============================================================================
HRESULT Sound::Init(void)
{
	HWND hWnd = Window::Instance()->hWnd();
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL,COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&XAudio2Device,0);
	if (FAILED(hr))
	{
		MessageBox(hWnd,"XAudio2オブジェクトの作成に失敗！","警告！",MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = XAudio2Device->CreateMasteringVoice(&MasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd,"マスターボイスの生成に失敗！","警告！",MB_ICONWARNING);

		if (XAudio2Device)
		{
			// XAudio2オブジェクトの開放
			XAudio2Device->Release();
			XAudio2Device = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int cnt = 1; cnt < MAX_SOUND; cnt++)
	{

		HANDLE file;
		DWORD chunkSize = 0;
		DWORD chunkPosition = 0;
		DWORD fileType;
		WAVEFORMATEXTENSIBLE format;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&format,0,sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer,0,sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		file = CreateFile(Param[cnt].FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			SourceVoice[cnt] = nullptr;
			continue;
			//MessageBox(hWnd,"サウンドデータファイルの生成に失敗！(1)","警告！",MB_ICONWARNING);
			//return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(file,0,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd,"サウンドデータファイルの生成に失敗！(2)","警告！",MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(file,'FFIR',&chunkSize,&chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"WAVEファイルのチェックに失敗！(1)","警告！",MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(file,&fileType,sizeof(DWORD),chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"WAVEファイルのチェックに失敗！(2)","警告！",MB_ICONWARNING);
			return S_FALSE;
		}
		if (fileType != 'EVAW')
		{
			MessageBox(hWnd,"WAVEファイルのチェックに失敗！(3)","警告！",MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(file,' tmf',&chunkSize,&chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"フォーマットチェックに失敗！(1)","警告！",MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(file,&format,chunkSize,chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"フォーマットチェックに失敗！(2)","警告！",MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(file,'atad',&AudioSize[cnt],&chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"オーディオデータ読み込みに失敗！(1)","警告！",MB_ICONWARNING);
			return S_FALSE;
		}
		AudioData[cnt] = (BYTE*)malloc(AudioSize[cnt]);
		hr = ReadChunkData(file,AudioData[cnt],AudioSize[cnt],chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"オーディオデータ読み込みに失敗！(2)","警告！",MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = XAudio2Device->CreateSourceVoice(&SourceVoice[cnt],&(format.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd,"ソースボイスの生成に失敗！","警告！",MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = AudioSize[cnt];
		buffer.pAudioData = AudioData[cnt];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = 0;

		// オーディオバッファの登録
		SourceVoice[cnt]->SubmitSourceBuffer(&buffer);

		//ステータスの初期化
		Volume[cnt] = 0;
		ChangeVolume[cnt] = 0;
		FadeFlag[cnt] = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Sound::Uninit(void)
{
	// 一時停止
	for (int cnt = 0; cnt < MAX_SOUND; cnt++)
	{
		if (SourceVoice[cnt])
		{
			// 一時停止
			SourceVoice[cnt]->Stop(0);

			// ソースボイスの破棄
			SourceVoice[cnt]->DestroyVoice();
			SourceVoice[cnt] = NULL;

			// オーディオデータの開放
			free(AudioData[cnt]);
			AudioData[cnt] = NULL;
			SourceVoice[cnt] = nullptr;
		}
	}

	// マスターボイスの破棄
	MasteringVoice->DestroyVoice();
	MasteringVoice = NULL;

	if (XAudio2Device)
	{
		// XAudio2オブジェクトの開放
		XAudio2Device->Release();
		XAudio2Device = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(停止)
//=============================================================================
HRESULT Sound::Play(SOUND_TYPE label)
{
	if (SourceVoice[label] == nullptr)
	{
		return E_FAIL;
	}
	XAUDIO2_VOICE_STATE state;

	XAUDIO2_BUFFER buffer;

	memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = AudioSize[label];
	buffer.pAudioData = AudioData[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	//ラベルのパラメータを見て無限ループか単一SEかを設定
	buffer.LoopCount = (Param[label].Loop) ? XAUDIO2_LOOP_INFINITE : 0;

	// 状態取得
	SourceVoice[label]->GetState(&state);
	if (state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		SourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		SourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	SourceVoice[label]->SubmitSourceBuffer(&buffer);
	//Volume[label] = ChangeVolume[label] = 1.0f;
	// 再生
	SourceVoice[label]->Start(0);

	return S_OK;
}

//=============================================================================
// 一時停止からのセグメント再生
//=============================================================================
HRESULT Sound::Pause(SOUND_TYPE label)
{
	if (SourceVoice[label] == nullptr)
	{
		return E_FAIL;
	}
	//再生が可能なデータなら再生を要求

	XAUDIO2_VOICE_STATE state;

	XAUDIO2_BUFFER buffer;

	memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = AudioSize[label];
	buffer.pAudioData = AudioData[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	//ラベルのパラメータを見て無限ループか単一SEかを設定
	buffer.LoopCount = (Param[label].Loop) ? XAUDIO2_LOOP_INFINITE : 0;

	// 状態取得
	SourceVoice[label]->GetState(&state);
	// 再生終了していた場合
	if (state.BuffersQueued == 0)
	{
		//ループ設定なしだった場合
		if (buffer.LoopCount == 0)
		{
			// 停止
			SourceVoice[label]->Stop(0);
			SourceVoice[label]->FlushSourceBuffers();

			// オーディオバッファの登録
			SourceVoice[label]->SubmitSourceBuffer(&buffer);
		}
		// 再生
		SourceVoice[label]->Start(0);
	}
	else
	{
		//再生中なら一時停止
		SourceVoice[label]->Stop(0);
	}


	return S_OK;
}

//=============================================================================
// セグメント停止
//=============================================================================
void Sound::Stop(SOUND_TYPE label)
{
	if (SourceVoice[label] == nullptr)
	{
		return ;
	}
	XAUDIO2_VOICE_STATE state;

	// 状態取得
	SourceVoice[label]->GetState(&state);
	if (state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		SourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		SourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止
//=============================================================================
void Sound::Stop(void)
{
	// 一時停止
	for (int cnt = 0; cnt < MAX_SOUND; cnt++)
	{
		Stop((SOUND_TYPE)cnt);
		//if (SourceVoice[cnt])
		//{
		//	// 一時停止
		//	SourceVoice[cnt]->Stop(0);
		//}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT Sound::CheckChunk(HANDLE file,DWORD format,DWORD *pChunkSize,DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD read;
	DWORD chunkType;
	DWORD chunkDataSize;
	DWORD RIFFDataSize = 0;
	DWORD fileType;
	DWORD bytesRead = 0;
	DWORD offset = 0;

	if (SetFilePointer(file,0,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(file,&chunkType,sizeof(DWORD),&read,NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(file,&chunkDataSize,sizeof(DWORD),&read,NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (chunkType)
		{
		case 'FFIR':
			RIFFDataSize = chunkDataSize;
			chunkDataSize = 4;
			if (ReadFile(file,&fileType,sizeof(DWORD),&read,NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(file,chunkDataSize,NULL,FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		offset += sizeof(DWORD) * 2;
		if (chunkType == format)
		{
			*pChunkSize = chunkDataSize;
			*pChunkDataPosition = offset;

			return S_OK;
		}

		offset += chunkDataSize;
		if (bytesRead >= RIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT Sound::ReadChunkData(HANDLE file,void *pBuffer,DWORD dwBuffersize,DWORD dwBufferoffset)
{
	DWORD read;

	if (SetFilePointer(file,dwBufferoffset,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(file,pBuffer,dwBuffersize,&read,NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

//=============================================================================
//サウンドの更新
//=============================================================================
void Sound::Update(void)
{

	for (int cnt = 1;cnt<MAX_SOUND;cnt++)
	{

		if (SourceVoice[cnt])
		{
			float CVolume = 0;//現在の音量の保存用
			SourceVoice[cnt]->GetVolume(&CVolume);//現在の音量を保存

			SourceVoice[cnt]->SetVolume(CVolume + ChangeVolume[cnt]);//音量を変更

			if (CVolume + ChangeVolume[cnt] - Volume[cnt] <= 0.01f&&CVolume + ChangeVolume[cnt] - Volume[cnt] >= -0.01f)
			{
				ChangeVolume[cnt] = 0;
			}

			SourceVoice[cnt]->GetVolume(&CVolume);//現在の音量を保存


			if (CVolume <= 0)
			{//無音いかになったとき
				SourceVoice[cnt]->SetVolume(0);//0に調整
				ChangeVolume[cnt] = 1;
			}
			else if (CVolume + ChangeVolume[cnt] > 1)
			{//100%以上になってしまったとき
				ChangeVolume[cnt] = 0;
				SourceVoice[cnt]->SetVolume(1.0f);
			}//else if

			if (CVolume <= 0.01f)
			{
				Stop((SOUND_TYPE)cnt);//音を停止
				ChangeVolume[cnt] = 1;
				FadeFlag[cnt] = false;//フェードのフラグをOFF
			}
		}
	}//for
}
//=============================================================================
//音量の調整
//引数:SOUND_TYPE…		変更したい音のラベル
//		float volume…	変更後の音量(1:100%、0:無音)
//=============================================================================
void Sound::SetVolume(SOUND_TYPE label,float volume)
{
	XAUDIO2_VOICE_STATE state;

	if (volume<0)//負にならないようにする
	{
		volume = 0;
	}

	SourceVoice[label]->GetState(&state);

	if (state.BuffersQueued != 0)
	{// 再生中
		SourceVoice[label]->SetVolume(volume);//音量を変更
	}
}
//=============================================================================
//音量の調整
//引数:SOUND_TYPE…		変更したい音のラベル
//		float volume…	変更後の音量(1:100% 0:無音)
//		float flame…	何フレームかけて変化するか
//=============================================================================
void Sound::SetFade(SOUND_TYPE label,float volume,int frame)
{
	if (volume<0)//設定値を正規化
	{
		Volume[label] = 0;
	}
	else
	{
		Volume[label] = volume;
	}

	SourceVoice[label]->GetVolume(&ChangeVolume[label]);//現在の音量を取得

	ChangeVolume[label] = (volume - ChangeVolume[label]) / frame;//1フレームで変化する音量を計算
}
//=============================================================================
//すべての音のフェード
//引数:float flame…	何フレームかけてフェードさせるか？
//=============================================================================
void Sound::Fade(int frame)
{
	for (int cnt = 1;cnt<MAX_SOUND;cnt++)
	{

		XAUDIO2_VOICE_STATE state;
		// 状態取得
		SourceVoice[cnt]->GetState(&state);
		if (state.BuffersQueued != 0 && FadeFlag[cnt] == false)
		{
			SourceVoice[cnt]->GetVolume(&ChangeVolume[cnt]);//現在の音量を取得
			//変化量を計算
			ChangeVolume[cnt] = -(ChangeVolume[cnt] / frame);
			Volume[cnt] = 0;
			FadeFlag[cnt] = true;//フェードフラグを立てる
		}

	}
}

//=============================================================================
//音のフェード
//引数:SOUND_TYPE…		フェードさせたい音のラベル
//		float flame…	何フレームかけてフェードさせるか？
//=============================================================================
void Sound::Fade(SOUND_TYPE label,int frame)
{
	SourceVoice[label]->GetVolume(&ChangeVolume[label]);//現在の音量を取得
	//変化量を計算
	ChangeVolume[label] = -(ChangeVolume[label] / frame);
	Volume[label] = 0;
	FadeFlag[label] = true;//フェードフラグを立てる
}