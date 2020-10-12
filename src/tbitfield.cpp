// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len >= 0)
    {
        BitLen = len;
        MemLen = len / 32 + 1;
        if ((len % (sizeof(TELEM) * 8)) > 0)
        {
            MemLen++;
        }
        for (int i = 0; i < MemLen; i++)
        {
            pMem[i] = 0;
        }
    }
    else throw "Lenght<0";
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[bf.MemLen];
    for (int i = 0; i < bf.MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n >= 0)
    {
        return n / 32;
    }
    else throw "Index<0";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n >= 0)
    {
        int tmp = n % 32;
        TELEM one = 1;
        return one << tmp;
    }
    else throw "Mask<0";
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return 0;
}

void TBitField::SetBit(const int n) // установить бит+
{
    int i = GetMemIndex(n);
    TELEM m = GetMemMask(n);
    pMem[i] = pMem[i] | m;
}

void TBitField::ClrBit(const int n) // очистить бит+
{
    int i = GetMemIndex(n);
    TELEM m = GetMemMask(n);
    pMem[i] = pMem[i] & (~m);
}

int TBitField::GetBit(const int n) const // получить значение бита+
{
    int i = GetMemIndex(n);
    TELEM m = GetMemMask(n);
    return pMem[i] & m;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание+
{
    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        pMem = new TELEM[bf.MemLen];
        MemLen = bf.MemLen;
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < bf.MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
    return (*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение+
{
    if (BitLen != bf.BitLen)
    {
        return false;
    }
    for (int i = 0; i < MemLen-1; i++)
    {
        if (pMem[i]!=bf.pMem[i])
        {
            return false;
        }
    }
    for (int i = (MemLen - 1) * 32; i < BitLen; i++)
    {
        if (GetBit(i)!=bf.GetBit(i))
        {
            return false;
        }
    }
    return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение+
{
  return !(*this==bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"+
{
    int flag = 1, max = BitLen;
    if (BitLen < bf.BitLen)
    {
        max = bf.BitLen;
        flag = 0;
    }
    TBitField res(max);
    if (flag == 1)
    {
        for (int i = 0; i < bf.MemLen - 1; i++)
        {
            res.pMem[i] = bf.pMem[i];
        }
        for (int i = (bf.MemLen - 1) * 32; i < bf.BitLen; i++)
        {
            if (bf.GetBit(i))
            {
                res.SetBit(i);
            }
        }
        for (int i = 0; i < MemLen; i++)
        {
            res.pMem[i] |= pMem[i];
        }
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"+
{
    int flag = 1, max = BitLen;
    if (BitLen < bf.BitLen)
    {
        max = bf.BitLen;
        flag = 0;
    }
    TBitField res(max);
    if (flag == 1)
    {
        for (int i = 0; i < bf.MemLen - 1; i++)
        {
            res.pMem[i] = bf.pMem[i];
        }
        for (int i = (bf.MemLen - 1) * 32; i < bf.BitLen; i++)
        {
            if (bf.GetBit(i))
            {
                res.SetBit(i);
            }
        }
        for (int i = 0; i < MemLen; i++)
        {
            res.pMem[i] &= pMem[i];
        }
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание+
{
    TBitField res(*this);
    for (int i = 0; i < res.MemLen; i++)
    {
        res.pMem[i] = ~pMem[i];
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод+
{
    char x;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> x;
        if (x == '0')
        {
            bf.ClrBit(i);
        }
        else if (x == '1')
        {
            bf.SetBit(i);
        }
        else 
        { 
            break;
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод+
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i))
        {
            ostr << '1';
        }
        else ostr << '0';
    }
    return ostr;
}
