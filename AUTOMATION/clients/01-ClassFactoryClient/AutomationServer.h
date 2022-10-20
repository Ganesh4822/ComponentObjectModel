#pragma once

class IMyMath : public IDispatch
{
    public:
        virtual HRESULT __stdcall SumOfTwoIntegers(int , int, int*) = 0;
        virtual HRESULT __stdcall SubstractionOfTwoIntegers(int, int, int*) = 0;

     
    

};

// {1A0D64DD-AC61-4C9D-8AB7-18C068F541AE}
const CLSID CLSID_IMyMath =   
    {0x1a0d64dd, 0xac61, 0x4c9d, 0x8a, 0xb7, 0x18, 0xc0, 0x68, 0xf5, 0x41, 0xae};

    // {512A46E1-5C89-4A76-B1CD-919954580D9F}
const IID IID_IMyMath = 
    {0x512a46e1, 0x5c89, 0x4a76, 0xb1, 0xcd, 0x91, 0x99, 0x54, 0x58, 0xd, 0x9f};