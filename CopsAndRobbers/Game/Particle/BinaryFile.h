///	<summary>
///	�o�C�i���t�@�C���������N���X
///	</summary>

#pragma once

#include <memory>

class BinaryFile
{
protected:

   //	�f�[�^
   std::unique_ptr<char[]> m_data;

   //	�T�C�Y
   unsigned int m_size;

public:

   //	�t�@�C�������w�肵�ă��[�h
   static BinaryFile LoadFile(const wchar_t* fileName);

   BinaryFile();

   //	���[�u�R���X�g���N�^
   BinaryFile(BinaryFile&& in);

   //	�A�N�Z�T
   char* GetData() { return m_data.get(); }
   unsigned int GetSize() { return m_size; }
};
