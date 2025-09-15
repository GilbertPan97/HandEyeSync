/PROG  CALIB	  Macro
/ATTR
OWNER		= MNEDITOR;
COMMENT		= "";
PROG_SIZE	= 2471;
CREATE		= DATE 25-08-21  TIME 16:56:02;
MODIFIED	= DATE 25-09-11  TIME 11:30:28;
FILE_NAME	= ;
VERSION		= 0;
LINE_COUNT	= 84;
MEMORY_SIZE	= 2771;
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
   3:  OVERRIDE=60% ;
   4:   ;
   5:J P[1] 30% FINE    ;
   6:  CALL MOVE(10,100) ;
   7:  WAIT    .50(sec) ;
   8:  CALL MOVE(0,100) ;
   9:  PAUSE ;
  10:L P[2] 500mm/sec FINE    ;
  11:  CALL MOVE(10,100) ;
  12:  WAIT   1.00(sec) ;
  13:  CALL MOVE(0,100) ;
  14:  PAUSE ;
  15:L P[3] 500mm/sec FINE    ;
  16:  CALL MOVE(10,100) ;
  17:  WAIT    .50(sec) ;
  18:  CALL MOVE(0,100) ;
  19:  PAUSE ;
  20:   ;
  21:L P[4] 500mm/sec FINE    ;
  22:  CALL MOVE(10,100) ;
  23:  WAIT    .50(sec) ;
  24:  CALL MOVE(0,100) ;
  25:  PAUSE ;
  26:L P[5] 500mm/sec FINE    ;
  27:  CALL MOVE(10,100) ;
  28:  WAIT    .50(sec) ;
  29:  CALL MOVE(0,100) ;
  30:  PAUSE ;
  31:L P[6] 500mm/sec FINE    ;
  32:  CALL MOVE(10,100) ;
  33:  WAIT    .50(sec) ;
  34:  CALL MOVE(0,100) ;
  35:  PAUSE ;
  36:   ;
  37:   ;
  38:L P[7] 500mm/sec FINE    ;
  39:  CALL MOVE(10,100) ;
  40:  WAIT    .50(sec) ;
  41:  CALL MOVE(0,100) ;
  42:  PAUSE ;
  43:L P[8] 500mm/sec FINE    ;
  44:  CALL MOVE(10,100) ;
  45:  WAIT    .50(sec) ;
  46:  CALL MOVE(0,100) ;
  47:  PAUSE ;
  48:L P[9] 500mm/sec FINE    ;
  49:  CALL MOVE(10,100) ;
  50:  WAIT    .50(sec) ;
  51:  CALL MOVE(0,100) ;
  52:  PAUSE ;
  53:   ;
  54:L P[10] 500mm/sec FINE    ;
  55:  CALL MOVE(10,100) ;
  56:  WAIT    .50(sec) ;
  57:  CALL MOVE(0,100) ;
  58:  PAUSE ;
  59:L P[11] 500mm/sec FINE    ;
  60:  CALL MOVE(10,100) ;
  61:  WAIT    .50(sec) ;
  62:  CALL MOVE(0,100) ;
  63:  PAUSE ;
  64:L P[12] 500mm/sec FINE    ;
  65:  CALL MOVE(10,100) ;
  66:  WAIT    .50(sec) ;
  67:  CALL MOVE(0,100) ;
  68:  PAUSE ;
  69:   ;
  70:L P[13] 500mm/sec FINE    ;
  71:  CALL MOVE(10,100) ;
  72:  WAIT    .50(sec) ;
  73:  CALL MOVE(0,100) ;
  74:  PAUSE ;
  75:L P[14] 500mm/sec FINE    ;
  76:  CALL MOVE(10,100) ;
  77:  WAIT    .50(sec) ;
  78:  CALL MOVE(0,100) ;
  79:  PAUSE ;
  80:L P[15] 500mm/sec FINE    ;
  81:  CALL MOVE(10,100) ;
  82:  WAIT    .50(sec) ;
  83:  CALL MOVE(0,100) ;
  84:   ;
/POS
P[1]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1098.637  mm,	Y =    -8.904  mm,	Z =   109.532  mm,
	W =  -120.543 deg,	P =    88.661 deg,	R =   -27.946 deg
};
P[2]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1087.526  mm,	Y =    -8.803  mm,	Z =   104.623  mm,
	W =  -120.705 deg,	P =    88.665 deg,	R =   -28.100 deg
};
P[3]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1077.755  mm,	Y =    -8.771  mm,	Z =    96.684  mm,
	W =  -121.180 deg,	P =    88.675 deg,	R =   -28.578 deg
};
P[4]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1087.499  mm,	Y =    -8.820  mm,	Z =    71.997  mm,
	W =  -121.365 deg,	P =    88.676 deg,	R =   -28.764 deg
};
P[5]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1087.445  mm,	Y =   -42.414  mm,	Z =    71.636  mm,
	W =  -122.332 deg,	P =    88.693 deg,	R =   -29.733 deg
};
P[6]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1087.481  mm,	Y =    20.472  mm,	Z =    73.013  mm,
	W =  -121.719 deg,	P =    88.682 deg,	R =   -29.117 deg
};
P[7]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1087.505  mm,	Y =   -89.221  mm,	Z =    59.204  mm,
	W =   -10.787 deg,	P =    81.851 deg,	R =    81.727 deg
};
P[8]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1093.470  mm,	Y =  -166.258  mm,	Z =   -27.264  mm,
	W =    -6.112 deg,	P =    71.785 deg,	R =    86.299 deg
};
P[9]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1088.407  mm,	Y =    77.575  mm,	Z =   134.649  mm,
	W =  -176.051 deg,	P =    80.036 deg,	R =   -83.358 deg
};
P[10]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =   985.635  mm,	Y =     -.650  mm,	Z =    42.037  mm,
	W =   -95.205 deg,	P =    76.308 deg,	R =    -2.670 deg
};
P[11]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =   907.614  mm,	Y =    -4.515  mm,	Z =   -17.000  mm,
	W =   -94.070 deg,	P =    66.442 deg,	R =    -1.590 deg
};
P[12]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =   861.683  mm,	Y =    -4.500  mm,	Z =   -86.779  mm,
	W =   -93.660 deg,	P =    58.579 deg,	R =    -1.224 deg
};
P[13]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1149.633  mm,	Y =   -29.919  mm,	Z =   103.261  mm,
	W =  -121.240 deg,	P =    88.679 deg,	R =   -16.950 deg
};
P[14]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =  1210.898  mm,	Y =   -49.448  mm,	Z =    99.759  mm,
	W =  -121.444 deg,	P =    88.688 deg,	R =    -5.793 deg
};
P[15]{
   GP1:
	UF : 0, UT : 10,		CONFIG : 'N U T, 0, 0, 0',
	X =   952.574  mm,	Y =   180.482  mm,	Z =   104.355  mm,
	W =  -123.131 deg,	P =    88.694 deg,	R =   -65.841 deg
};
/END
