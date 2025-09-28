/PROG  CALIB	  Macro
/ATTR
OWNER		= MNEDITOR;
COMMENT		= "";
PROG_SIZE	= 1699;
CREATE		= DATE 25-08-23  TIME 11:01:20;
MODIFIED	= DATE 25-09-24  TIME 14:19:10;
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
	X =  1061.314  mm,	Y =  -166.264  mm,	Z =   124.235  mm,
	W =  -122.115 deg,	P =    87.500 deg,	R =   -34.224 deg
};
P[2]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1048.376  mm,	Y =  -167.157  mm,	Z =   107.860  mm,
	W =  -122.298 deg,	P =    87.505 deg,	R =   -34.374 deg
};
P[3]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1038.093  mm,	Y =  -174.721  mm,	Z =   108.815  mm,
	W =  -122.432 deg,	P =    87.509 deg,	R =   -34.541 deg
};
P[4]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1051.423  mm,	Y =  -170.567  mm,	Z =    78.515  mm,
	W =  -122.434 deg,	P =    87.517 deg,	R =   -34.542 deg
};
P[5]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1051.617  mm,	Y =  -203.794  mm,	Z =    82.770  mm,
	W =  -122.342 deg,	P =    87.512 deg,	R =   -34.449 deg
};
P[6]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1051.597  mm,	Y =  -135.022  mm,	Z =    82.717  mm,
	W =  -122.447 deg,	P =    87.517 deg,	R =   -34.553 deg
};
P[7]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1050.965  mm,	Y =  -271.063  mm,	Z =    55.021  mm,
	W =    -9.389 deg,	P =    79.949 deg,	R =    78.296 deg
};
P[8]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1051.786  mm,	Y =  -341.527  mm,	Z =   -70.924  mm,
	W =    -2.952 deg,	P =    68.284 deg,	R =    84.596 deg
};
P[9]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1051.709  mm,	Y =    22.212  mm,	Z =   158.523  mm,
	W =  -173.703 deg,	P =    70.446 deg,	R =   -85.919 deg
};
P[10]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =   973.097  mm,	Y =  -166.251  mm,	Z =    47.722  mm,
	W =   -95.086 deg,	P =    77.643 deg,	R =    -7.290 deg
};
P[11]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =   921.753  mm,	Y =  -146.309  mm,	Z =    27.684  mm,
	W =   -92.857 deg,	P =    71.307 deg,	R =    -5.082 deg
};
P[12]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1200.157  mm,	Y =  -179.555  mm,	Z =   144.391  mm,
	W =    87.855 deg,	P =    75.014 deg,	R =   175.642 deg
};
P[13]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1178.870  mm,	Y =  -243.547  mm,	Z =   125.069  mm,
	W =  -122.128 deg,	P =    87.520 deg,	R =   -10.816 deg
};
P[14]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =   991.844  mm,	Y =  -153.770  mm,	Z =   120.487  mm,
	W =  -122.096 deg,	P =    87.523 deg,	R =   -45.559 deg
};
P[15]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =   958.806  mm,	Y =   -51.519  mm,	Z =   101.842  mm,
	W =   -63.122 deg,	P =    87.684 deg,	R =     2.323 deg
};
/END
