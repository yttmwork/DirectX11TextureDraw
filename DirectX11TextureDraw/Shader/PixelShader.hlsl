struct PS_IN
{
        float4 pos : SV_POSITION;
        float4 col : COLOR0;
};
 
 
float4 ps_main( PS_IN input ) : SV_Target
{
        return input.col;
}