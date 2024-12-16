void pixel_arr_init(short pixel_arr[], int pixel_arr_size)
{
    if (pixel_arr_size != 389) {
        return;
    }

    for (int i = 0; i < pixel_arr_size; i++) {
        pixel_arr[i] = -1;
    }
    
    // 30172	O'Hare	South	40890
    // 30171	O'Hare	North	40890
    pixel_arr[172] = 0;
    pixel_arr[171] = 65;

    // 30159	Rosemont	North	40820
    // 30160	Rosemont	South	40820
    pixel_arr[160] = 1;
    pixel_arr[159] = 64;

    // 30045	Cumberland	South	40230
    // 30044	Cumberland	North	40230
    pixel_arr[45] = 2;
    pixel_arr[44] = 63;

    // 30145	Harlem (Blue Line-O'Hare Branch)	North	40750
    // 30146	Harlem (Blue Line-O'Hare Branch)	South	40750
    pixel_arr[146] = 3;
    pixel_arr[145] = 62; 

    // 30248	Jefferson Park	South	41280
    // 30247	Jefferson Park	North	41280
    pixel_arr[248] = 4;
    pixel_arr[247] = 61;

    // 30259	Montrose (Blue Line)	North	41330
    // 30260	Montrose (Blue Line)	South	41330
    pixel_arr[260] = 5;
    pixel_arr[259] = 60;

    // 30107	Irving Park (Blue Line)	North	40550
    // 30108	Irving Park (Blue Line)	South	40550
    pixel_arr[108] = 6;
    pixel_arr[107] = 59;

    // 30240	Addison (Blue Line)	South	41240
    // 30239	Addison (Blue Line)	North	41240
    pixel_arr[240] = 7;
    pixel_arr[239] = 58;

    // 30013	Belmont (Blue Line)	South	40060
    // 30012	Belmont (Blue Line)	North	40060
    pixel_arr[13] = 8;
    pixel_arr[12] = 57;

    // 30197	Logan Square	North	41020
    // 30198	Logan Square	South	41020
    pixel_arr[198] = 9;
    pixel_arr[197] = 56;

    // 30111	California (Blue Line-O'Hare Branch)	North	40570
    // 30112	California (Blue Line-O'Hare Branch)	South	40570
    pixel_arr[112] = 10;
    pixel_arr[111] = 55;  

    // 30130	Western (Blue Line-O'Hare Branch)	South	40670
    // 30129	Western (Blue Line-O'Hare Branch)	North	40670
    pixel_arr[130] = 11;
    pixel_arr[129] = 54;

    // 30116	Damen (Blue Line-O'Hare Branch)	South	40590
    // 30115	Damen (Blue Line-O'Hare Branch)	North	40590
    pixel_arr[116] = 12;
    pixel_arr[115] = 53;

    // 30062	Division	North	40320
    // 30063	Division	South	40320
    pixel_arr[63] = 13;
    pixel_arr[62] = 52;

    // 30271	Chicago (Blue Line)	North	41410
    // 30272	Chicago (Blue Line)	South	41410
    pixel_arr[272] = 14;
    pixel_arr[271] = 51;

    // 30096	Grand (Blue Line)	South	40490
    // 30095	Grand (Blue Line)	North	40490
    pixel_arr[96] = 15;
    pixel_arr[95] = 50;

    // 30375	Clark/Lake (Subway)	North	40380
    // 30374	Clark/Lake (Subway)	South	40380
    pixel_arr[374] = 16;
    pixel_arr[375] = 49;

    // 30073	Washington (Blue Line)	South	40370
    // 30072	Washington (Blue Line)	North	40370
    pixel_arr[73] = 17;
    pixel_arr[72] = 48;

    // 30153	Monroe (Blue Line)	North	40790
    // 30154	Monroe (Blue Line)	South	40790
    pixel_arr[154] = 18;
    pixel_arr[153] = 47;

    // 30015	Jackson (Blue Line)	South	40070
    // 30014	Jackson (Blue Line)	North	40070
    pixel_arr[15] = 19;
    pixel_arr[14] = 46;

    // 30261	LaSalle	East	41340
    // 30262	LaSalle	West	41340
    pixel_arr[262] = 20;
    pixel_arr[261] = 45;

    // 30084	Clinton (Blue Line)	East	40430
    // 30085	Clinton (Blue Line)	West	40430
    pixel_arr[85] = 21;
    pixel_arr[84] = 44;

    // 30069	UIC-Halsted	West	40350
    // 30068	UIC-Halsted	East	40350
    pixel_arr[69] = 22;
    pixel_arr[68] = 43;

    // 30092	Racine	East	40470
    // 30093	Racine	West	40470
    pixel_arr[92] = 23;
    pixel_arr[93] = 42;

    // 30158	Illinois Medical District	West	40810
    // 30157	Illinois Medical District	East	40810
    pixel_arr[158] = 24;
    pixel_arr[157] = 41;

    // 30043	Western (Blue Line-Forest Park Branch)	West	40220
    // 30042	Western (Blue Line-Forest Park Branch)	East	40220
    pixel_arr[43] = 25;
    pixel_arr[42] = 40;

    // 30048	Kedzie-Homan	East	40250
    // 30049	Kedzie-Homan	West	40250
    pixel_arr[49] = 26;
    pixel_arr[48] = 39;

    // 30180	Pulaski (Blue Line-Forest Park Branch)	West	40920
    // 30179	Pulaski (Blue Line-Forest Park Branch)	East	40920
    pixel_arr[180] = 27;
    pixel_arr[179] = 38;

    // 30188	Cicero-Forest Park	West	40970
    // 30187	Cicero-Forest Park	East	40970
    pixel_arr[188] = 28;
    pixel_arr[187] = 37;

    // 30001	Austin (Blue Line)	East	40010
    // 30002	Austin (Blue Line)	West	40010
    pixel_arr[2] = 29;
    pixel_arr[1] = 36;

    // 30034	Oak Park (Blue Line)	East	40180
    // 30035	Oak Park (Blue Line)	West	40180
    pixel_arr[35] = 30;
    pixel_arr[34] = 35;

    // 30189	Harlem-Forest Park	East	40980
    // 30190	Harlem-Forest Park	West	40980
    pixel_arr[189] = 31;
    pixel_arr[190] = 34;

    // 30077	Forest Park	West	40390
    // 30076	Forest Park	East	40390
    pixel_arr[77] = 32;
    pixel_arr[76] = 33;

}