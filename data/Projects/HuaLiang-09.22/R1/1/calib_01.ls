/PROG  CALIB_01	  Macro
/ATTR
OWNER		= SHIFT;
COMMENT		= "";
PROG_SIZE	= 1733;
CREATE		= DATE 25-09-22  TIME 16:00:28;
MODIFIED	= DATE 25-09-22  TIME 16:09:12;
FILE_NAME	= CALIB;
VERSION		= 0;
LINE_COUNT	= 40;
MEMORY_SIZE	= 2081;
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
	X =  1069.381  mm,	Y =  -692.393  mm,	Z =   144.249  mm,
	W =  -122.027 deg,	P =    87.498 deg,	R =   -34.134 deg
};
P[2]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1050.076  mm,	Y =  -692.384  mm,	Z =   142.887  mm,
	W =  -122.224 deg,	P =    87.503 deg,	R =   -34.334 deg
};
P[3]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1040.543  mm,	Y =  -692.373  mm,	Z =   127.483  mm,
	W =  -122.365 deg,	P =    87.505 deg,	R =   -34.473 deg
};
P[4]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1059.625  mm,	Y =  -692.384  mm,	Z =    70.772  mm,
	W =  -122.409 deg,	P =    87.514 deg,	R =   -34.516 deg
};
P[5]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1059.648  mm,	Y =  -729.923  mm,	Z =    80.906  mm,
	W =  -122.342 deg,	P =    87.512 deg,	R =   -34.449 deg
};
P[6]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1059.628  mm,	Y =  -661.151  mm,	Z =    80.853  mm,
	W =  -122.447 deg,	P =    87.517 deg,	R =   -34.553 deg
};
P[7]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1058.996  mm,	Y =  -797.193  mm,	Z =    53.157  mm,
	W =    -9.389 deg,	P =    79.949 deg,	R =    78.296 deg
};
P[8]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1058.943  mm,	Y =  -882.975  mm,	Z =   -29.645  mm,
	W =    -2.938 deg,	P =    68.313 deg,	R =    84.534 deg
};
P[9]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1054.634  mm,	Y =  -634.112  mm,	Z =   133.650  mm,
	W =  -161.270 deg,	P =    82.960 deg,	R =   -73.287 deg
};
P[10]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =   930.751  mm,	Y =  -692.386  mm,	Z =    40.139  mm,
	W =   -92.526 deg,	P =    73.073 deg,	R =    -4.818 deg
};
P[11]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, 0',
	X =   855.033  mm,	Y =  -696.698  mm,	Z =   -49.882  mm,
	W =   -90.422 deg,	P =    60.752 deg,	R =    -2.867 deg
};
P[12]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, 0',
	X =   799.975  mm,	Y =  -692.367  mm,	Z =  -126.945  mm,
	W =   -89.757 deg,	P =    52.656 deg,	R =    -2.312 deg
};
P[13]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1186.901  mm,	Y =  -769.676  mm,	Z =   123.205  mm,
	W =  -122.128 deg,	P =    87.520 deg,	R =   -10.816 deg
};
P[14]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =  1071.869  mm,	Y =  -675.961  mm,	Z =   118.523  mm,
	W =  -106.626 deg,	P =    87.647 deg,	R =   -14.719 deg
};
P[15]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'F U T, 0, 0, -1',
	X =   977.778  mm,	Y =  -654.341  mm,	Z =   118.385  mm,
	W =  -105.944 deg,	P =    87.813 deg,	R =   -34.573 deg
};
/END
