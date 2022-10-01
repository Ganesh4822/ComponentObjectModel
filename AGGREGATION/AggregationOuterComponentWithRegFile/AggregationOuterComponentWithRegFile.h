class ISum : public IUnknown
{
    public:
        virtual HRESULT __stdcall SumofTwoIntegers(int, int, int*) = 0;
};

class ISubtract : public IUnknown
{
    public:
        virtual HRESULT __stdcall SubtractionOfTwoIntegers(int,int,int*) = 0;
};

const CLSID CLSID_SumSubstract = 
// {28FCFC72-B4E0-4DFC-B8BF-964ED85C1E38}
{0x28fcfc72, 0xb4e0, 0x4dfc, 0xb8, 0xbf, 0x96, 0x4e, 0xd8, 0x5c, 0x1e, 0x38};

const IID IID_ISum = 
// {3FB44388-EFAF-4BFC-96D9-1A69AE78F3F3}
 
{0x3fb44388, 0xefaf, 0x4bfc, 0x96, 0xd9, 0x1a, 0x69, 0xae, 0x78, 0xf3, 0xf3};


const IID IID_ISubstract = 

// {0E9819E4-EE48-4DE7-B2AB-5ACDD3D9D4B2}
{0xe9819e4, 0xee48, 0x4de7, 0xb2, 0xab, 0x5a, 0xcd, 0xd3, 0xd9, 0xd4, 0xb2};
