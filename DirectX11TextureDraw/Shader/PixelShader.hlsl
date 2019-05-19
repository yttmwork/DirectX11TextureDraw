struct PS_IN
{
        float4 pos : SV_POSITION;
        float4 col : COLOR0;
		float2 tex_pos : TEXCORD0;
};
 
Texture2D    Texture : register(t0[0]); // Textureをスロット0の0番目のテクスチャレジスタに設定
SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定

float4 ps_main( PS_IN input ) : SV_Target
{
        return Texture.Sample(Sampler, input.tex_pos);
}
