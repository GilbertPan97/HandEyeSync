/PROG  CALIB	  Macro
/ATTR
OWNER		= MNEDITOR;
COMMENT		= "";
PROG_SIZE	= 1699;
CREATE		= DATE 25-08-23  TIME 11:01:20;
MODIFIED	= DATE 25-09-22  TIME 14:51:14;
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
	X =  1061.350  mm,	Y =  -166.264  mm,	Z =   146.113  mm,
	W =  -122.027 deg,	P =    87.498 deg,	R =   -34.134 deg
};
P[2]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1042.045  mm,	Y =  -166.255  mm,	Z =   144.751  mm,
	W =  -122.224 deg,	P =    87.503 deg,	R =   -34.334 deg
};
P[3]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1032.512  mm,	Y =  -166.244  mm,	Z =   129.347  mm,
	W =  -122.365 deg,	P =    87.505 deg,	R =   -34.473 deg
};
P[4]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1051.594  mm,	Y =  -166.255  mm,	Z =    72.636  mm,
	W =  -122.409 deg,	P =    87.514 deg,	R =   -34.516 deg
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
	X =  1050.912  mm,	Y =  -356.846  mm,	Z =   -27.781  mm,
	W =    -2.938 deg,	P =    68.313 deg,	R =    84.534 deg
};
P[9]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1046.603  mm,	Y =  -107.983  mm,	Z =   135.514  mm,
	W =  -161.270 deg,	P =    82.960 deg,	R =   -73.287 deg
};
P[10]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =   922.720  mm,	Y =  -166.257  mm,	Z =    42.003  mm,
	W =   -92.526 deg,	P =    73.073 deg,	R =    -4.818 deg
};
P[11]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, 0',
	X =   847.002  mm,	Y =  -170.569  mm,	Z =   -48.018  mm,
	W =   -90.422 deg,	P =    60.752 deg,	R =    -2.867 deg
};
P[12]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, 0',
	X =   791.944  mm,	Y =  -166.238  mm,	Z =  -125.081  mm,
	W =   -89.757 deg,	P =    52.656 deg,	R =    -2.312 deg
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
	X =  1307.208  mm,	Y =  -259.610  mm,	Z =   124.886  mm,
	W =  -122.093 deg,	P =    87.523 deg,	R =     9.641 deg
};
P[15]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =   957.217  mm,	Y =   -44.745  mm,	Z =   124.944  mm,
	W =  -122.293 deg,	P =    87.516 deg,	R =   -57.855 deg
};
/END
