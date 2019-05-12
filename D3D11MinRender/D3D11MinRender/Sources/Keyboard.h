/*
	@file	Keyboard.h
	@brief	Windowsキーボード入力
	@date	2019/03/29
	@detail	キーコード確認用リンク:http://www.azaelia.net/factory/vk.html
	@author	番場 宥輝
*/
#pragma once
#include<Windows.h>


/*!
	@brief	キーボード入力クラス
*/
class Keyboard
{
public:
	/*!
		@brief	デストラクタ
	*/
	~Keyboard();

	/*! 方向キー */

	/*!
		@var	c_Up
		@brief	↑ 上矢印(PgUp)キー
	*/
	static constexpr unsigned char c_Up		= 0x26;

	/*!
		@var	c_Down
		@brief	↓ 下矢印(PgDn)キー
	*/
	static constexpr unsigned char c_Down	= 0x28;
	/*!
		@var	c_Left
		@brief	← 左矢印(Home)キー
	*/
	static constexpr unsigned char c_Left	= 0x25;
	/*!
		@var	c_Right
		@brief	→ 右矢印(End)キー
	*/
	static constexpr unsigned char c_Right	= 0x27;

	/*! 特殊キー */

	/*!
		@var	c_Space
		@brief	スペースキー
	*/
	static constexpr unsigned char c_Space	= 0x20;
	
	/*!
		@var	c_Enter
		@brief	エンターキー
	*/
	static constexpr unsigned char c_Enter	= 0x0D;

	/*!
		@var	c_Shift
		@brief	シフトキー
	*/
	static constexpr unsigned char c_Shift	= 0x10;
	/*!
		@var	c_Back
		@brief	バックスペースキー
	*/
	static constexpr unsigned char c_Back	= 0x08;
	/*!
		@var	c_Tab
		@brief	タブキー
	*/
	static constexpr unsigned char c_Tab	= 0x09;
	/*!
		@var	c_Delete
		@brief	デリートキー
	*/
	static constexpr unsigned char c_Delete	= 0x2E;

	/*!
		@fn		UpdateBuffer
		@brief	入力バッファの更新
		@detail	※入力判定を取得する下記関数の呼び出し前に行ってください
	*/
	static void UpdateBuffer();

	/*!
		@fn			GetButton
		@brief		キーの押下判定
		@param[in]	判定するキー
		@return		true:押されている  false:押されていない
	*/
	static bool GetButton(unsigned char key);

	/*!
		@fn			GetButtonDown
		@brief		キーの押した瞬間の押下判定
		@param[in]	判定するキー
		@return		true:押された  false:押された瞬間ではない
	*/
	static bool GetButtonDown(unsigned char key);

	/*!
		@fn			GetButtonUp
		@brief		キーの離した瞬間の判定
		@param[in]	判定するキー
		@return		true:離された  false:離された瞬間ではない
	*/
	static bool GetButtonUp(unsigned char key);

	/*!
		@fn		ClearBuffer
		@brief	入力バッファをクリアする
		@detail	STLのfillを使って入力バッファをNULLで初期化する
	*/
	static void ClearBuffer();
private:
	/*!
		@brief	コンストラクタ
	*/
	explicit Keyboard();

	/*!
		@fn			ToUpperAlpha
		@brief		大文字アルファベットへ変換
		@detail		アルファベットじゃなかったら引数ををそのまま返す
		@param[in]	変換文字
		@return		変換後文字
	*/
	static unsigned char ToUpperAlpha(unsigned char value);

	/*!
		@var	c_KeyNum
		@brief	キーの数
	*/
	static constexpr unsigned int c_KeyNum = 256;

	/*!
		@struct	Buffer
		@brief	入力バッファを保存する構造体
	*/
	struct Buffer
	{
		unsigned char values[c_KeyNum];
	};

	/*!
		@var	m_CurrentBuffer
		@brief	現在の入力バッファ
	*/
	static Buffer m_CurrentBuffer;

	/*!
		@var	m_PrevBuffer
		@brief	1フレーム前の入力バッファ
	*/
	static Buffer m_PrevBuffer;
};