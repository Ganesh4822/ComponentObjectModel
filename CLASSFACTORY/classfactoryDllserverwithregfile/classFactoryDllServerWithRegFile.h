#pragma once

/*
class IUnknown
{
    virtual HRESULT __stdcall QueryInterface(REFIID,void**) = 0;
}
*/
class ISum : public IUnknown
{
    public:
        virtual HRESULT __stdcall sumOfTwoIntegers(int, int, int*) = 0;

};

class ISubtract : public IUnknown
{
    public:
        virtual HRESULT __stdcall substractionOfTwoIntegers(int, int, int*) = 0;
};


// {AC241AC1-C5F0-4EFF-9EA8-1FF4F3221C6B}
const CLSID CLSID_SumSubtract = 
{0xac241ac1, 0xc5f0, 0x4eff, 0x9e, 0xa8, 0x1f, 0xf4, 0xf3, 0x22, 0x1c, 0x6b};


// {63A7E971-0437-424E-943D-4C5D583E2C50} 
const IID IID_ISum = 
{0x63a7e971, 0x437, 0x424e, 0x94, 0x3d, 0x4c, 0x5d, 0x58, 0x3e, 0x2c, 0x50};


// {A7B19B8B-4AB8-4D1A-9905-A1F648D827A9} 
const IID IID_ISubtract = 
{0xa7b19b8b, 0x4ab8, 0x4d1a, 0x99, 0x5, 0xa1, 0xf6, 0x48, 0xd8, 0x27, 0xa9};
