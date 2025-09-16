/PROG  CALIB	  Macro
/ATTR
OWNER		= MNEDITOR;
COMMENT		= "";
PROG_SIZE	= 1699;
CREATE		= DATE 25-08-23  TIME 11:01:20;
MODIFIED	= DATE 25-08-23  TIME 11:39:20;
FILE_NAME	= ;
VERSION		= 0;
LINE_COUNT	= 40;
MEMORY_SIZE	= 2047;
PROTECT		= READ_WRITE;
TCD:  STACK_SIZE	= 0,
      TASK_PRIORITY	= 50,
      TIME_SLICE	= 0,
      BUSY_LAMP_OFF	= 0,
      ABORT_REQUEST	= 0,
      PAUSE_REQUEST	= 0;
DEFAULT_GROUP	= 1,*,*,*,*;
CONTROL_CODE	= 00000000 00000000;
/APPL
/MN
   1:   ;
   2:  UFRAME_NUM=0 ;
   3:  UTOOL_NUM=10 ;
   4:  OVERRIDE=30% ;
   5:   ;
   6:J P[1] 100% FINE    ;
   7:  CALL GRAB_CAL    ;
   8:L P[2] 500mm/sec FINE    ;
   9:  CALL GRAB_CAL    ;
  10:L P[3] 500mm/sec FINE    ;
  11:  CALL GRAB_CAL    ;
  12:   ;
  13:L P[4] 500mm/sec FINE    ;
  14:  CALL GRAB_CAL    ;
  15:L P[5] 500mm/sec FINE    ;
  16:  CALL GRAB_CAL    ;
  17:L P[6] 500mm/sec FINE    ;
  18:  CALL GRAB_CAL    ;
  19:   ;
  20:L P[7] 500mm/sec FINE    ;
  21:  CALL GRAB_CAL    ;
  22:L P[8] 500mm/sec FINE    ;
  23:  CALL GRAB_CAL    ;
  24:L P[9] 500mm/sec FINE    ;
  25:  CALL GRAB_CAL    ;
  26:   ;
  27:L P[10] 500mm/sec FINE    ;
  28:  CALL GRAB_CAL    ;
  29:L P[11] 500mm/sec FINE    ;
  30:  CALL GRAB_CAL    ;
  31:L P[12] 500mm/sec FINE    ;
  32:  CALL GRAB_CAL    ;
  33:   ;
  34:L P[13] 500mm/sec FINE    ;
  35:  CALL GRAB_CAL    ;
  36:L P[14] 500mm/sec FINE    ;
  37:  CALL GRAB_CAL    ;
  38:L P[15] 500mm/sec FINE    ;
  39:  CALL GRAB_CAL    ;
  40:   ;
/POS
P[1]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1172.508  mm,	Y =  -166.277  mm,	Z =   125.344  mm,
	W =  -121.838 deg,	P =    87.495 deg,	R =   -33.946 deg
};
P[2]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1156.050  mm,	Y =  -166.265  mm,	Z =   125.272  mm,
	W =  -122.005 deg,	P =    87.503 deg,	R =   -34.112 deg
};
P[3]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1146.052  mm,	Y =  -166.259  mm,	Z =   125.213  mm,
	W =  -122.058 deg,	P =    87.503 deg,	R =   -34.167 deg
};
P[4]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1146.028  mm,	Y =  -166.246  mm,	Z =    84.800  mm,
	W =  -122.190 deg,	P =    87.511 deg,	R =   -34.298 deg
};
P[5]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1146.003  mm,	Y =  -191.350  mm,	Z =    84.723  mm,
	W =  -122.399 deg,	P =    87.525 deg,	R =   -34.504 deg
};
P[6]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1145.996  mm,	Y =  -133.064  mm,	Z =    84.667  mm,
	W =  -122.374 deg,	P =    87.522 deg,	R =   -34.481 deg
};
P[7]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1160.200  mm,	Y =  -311.236  mm,	Z =    77.348  mm,
	W =    -9.412 deg,	P =    79.946 deg,	R =    78.273 deg
};
P[8]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1156.501  mm,	Y =  -374.339  mm,	Z =   -16.414  mm,
	W =    -2.371 deg,	P =    65.829 deg,	R =    85.056 deg
};
P[9]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1151.431  mm,	Y =  -467.185  mm,	Z =  -224.624  mm,
	W =     -.022 deg,	P =    47.118 deg,	R =    87.043 deg
};
P[10]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1033.612  mm,	Y =  -166.264  mm,	Z =    68.843  mm,
	W =   -92.519 deg,	P =    73.062 deg,	R =    -4.812 deg
};
P[11]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, 0',
	X =   960.499  mm,	Y =  -170.576  mm,	Z =   -38.447  mm,
	W =   -90.418 deg,	P =    60.743 deg,	R =    -2.863 deg
};
P[12]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, 0',
	X =   889.884  mm,	Y =  -166.243  mm,	Z =   -82.974  mm,
	W =   -89.753 deg,	P =    52.648 deg,	R =    -2.310 deg
};
P[13]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1278.398  mm,	Y =  -222.667  mm,	Z =   125.129  mm,
	W =  -122.027 deg,	P =    87.512 deg,	R =   -10.716 deg
};
P[14]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1443.414  mm,	Y =  -290.135  mm,	Z =   124.959  mm,
	W =  -122.087 deg,	P =    87.520 deg,	R =     9.647 deg
};
P[15]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1056.218  mm,	Y =   -63.051  mm,	Z =   124.986  mm,
	W =  -122.220 deg,	P =    87.514 deg,	R =   -57.781 deg
};
/END
