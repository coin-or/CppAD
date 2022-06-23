// includes
# include <stddef.h>
# include <math.h>

// typedefs
typedef double float_point_t;

// externals
// azmul
static float_point_t azmul(float_point_t x, float_point_t y)
{	if( x == 0.0 ) return 0.0;
	return x * y;
}

// sign
static float_point_t sign(float_point_t x)
{	if( x > 0.0 ) return 1.0;
	if( x == 0.0 ) return 0.0;
	return -1.0;
}

// This JIT function
int cppad_jit_gradient_det(
	size_t               nx              ,
	const float_point_t* x               ,
	size_t               ny              ,
	float_point_t*       y               ,
	size_t*              compare_change  )
{	// begin function body 

	// declare variables
	float_point_t v[412];
	size_t i;

	// check nx, ny
	if( nx != 25) return 1;
	if( ny != 25) return 2;

	// initialize
	*compare_change = 0;
	v[0]            = NAN; // const 

	// independent variables
	// set v[1+i] for i = 0, ..., nx-1
	for(i = 0; i < nx; ++i)
		v[1+i] = x[i];

	// constants
	// set v[1+nx+i] for i = 0, ..., nc-1
	// nc = 2
	v[1+nx+0] = 0;
	v[1+nx+1] = -1;

	// result nodes
	// set v[1+nx+nc+i] for i = 0, ..., n_result_node-1
	//n_result_node = 384
	v[28] = v[19]* v[25];
	v[29] = v[20]* v[24];
	v[30] = v[28]- v[29];
	v[31] = v[13]* v[30];
	v[32] = v[18]* v[25];
	v[33] = v[20]* v[23];
	v[34] = v[32]- v[33];
	v[35] = v[14]* v[34];
	v[36] = v[18]* v[24];
	v[37] = v[19]* v[23];
	v[38] = v[36]- v[37];
	v[39] = v[15]* v[38];
	v[42] = v[40]- v[41];
	v[43] = v[7]* v[42];
	v[44] = v[12]* v[30];
	v[45] = v[17]* v[25];
	v[46] = v[20]* v[22];
	v[47] = v[45]- v[46];
	v[48] = v[14]* v[47];
	v[49] = v[17]* v[24];
	v[50] = v[19]* v[22];
	v[51] = v[49]- v[50];
	v[52] = v[15]* v[51];
	v[55] = v[53]- v[54];
	v[56] = v[8]* v[55];
	v[57] = v[12]* v[34];
	v[58] = v[13]* v[47];
	v[59] = v[17]* v[23];
	v[60] = v[18]* v[22];
	v[61] = v[59]- v[60];
	v[62] = v[15]* v[61];
	v[65] = v[63]- v[64];
	v[66] = v[9]* v[65];
	v[67] = v[12]* v[38];
	v[68] = v[13]* v[51];
	v[69] = v[14]* v[61];
	v[72] = v[70]- v[71];
	v[73] = v[10]* v[72];
	v[76] = v[74]- v[75];
	v[79] = v[77]- v[78];
	v[80] = v[6]* v[79];
	v[81] = v[11]* v[30];
	v[82] = v[16]* v[25];
	v[83] = v[20]* v[21];
	v[84] = v[82]- v[83];
	v[85] = v[14]* v[84];
	v[86] = v[16]* v[24];
	v[87] = v[19]* v[21];
	v[88] = v[86]- v[87];
	v[89] = v[15]* v[88];
	v[92] = v[90]- v[91];
	v[93] = v[8]* v[92];
	v[94] = v[11]* v[34];
	v[95] = v[13]* v[84];
	v[96] = v[16]* v[23];
	v[97] = v[18]* v[21];
	v[98] = v[96]- v[97];
	v[99] = v[15]* v[98];
	v[102] = v[100]- v[101];
	v[103] = v[9]* v[102];
	v[104] = v[11]* v[38];
	v[105] = v[13]* v[88];
	v[106] = v[14]* v[98];
	v[109] = v[107]- v[108];
	v[110] = v[10]* v[109];
	v[113] = v[111]- v[112];
	v[116] = v[114]- v[115];
	v[117] = v[6]* v[116];
	v[120] = v[118]- v[119];
	v[121] = v[7]* v[120];
	v[122] = v[11]* v[47];
	v[123] = v[12]* v[84];
	v[124] = v[16]* v[22];
	v[125] = v[17]* v[21];
	v[126] = v[124]- v[125];
	v[127] = v[15]* v[126];
	v[130] = v[128]- v[129];
	v[131] = v[9]* v[130];
	v[132] = v[11]* v[51];
	v[133] = v[12]* v[88];
	v[134] = v[14]* v[126];
	v[137] = v[135]- v[136];
	v[138] = v[10]* v[137];
	v[141] = v[139]- v[140];
	v[144] = v[142]- v[143];
	v[145] = v[6]* v[144];
	v[148] = v[146]- v[147];
	v[149] = v[7]* v[148];
	v[152] = v[150]- v[151];
	v[153] = v[8]* v[152];
	v[154] = v[11]* v[61];
	v[155] = v[12]* v[98];
	v[156] = v[13]* v[126];
	v[159] = v[157]- v[158];
	v[160] = v[10]* v[159];
	v[163] = v[161]- v[162];
	v[166] = v[164]- v[165];
	v[167] = v[6]* v[166];
	v[170] = v[168]- v[169];
	v[171] = v[7]* v[170];
	v[174] = v[172]- v[173];
	v[175] = v[8]* v[174];
	v[178] = v[176]- v[177];
	v[179] = v[9]* v[178];
	v[182] = v[180]- v[181];
	v[183] = v[26]- v[5];
	v[184] = azmul( v[183], v[178] );
	v[185] = azmul( v[183], v[9] );
	v[186] = azmul( v[5], v[174] );
	v[187] = azmul( v[5], v[8] );
	v[188] = azmul( v[183], v[170] );
	v[189] = azmul( v[183], v[7] );
	v[190] = azmul( v[5], v[166] );
	v[191] = azmul( v[5], v[6] );
	v[192] = azmul( v[27], v[163] );
	v[193] = azmul( v[27], v[4] );
	v[194] = v[26]- v[193];
	v[195] = azmul( v[194], v[159] );
	v[196] = azmul( v[194], v[10] );
	v[197] = v[185]+ v[196];
	v[200] = v[198]- v[199];
	v[201] = azmul( v[197], v[126] );
	v[202] = azmul( v[197], v[13] );
	v[203] = azmul( v[200], v[98] );
	v[204] = azmul( v[200], v[12] );
	v[205] = azmul( v[197], v[61] );
	v[206] = azmul( v[197], v[11] );
	v[207] = azmul( v[193], v[152] );
	v[208] = azmul( v[193], v[8] );
	v[209] = azmul( v[194], v[148] );
	v[210] = azmul( v[194], v[7] );
	v[211] = azmul( v[193], v[144] );
	v[212] = azmul( v[193], v[6] );
	v[213] = v[26]- v[3];
	v[214] = azmul( v[213], v[137] );
	v[215] = azmul( v[213], v[10] );
	v[216] = v[187]+ v[215];
	v[219] = v[217]- v[218];
	v[220] = azmul( v[216], v[126] );
	v[221] = azmul( v[216], v[14] );
	v[222] = azmul( v[219], v[88] );
	v[223] = azmul( v[219], v[12] );
	v[224] = azmul( v[216], v[51] );
	v[225] = azmul( v[216], v[11] );
	v[226] = azmul( v[3], v[130] );
	v[227] = azmul( v[3], v[9] );
	v[228] = v[208]+ v[227];
	v[231] = v[229]- v[230];
	v[232] = azmul( v[228], v[126] );
	v[233] = azmul( v[228], v[15] );
	v[235] = v[26]- v[234];
	v[236] = azmul( v[235], v[21] );
	v[237] = azmul( v[235], v[17] );
	v[238] = azmul( v[234], v[22] );
	v[239] = azmul( v[234], v[16] );
	v[240] = azmul( v[231], v[84] );
	v[241] = azmul( v[231], v[12] );
	v[242] = azmul( v[228], v[47] );
	v[243] = azmul( v[228], v[11] );
	v[244] = azmul( v[213], v[120] );
	v[245] = azmul( v[213], v[7] );
	v[246] = azmul( v[3], v[116] );
	v[247] = azmul( v[3], v[6] );
	v[248] = azmul( v[27], v[113] );
	v[249] = azmul( v[27], v[2] );
	v[250] = v[26]- v[249];
	v[251] = azmul( v[250], v[109] );
	v[252] = azmul( v[250], v[10] );
	v[253] = v[189]+ v[252];
	v[256] = v[254]- v[255];
	v[257] = azmul( v[253], v[98] );
	v[258] = azmul( v[253], v[14] );
	v[259] = azmul( v[256], v[88] );
	v[260] = azmul( v[256], v[13] );
	v[261] = azmul( v[253], v[38] );
	v[262] = azmul( v[253], v[11] );
	v[263] = azmul( v[249], v[102] );
	v[264] = azmul( v[249], v[9] );
	v[265] = v[210]+ v[264];
	v[268] = v[266]- v[267];
	v[269] = azmul( v[265], v[98] );
	v[270] = azmul( v[265], v[15] );
	v[272] = v[26]- v[271];
	v[273] = azmul( v[272], v[21] );
	v[274] = azmul( v[272], v[18] );
	v[275] = azmul( v[271], v[23] );
	v[276] = azmul( v[271], v[16] );
	v[277] = azmul( v[268], v[84] );
	v[278] = azmul( v[268], v[13] );
	v[279] = azmul( v[265], v[34] );
	v[280] = azmul( v[265], v[11] );
	v[281] = azmul( v[250], v[92] );
	v[282] = azmul( v[250], v[8] );
	v[283] = v[245]+ v[282];
	v[286] = v[284]- v[285];
	v[287] = azmul( v[283], v[88] );
	v[288] = azmul( v[283], v[15] );
	v[290] = v[26]- v[289];
	v[291] = azmul( v[290], v[21] );
	v[292] = azmul( v[290], v[19] );
	v[293] = azmul( v[289], v[24] );
	v[294] = azmul( v[289], v[16] );
	v[295] = azmul( v[286], v[84] );
	v[296] = azmul( v[286], v[14] );
	v[298] = v[26]- v[297];
	v[299] = azmul( v[298], v[21] );
	v[300] = azmul( v[298], v[20] );
	v[302] = azmul( v[297], v[25] );
	v[304] = azmul( v[297], v[16] );
	v[305] = azmul( v[283], v[30] );
	v[307] = azmul( v[283], v[11] );
	v[308] = azmul( v[249], v[79] );
	v[310] = azmul( v[249], v[6] );
	v[311] = v[26]- v[1];
	v[312] = azmul( v[311], v[72] );
	v[314] = azmul( v[311], v[10] );
	v[315] = v[191]+ v[314];
	v[318] = v[316]- v[317];
	v[319] = azmul( v[315], v[61] );
	v[320] = azmul( v[315], v[14] );
	v[321] = azmul( v[318], v[51] );
	v[322] = azmul( v[318], v[13] );
	v[323] = azmul( v[315], v[38] );
	v[324] = azmul( v[315], v[12] );
	v[325] = azmul( v[1], v[65] );
	v[327] = azmul( v[1], v[9] );
	v[328] = v[212]+ v[327];
	v[331] = v[329]- v[330];
	v[332] = azmul( v[328], v[61] );
	v[333] = azmul( v[328], v[15] );
	v[335] = v[26]- v[334];
	v[336] = azmul( v[335], v[22] );
	v[337] = azmul( v[335], v[18] );
	v[338] = azmul( v[334], v[23] );
	v[339] = azmul( v[334], v[17] );
	v[340] = azmul( v[331], v[47] );
	v[341] = azmul( v[331], v[13] );
	v[342] = azmul( v[328], v[34] );
	v[343] = azmul( v[328], v[12] );
	v[344] = azmul( v[311], v[55] );
	v[346] = azmul( v[311], v[8] );
	v[347] = v[247]+ v[346];
	v[350] = v[348]- v[349];
	v[351] = azmul( v[347], v[51] );
	v[352] = azmul( v[347], v[15] );
	v[354] = v[26]- v[353];
	v[355] = azmul( v[354], v[22] );
	v[356] = azmul( v[354], v[19] );
	v[357] = azmul( v[353], v[24] );
	v[358] = azmul( v[353], v[17] );
	v[359] = azmul( v[350], v[47] );
	v[360] = azmul( v[350], v[14] );
	v[362] = v[26]- v[361];
	v[363] = azmul( v[362], v[22] );
	v[364] = azmul( v[362], v[20] );
	v[366] = azmul( v[361], v[25] );
	v[368] = azmul( v[361], v[17] );
	v[369] = azmul( v[347], v[30] );
	v[371] = azmul( v[347], v[12] );
	v[372] = azmul( v[1], v[42] );
	v[374] = azmul( v[1], v[7] );
	v[375] = v[310]+ v[374];
	v[378] = v[376]- v[377];
	v[379] = azmul( v[375], v[38] );
	v[381] = azmul( v[375], v[15] );
	v[383] = v[26]- v[382];
	v[384] = azmul( v[383], v[23] );
	v[385] = azmul( v[383], v[19] );
	v[386] = azmul( v[382], v[24] );
	v[387] = azmul( v[382], v[18] );
	v[388] = azmul( v[378], v[34] );
	v[390] = azmul( v[378], v[14] );
	v[392] = v[26]- v[391];
	v[393] = azmul( v[392], v[23] );
	v[394] = azmul( v[392], v[20] );
	v[396] = azmul( v[391], v[25] );
	v[398] = azmul( v[391], v[18] );
	v[399] = azmul( v[375], v[30] );
	v[401] = azmul( v[375], v[13] );
	v[403] = v[26]- v[402];
	v[404] = azmul( v[403], v[24] );
	v[406] = azmul( v[403], v[20] );
	v[408] = azmul( v[402], v[25] );
	v[410] = azmul( v[402], v[19] );

	// dependent variables
	// set y[i] for i = 0, ny-1
	y[0] = v[76];
	y[1] = v[248];
	y[2] = v[141];
	y[3] = v[192];
	y[4] = v[182];
	y[5] = v[309];
	y[6] = v[373];
	y[7] = v[345];
	y[8] = v[326];
	y[9] = v[313];
	y[10] = v[306];
	y[11] = v[370];
	y[12] = v[400];
	y[13] = v[389];
	y[14] = v[380];
	y[15] = v[303];
	y[16] = v[367];
	y[17] = v[397];
	y[18] = v[409];
	y[19] = v[405];
	y[20] = v[301];
	y[21] = v[365];
	y[22] = v[395];
	y[23] = v[407];
	y[24] = v[411];

	return 0;
}
