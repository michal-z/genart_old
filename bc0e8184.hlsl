#if VS_0 || PS_0

#define RSI_0 "RootFlags(0)"

#endif // #if VS_0 || PS_0
//-------------------------------------------------------------------------------------------------
#if VS_0

[RootSignature(RSI_0)]
float4 VS0_Main(uint id : SV_VertexID) : SV_Position {
	float2 positions[3] = { float2(-1.0f, -1.0f), float2(0.0f, 1.0f), float2(1.0f, -1.0f) };
	return float4(positions[id], 0.0f, 1.0f);
}

#endif // #if VS_0
//-------------------------------------------------------------------------------------------------
#if PS_0

[RootSignature(RSI_0)]
float4 PS0_Main(float4 position : SV_Position) : SV_Target0 {
	return float4(0.0f, 0.5f, 0.0f, 1.0f);
}

#endif // #if PS_0
