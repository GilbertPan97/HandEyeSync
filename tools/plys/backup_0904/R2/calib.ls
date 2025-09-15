/PROG  CALIB	  Macro
/ATTR
OWNER		= MNEDITOR;
COMMENT		= "";
PROG_SIZE	= 2411;
CREATE		= DATE 25-08-21  TIME 16:56:02;
MODIFIED	= DATE 25-08-21  TIME 17:47:30;
FILE_NAME	= ;
VERSION		= 0;
LINE_COUNT	= 77;
MEMORY_SIZE	= 2739;
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
   1:  UFRAME_NUM=0 ;
   2:  UTOOL_NUM=10 ;
   3:   ;
   4:   ;
   5:J P[1] 100% FINE    ;
   6:  CALL MOVE(10,100) ;
   7:  WAIT    .50(sec) ;
   8:  CALL MOVE(0,100) ;
   9:  PAUSE ;
  10:   ;
  11:L P[2] 2000mm/sec FINE    ;
  12:  CALL MOVE(10,100) ;
  13:  WAIT   1.00(sec) ;
  14:  CALL MOVE(0,100) ;
  15:  PAUSE ;
  16:   ;
  17:L P[3] 2000mm/sec FINE    ;
  18:  CALL MOVE(10,100) ;
  19:  WAIT    .50(sec) ;
  20:  CALL MOVE(0,100) ;
  21:  PAUSE ;
  22:   ;
  23:L P[4] 2000mm/sec FINE    ;
  24:  CALL MOVE(10,100) ;
  25:  WAIT    .50(sec) ;
  26:  CALL MOVE(0,100) ;
  27:  PAUSE ;
  28:L P[5] 2000mm/sec FINE    ;
  29:  CALL MOVE(10,100) ;
  30:  WAIT    .50(sec) ;
  31:  CALL MOVE(0,100) ;
  32:  PAUSE ;
  33:L P[6] 2000mm/sec FINE    ;
  34:  CALL MOVE(10,100) ;
  35:  WAIT    .50(sec) ;
  36:  CALL MOVE(0,100) ;
  37:   ;
  38:L P[7] 2000mm/sec FINE    ;
  39:  CALL MOVE(10,100) ;
  40:  WAIT    .50(sec) ;
  41:  CALL MOVE(0,100) ;
  42:L P[8] 2000mm/sec FINE    ;
  43:  CALL MOVE(10,100) ;
  44:  WAIT    .50(sec) ;
  45:  CALL MOVE(0,100) ;
  46:L P[9] 2000mm/sec FINE    ;
  47:  CALL MOVE(10,100) ;
  48:  WAIT    .50(sec) ;
  49:  CALL MOVE(0,100) ;
  50:   ;
  51:L P[10] 2000mm/sec FINE    ;
  52:  CALL MOVE(10,100) ;
  53:  WAIT    .50(sec) ;
  54:  CALL MOVE(0,100) ;
  55:L P[11] 2000mm/sec FINE    ;
  56:  CALL MOVE(10,100) ;
  57:  WAIT    .50(sec) ;
  58:  CALL MOVE(0,100) ;
  59:L P[12] 2000mm/sec FINE    ;
  60:  CALL MOVE(10,100) ;
  61:  WAIT    .50(sec) ;
  62:  CALL MOVE(0,100) ;
  63:   ;
  64:L P[13] 2000mm/sec FINE    ;
  65:  CALL MOVE(10,100) ;
  66:  WAIT    .50(sec) ;
  67:  CALL MOVE(0,100) ;
  68:L P[14] 2000mm/sec FINE    ;
  69:  CALL MOVE(10,100) ;
  70:  WAIT    .50(sec) ;
  71:  CALL MOVE(0,100) ;
  72:L P[15] 2000mm/sec FINE    ;
  73:   ;
  74:  CALL MOVE(10,100) ;
  75:  WAIT    .50(sec) ;
  76:  CALL MOVE(0,100) ;
  77:   ;
/POS
P[1]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1185.704  mm,	Y =    23.405  mm,	Z =   116.720  mm,
	W =  -119.978 deg,	P =    88.650 deg,	R =   -27.379 deg
};
P[2]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1173.854  mm,	Y =    23.420  mm,	Z =   116.674  mm,
	W =  -120.158 deg,	P =    88.655 deg,	R =   -27.558 deg
};
P[3]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1163.266  mm,	Y =    23.422  mm,	Z =   116.612  mm,
	W =  -120.402 deg,	P =    88.660 deg,	R =   -27.801 deg
};
P[4]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1173.802  mm,	Y =    23.398  mm,	Z =    85.403  mm,
	W =  -120.741 deg,	P =    88.667 deg,	R =   -28.144 deg
};
P[5]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1173.783  mm,	Y =    54.279  mm,	Z =    85.349  mm,
	W =  -121.124 deg,	P =    88.673 deg,	R =   -28.527 deg
};
P[6]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1173.773  mm,	Y =     2.216  mm,	Z =    85.303  mm,
	W =  -121.332 deg,	P =    88.677 deg,	R =   -28.737 deg
};
P[7]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1173.821  mm,	Y =   -62.310  mm,	Z =    85.766  mm,
	W =   -14.096 deg,	P =    84.093 deg,	R =    78.435 deg
};
P[8]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1173.789  mm,	Y =  -104.375  mm,	Z =    14.883  mm,
	W =    -7.119 deg,	P =    75.515 deg,	R =    85.322 deg
};
P[9]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1173.747  mm,	Y =  -222.575  mm,	Z =   -98.142  mm,
	W =    -4.778 deg,	P =    62.281 deg,	R =    87.520 deg
};
P[10]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1095.199  mm,	Y =    20.750  mm,	Z =    55.325  mm,
	W =   -95.778 deg,	P =    78.749 deg,	R =    -3.236 deg
};
P[11]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1020.905  mm,	Y =    15.909  mm,	Z =   -18.655  mm,
	W =   -94.199 deg,	P =    68.174 deg,	R =    -1.717 deg
};
P[12]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =   968.524  mm,	Y =    26.007  mm,	Z =   -78.552  mm,
	W =   -93.689 deg,	P =    59.423 deg,	R =    -1.259 deg
};
P[13]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1240.902  mm,	Y =   -16.818  mm,	Z =   116.558  mm,
	W =  -120.672 deg,	P =    88.667 deg,	R =   -16.380 deg
};
P[14]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1316.356  mm,	Y =   -49.483  mm,	Z =   116.488  mm,
	W =  -120.879 deg,	P =    88.673 deg,	R =    -5.228 deg
};
P[15]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1033.812  mm,	Y =   198.569  mm,	Z =   116.437  mm,
	W =  -121.296 deg,	P =    88.677 deg,	R =   -64.029 deg
};
/END
