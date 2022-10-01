class IMultiplication : public IUnknown
{
    public:
        virtual HRESULT __stdcall MultiplicationOfTwoIntegers(int,int,int*) = 0;;   
};

class IDivision : public IUnknown
{
    public:
        virtual HRESULT __stdcall DivisionOfTwoIntegers(int,int,int*) = 0;
};


const CLSID CLSID_MultiplicationDivision = 
// {6FAC6CE2-00B2-4A45-92C4-A6A0ADACF603}
 {0x6fac6ce2, 0xb2, 0x4a45, 0x92, 0xc4, 0xa6, 0xa0, 0xad, 0xac, 0xf6, 0x3};

const IID IID_IMultiplication = 
// {9B6F2B46-BB59-4481-BD39-11679D1D88E8} 
{0x9b6f2b46, 0xbb59, 0x4481, 0xbd, 0x39, 0x11, 0x67, 0x9d, 0x1d, 0x88, 0xe8};

const IID IID_IDivision = 
// {7B371238-1466-4568-A898-D2435F3F473A} 
{0x7b371238, 0x1466, 0x4568, 0xa8, 0x98, 0xd2, 0x43, 0x5f, 0x3f, 0x47, 0x3a};

