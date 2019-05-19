struct VS_IN
{
        float4 pos : POSITION0;
        float4 col : COLOR0;
		float2 tex : TEXCOORD;
};
 
struct VS_OUT
{
        float4 pos : SV_POSITION;
        float4 col : COLOR0;
		float2 tex : TEXCOORD;
};
 
 
VS_OUT vs_main( VS_IN input )
{
        VS_OUT output;
 
        output.pos = input.pos;
        output.col = input.col;
		output.tex = input.tex;
        return output;
}
