void main()
{
	float2 uv = gl_TexCoord[0].xy;
	float pattern = (uv.x < 0.5 && uv.y < 0.5) || (uv.x > 0.5f && uv.y > 0.5f) ? 0.5 : 1;
	gl_FragColor = float4(1,0,1,1) * pattern;
}