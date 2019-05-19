struct VS_IN
{
        float4 pos : POSITION0;
        float4 col : COLOR0;
};
 
struct VS_OUT
{
        float4 pos : SV_POSITION;
        float4 col : COLOR0;
};
 
 
VS_OUT vs_main( VS_IN input )
{
        VS_OUT output;
 
        output.pos = input.pos;
        output.col = input.col;
        return output;
}
