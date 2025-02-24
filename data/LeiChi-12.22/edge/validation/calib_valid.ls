/PROG  CALIB_VALID
/ATTR
OWNER		= MNEDITOR;
COMMENT		= "";
PROG_SIZE	= 846;
CREATE		= DATE 25-01-09  TIME 16:56:42;
MODIFIED	= DATE 25-01-09  TIME 16:56:42;
FILE_NAME	= ;
VERSION		= 0;
LINE_COUNT	= 14;
MEMORY_SIZE	= 1162;
PROTECT		= READ_WRITE;
TCD:  STACK_SIZE	= 0,
      TASK_PRIORITY	= 50,
      TIME_SLICE	= 0,
      BUSY_LAMP_OFF	= 0,
      ABORT_REQUEST	= 0,
      PAUSE_REQUEST	= 0;
DEFAULT_GROUP	= 1,*,*,*,*;
CONTROL_CODE	= 00000000 00000000;
LOCAL_REGISTERS	= 0,0,0;
/MN
   1:  UFRAME_NUM=0 ;
   2:  UTOOL_NUM=0 ;
   3:  OVERRIDE=20% ;
   4:J P[1] 3% FINE    ;
   5:J P[2] 3% FINE    ;
   6:J P[3] 3% FINE    ;
   7:   ;
   8:  PAUSE ;
   9:  UFRAME_NUM=0 ;
  10:  UTOOL_NUM=0 ;
  11:  OVERRIDE=20% ;
  12:L P[4] 100mm/sec CNT100    ;
  13:L P[5] 100mm/sec CNT100    ;
  14:L P[6] 100mm/sec CNT100    ;
/POS
P[1]{
   GP1:
	UF : 0, UT : 0,		CONFIG : 'N U T, 0, 0, 0',
	X =  -952.629  mm,	Y =  1137.671  mm,	Z =   -85.292  mm,
	W =  -172.743 deg,	P =   -22.054 deg,	R =    -4.176 deg
};
P[2]{
   GP1:
	UF : 0, UT : 0,		CONFIG : 'N U T, 0, 0, 0',
	X =  -742.195  mm,	Y =  1151.130  mm,	Z =    46.004  mm,
	W =  -176.572 deg,	P =   -21.792 deg,	R =     2.923 deg
};
P[3]{
   GP1:
	UF : 0, UT : 0,		CONFIG : 'N U T, 0, 0, 0',
	X =  -674.694  mm,	Y =  1097.218  mm,	Z =  -154.098  mm,
	W =  -170.978 deg,	P =   -21.794 deg,	R =     2.886 deg
};
P[4]{
   GP1:
	UF : 0, UT : 0,		CONFIG : 'N U T, 0, 0, 0',
	X =  -439.605  mm,	Y =  2460.641  mm,	Z =   235.074  mm,
	W =   145.578 deg,	P =    -6.192 deg,	R =    88.040 deg
};
P[5]{
   GP1:
	UF : 0, UT : 0,		CONFIG : 'N U T, 0, 0, 0',
	X =  -518.691  mm,	Y =  2335.694  mm,	Z =   124.200  mm,
	W =   145.990 deg,	P =   -14.113 deg,	R =    82.314 deg
};
P[6]{
   GP1:
	UF : 0, UT : 0,		CONFIG : 'N U T, 0, 0, 0',
	X =  -321.927  mm,	Y =  2455.880  mm,	Z =   199.809  mm,
	W =   152.804 deg,	P =    -6.553 deg,	R =    88.298 deg
};
/END
