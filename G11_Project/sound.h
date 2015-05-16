//=============================================================================
//
// サウンド処理 [sound.h]
// Author : TOMOHIRO KOUNO
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// パラメータ構造体定義
//=============================================================================
typedef struct
{
	char *FileName;	// ファイル名
	bool Loop;			// ループするかどうか
} PARAM;

// サウンドファイル
typedef enum {
	SOUND_NONE = 0,
	BGM_TITLE,
	MAX_SOUND

}SOUND_TYPE; //名前を入力

class Sound
{
public:
	static void Initialize(void);
	static void Finalize(void);
	void Update(void);

	static Sound* Instance(){ return Self; }

	HRESULT Play(SOUND_TYPE label);
	HRESULT Pause(SOUND_TYPE label);
	void Stop(SOUND_TYPE label);
	void Stop(void);
	void SetVolume(SOUND_TYPE label,float volume);
	void SetFade(SOUND_TYPE label,float volume,int frame);
	void Fade(int frame);
	void Fade(SOUND_TYPE label,int frame);
private:
	Sound();
	Sound(const Sound& other){};
	~Sound(){};
	Sound &operator= (const Sound& other){};

	HRESULT Init(void);
	void Uninit(void);

	IXAudio2* XAudio2Device;
	IXAudio2MasteringVoice* MasteringVoice;
	IXAudio2SourceVoice* SourceVoice[MAX_SOUND];
	BYTE* AudioData[MAX_SOUND];
	DWORD AudioSize[MAX_SOUND];
	float Volume[MAX_SOUND];
	float ChangeVolume[MAX_SOUND];
	bool FadeFlag[MAX_SOUND];
	const static PARAM Param[MAX_SOUND];

	HRESULT CheckChunk(HANDLE file,DWORD format,DWORD *pChunkSize,DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE file,void *pBuffer,DWORD dwBuffersize,DWORD dwBufferoffset);

	static Sound* Self;

};

#endif
