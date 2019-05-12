/*
	@file	Keyboard.cpp
	@brief	Windowsキーボード入力
	@date	2019/03/29
	@detail	キーコード確認用リンク:http://www.azaelia.net/factory/vk.html
	@author	番場 宥輝
*/
#include "Keyboard.h"
#include <algorithm>
#include <cctype>

/*!
	@var	m_CurrentBuffer
	@brief	現在の入力バッファ
			静的メンバの宣言
*/
Keyboard::Buffer Keyboard::m_CurrentBuffer;

/*!
	@var	m_PrevBuffer
	@brief	1フレーム前の入力バッファ
			静的メンバの宣言
*/

Keyboard::Buffer Keyboard::m_PrevBuffer;

/*!
	@brief	コンストラクタ
*/
Keyboard::Keyboard()
{
	ClearBuffer();
}

/*!
	@brief	デストラクタ
*/
Keyboard::~Keyboard()
{
	ClearBuffer();
}

/*!
	@fn		UpdateBuffer
	@brief	入力バッファの更新
	@detail	※入力判定を取得する下記関数の呼び出し前に行ってください
*/
void Keyboard::UpdateBuffer()
{
	//	古いバッファの更新
	m_PrevBuffer = m_CurrentBuffer;

	//	新しいバッファの更新
	GetKeyboardState(m_CurrentBuffer.values);
}

/*!
	@fn			GetButton
	@brief		キーの押下判定
	@param[in]	判定するキー
	@return		true:押されている  false:押されていない
*/
bool Keyboard::GetButton(unsigned char key)
{
	//	大文字と小文字で値が違うので
	//	アルファベットは大文字に変換してあげる
	auto value = ToUpperAlpha(key);
	return m_CurrentBuffer.values[value] & 0x80;
}

/*!
	@fn			GetButtonDown
	@brief		キーの押した瞬間の押下判定
	@param[in]	判定するキー
	@return		true:押された  false:押された瞬間ではない
*/
bool Keyboard::GetButtonDown(unsigned char key)
{
	//	大文字と小文字で値が違うので
	//	アルファベットは大文字に変換してあげる
	auto value = ToUpperAlpha(key);
	return (m_CurrentBuffer.values[value] ^ m_PrevBuffer.values[value]) & m_CurrentBuffer.values[value];
}

/*!
	@fn			GetButtonUp
	@brief		キーの離した瞬間の判定
	@param[in]	判定するキー
	@return		true:離された  false:離された瞬間ではない
*/
bool Keyboard::GetButtonUp(unsigned char key)
{
	//	大文字と小文字で値が違うので
	//	アルファベットは大文字に変換してあげる
	auto value = ToUpperAlpha(key);
	return (m_CurrentBuffer.values[value] ^ m_PrevBuffer.values[value]) & m_PrevBuffer.values[value];
}

/*!
	@fn		ClearBuffer
	@brief	入力バッファをクリアする
	@detail	STLのfillを使って入力バッファをNULLで初期化する
*/
void Keyboard::ClearBuffer()
{
	std::fill(m_CurrentBuffer.values, m_CurrentBuffer.values + c_KeyNum, NULL);
	std::fill(m_PrevBuffer.values, m_PrevBuffer.values + c_KeyNum, NULL);
}

/*!
	@fn			ToUpperAlpha
	@brief		大文字アルファベットへ変換
	@detail		アルファベットじゃなかったら引数ををそのまま返す
	@param[in]	変換文字
	@return		変換後文字
*/
unsigned char Keyboard::ToUpperAlpha(unsigned char value)
{
	//	アルファベットでなければ早々に返却
	if (!std::isalpha(value)) { return value; }

	//	大文字にして返却
	return toupper(value);
}