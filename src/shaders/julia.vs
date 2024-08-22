struct ps_in
{
	float4 pos : SV_Position;
	float2 texcoord : TEXCOORD;
};

ps_in
main(in float4 pos : POSITION,
	 in float2 texcoord : TEXCOORD)
{
	ps_in		output;


	output.pos = pos;
	output.texcoord = texcoord;

	return (output);
}

