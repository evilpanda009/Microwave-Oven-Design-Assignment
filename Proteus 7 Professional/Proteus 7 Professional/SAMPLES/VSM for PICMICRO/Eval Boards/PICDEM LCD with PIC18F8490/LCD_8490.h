#define SEG0	LCDSE0bits.SE0
#define SEG1	LCDSE0bits.SE1
#define SEG2	LCDSE0bits.SE2
#define SEG3	LCDSE0bits.SE3
#define SEG4	LCDSE0bits.SE4
#define SEG5	LCDSE0bits.SE5
#define SEG6	LCDSE0bits.SE6
#define SEG7	LCDSE0bits.SE7

#define SEG8	LCDSE1bits.SE8
#define SEG9	LCDSE1bits.SE9
#define SEG10	LCDSE1bits.SE10
#define SEG11	LCDSE1bits.SE11
#define SEG12	LCDSE1bits.SE12
#define SEG13	LCDSE1bits.SE13
#define SEG14	LCDSE1bits.SE14
#define SEG15	LCDSE1bits.SE15

#define SEG16	LCDSE2bits.SE16
#define SEG17	LCDSE2bits.SE17
#define SEG18	LCDSE2bits.SE18
#define SEG19	LCDSE2bits.SE19
#define SEG20	LCDSE2bits.SE20
#define SEG21	LCDSE2bits.SE21
#define SEG22	LCDSE2bits.SE22
#define SEG23	LCDSE2bits.SE23

#define SEG24	LCDSE3bits.SE24
#define SEG25	LCDSE3bits.SE25
#define SEG26	LCDSE3bits.SE26
#define SEG27	LCDSE3bits.SE27
#define SEG28	LCDSE3bits.SE28
#define SEG29	LCDSE3bits.SE29
#define SEG30	LCDSE3bits.SE30
#define SEG31	LCDSE3bits.SE31

#define SEG32	LCDSE4bits.SE32
#define SEG33	LCDSE4bits.SE33
#define SEG34	LCDSE4bits.SE34
#define SEG35	LCDSE4bits.SE35
#define SEG36	LCDSE4bits.SE36
#define SEG37	LCDSE4bits.SE37
#define SEG38	LCDSE4bits.SE38
#define SEG39	LCDSE4bits.SE39

#define SEG40	LCDSE5bits.SE40
#define SEG41	LCDSE5bits.SE41
#define SEG42	LCDSE5bits.SE42
#define SEG43	LCDSE5bits.SE43
#define SEG44	LCDSE5bits.SE44
#define SEG45	LCDSE5bits.SE45
#define SEG46	LCDSE5bits.SE46
#define SEG47	LCDSE5bits.SE47

//	 LCDSE0 = 0xFF;
//	 LCDSE1 = 0xFF;
//	 LCDSE2 = 0xFF;
//	 LCDSE3 = 0xA7; //Disable Seg30, Seg28 and Seg27
//	 LCDSE4 = 0xFE; //Disable Seg32
//	 LCDSE5 = 0xFF;

#define X1	 	LCDDATA2bits.S17C0 		//C0S17
#define X2  	LCDDATA20bits.S17C3  	//C3S17
#define X3  	LCDDATA8bits.S17C1  	//C1S17
#define X4  	LCDDATA14bits.S17C2  	//C2S17
#define X5  	LCDDATA14bits.S16C2  	//C2S16
#define X6	 	LCDDATA8bits.S16C1  	//C1S16
#define X7  	LCDDATA20bits.S16C3  	//C3S16
#define X8	 	LCDDATA2bits.S16C0 		//C0S16
#define X9	 	LCDDATA1bits.S15C0 		//C0S15
#define X10 	LCDDATA19bits.S15C3  	//C3S15
#define X11 	LCDDATA7bits.S15C1  	//C1S15
#define X12 	LCDDATA13bits.S15C2  	//C2S15
#define X13 	LCDDATA13bits.S14C2  	//C2S14
#define X14 	LCDDATA7bits.S14C1  	//C1S14
#define X15 	LCDDATA19bits.S14C3  	//C3S14
#define X16 	LCDDATA1bits.S14C0 		//C0S14
#define X17 	LCDDATA1bits.S13C0 		//C0S13
#define X18 	LCDDATA19bits.S13C3  	//C3S13
#define X19 	LCDDATA7bits.S13C1  	//C1S13
#define X20 	LCDDATA13bits.S13C2  	//C2S13
#define X21 	LCDDATA13bits.S12C2  	//C2S12
#define X22 	LCDDATA7bits.S12C1  	//C1S12
#define X23 	LCDDATA0bits.S06C0 		//C0S6
#define X24 	LCDDATA18bits.S06C3  	//C3S6
#define X25 	LCDDATA6bits.S06C1  	//C1S6
#define X26		LCDDATA12bits.S06C2  	//C2S6
#define X27 	LCDDATA9bits.S27C1  	//C1S27
#define X28 	LCDDATA9bits.S28C1  	//C1S28
#define X29 	LCDDATA21bits.S28C3  	//C3S28
#define X30 	LCDDATA21bits.S27C3  	//C3S27
#define X31 	LCDDATA3bits.S26C0 		//C0S26
#define X32 	LCDDATA21bits.S26C3  	//C3S26
#define X33 	LCDDATA9bits.S26C1  	//C1S26
#define X34 	LCDDATA15bits.S26C2  	//C2S26
#define X35 	LCDDATA15bits.S27C2  	//C2S27
#define X36 	LCDDATA15bits.S28C2  	//C2S28
#define S1		LCDDATA3bits.S29C0 	    //C0S29
#define S2  	LCDDATA21bits.S29C3  	//C3S29
#define S3  	LCDDATA9bits.S29C1  	//C1S29
#define S4  	LCDDATA15bits.S29C2  	//C2S29
#define S5	 	LCDDATA3bits.S31C0 		//C0S31
#define S6	 	LCDDATA3bits.S30C0 		//C0S30
#define S7  	LCDDATA21bits.S30C3  	//C3S30
#define S8 	    LCDDATA21bits.S31C3  	//C3S31
#define S9	 	LCDDATA3bits.S24C0 		//C0S24
#define S10  	LCDDATA18bits.S00C3  	//C3S0
#define S11 	LCDDATA0bits.S01C0 		//C0S1
#define S12 	LCDDATA0bits.S00C0 		//C0S0
#define S13 	LCDDATA0bits.S05C0 		//C0S5
#define F1  	LCDDATA14bits.S18C2  	//C2S18
#define F2  	LCDDATA8bits.S18C1  	//C1S18
#define F3  	LCDDATA20bits.S18C3  	//C3S18
#define F4  	LCDDATA14bits.S19C2  	//C2S19
#define DP1 	LCDDATA13bits.S09C2  	//C2S9
#define DP2 	LCDDATA0bits.S03C0 		//C0S3
#define COL 	LCDDATA14bits.S22C2  	//C2S22
#define S1A 	LCDDATA2bits.S19C0 		//C0S19
#define S1B 	LCDDATA2bits.S20C0 		//C0S20
#define S1C 	LCDDATA8bits.S20C1  	//C1S20
#define S1D 	LCDDATA14bits.S20C2  	//C2S20
#define S1E 	LCDDATA8bits.S19C1  	//C1S19
#define S1F 	LCDDATA20bits.S19C3  	//C3S19
#define S1G 	LCDDATA20bits.S20C3  	//C3S20
#define S2A 	LCDDATA2bits.S21C0 		//C0S21
#define S2B 	LCDDATA2bits.S22C0 		//C0S22
#define S2C 	LCDDATA8bits.S22C1  	//C1S22
#define S2D 	LCDDATA14bits.S21C2  	//C2S21
#define S2E 	LCDDATA8bits.S21C1  	//C1S21
#define S2F 	LCDDATA20bits.S21C3  	//C3S21
#define S2G 	LCDDATA20bits.S22C3  	//C3S22
#define S3A 	LCDDATA2bits.S23C0 		//C0S23
#define S3B 	LCDDATA1bits.S11C0 		//C0S11
#define S3C 	LCDDATA7bits.S11C1  	//C1S11
#define S3D 	LCDDATA14bits.S23C2  	//C2S23
#define S3E 	LCDDATA8bits.S23C1  	//C1S23
#define S3F 	LCDDATA20bits.S23C3  	//C3S23
#define S3G 	LCDDATA19bits.S11C3  	//C3S11
#define S4A 	LCDDATA1bits.S10C0 		//C0S10
#define S4B 	LCDDATA1bits.S09C0 		//C0S9
#define S4C 	LCDDATA7bits.S09C1  	//C1S9
#define S4D 	LCDDATA13bits.S10C2  	//C2S10
#define S4E 	LCDDATA7bits.S10C1  	//C1S10
#define S4F 	LCDDATA19bits.S10C3  	//C3S10
#define S4G 	LCDDATA19bits.S09C3  	//C3S9
#define S5A 	LCDDATA1bits.S08C0 		//C0S8
#define S5B 	LCDDATA0bits.S07C0 		//C0S7
#define S5C 	LCDDATA6bits.S07C1  	//C1S7
#define S5D 	LCDDATA13bits.S08C2  	//C2S8
#define S5E 	LCDDATA7bits.S08C1  	//C1S8
#define S5F 	LCDDATA19bits.S08C3  	//C3S8
#define S5G 	LCDDATA18bits.S07C3  	//C3S7
#define S6A 	LCDDATA15bits.S25C2  	//C2S25
#define S6B 	LCDDATA15bits.S24C2  	//C2S24
#define S6C 	LCDDATA21bits.S24C3  	//C3S24
#define S6D 	LCDDATA3bits.S25C0 		//C0S25
#define S6E 	LCDDATA21bits.S25C3  	//C3S25
#define S6F 	LCDDATA9bits.S25C1  	//C1S25
#define S6G 	LCDDATA9bits.S24C1  	//C1S24
#define S7A		LCDDATA12bits.S02C2  	//C2S2
#define S7B		LCDDATA12bits.S03C2  	//C2S3
#define S7C 	LCDDATA18bits.S03C3  	//C3S3
#define S7D 	LCDDATA0bits.S02C0 		//C0S2
#define S7E 	LCDDATA18bits.S02C3  	//C3S2
#define S7F 	LCDDATA6bits.S02C1 		//C1S2
#define S7G 	LCDDATA6bits.S03C1 		//C1S3
#define S8A		LCDDATA12bits.S04C2  	//C2S4
#define S8B		LCDDATA12bits.S05C2  	//C2S5
#define S8C 	LCDDATA18bits.S05C3  	//C3S5
#define S8D 	LCDDATA0bits.S04C0 		//C0S4
#define S8E 	LCDDATA18bits.S04C3  	//C3S4
#define S8F 	LCDDATA6bits.S04C1  	//C1S4
#define S8G 	LCDDATA6bits.S05C1  	//C1S5
/*********************************************************************
Defines: To Turn on a segment
*********************************************************************/
#define	 X1ON	 X1=1
#define  X2ON	 X2=1	
#define  X3ON	 X3=1	
#define  X4ON	 X4=1	
#define  X5ON	 X5=1	
#define  X6ON	 X6=1	
#define  X7ON	 X7=1	
#define  X8ON	 X8=1	
#define  X9ON	 X9=1	
#define  X10ON	 X10=1	
#define  X11ON	 X11=1	
#define  X12ON	 X12=1	
#define  X13ON	 X13=1	
#define  X14ON	 X14=1	
#define  X15ON	 X15=1	
#define  X16ON	 X16=1	
#define  X17ON	 X17=1	
#define  X18ON	 X18=1	
#define  X19ON	 X19=1	
#define  X20ON	 X20=1	
#define  X21ON	 X21=1	
#define  X22ON	 X22=1	
#define  X23ON	 X23=1	
#define  X24ON	 X24=1	
#define  X25ON	 X25=1	
#define  X26ON	 X26=1	
#define  X27ON	 X27=1	
#define  X28ON	 X28=1	
#define  X29ON	 X29=1	
#define  X30ON	 X30=1	
#define  X31ON	 X31=1	
#define  X32ON	 X32=1	
#define  X33ON	 X33=1	
#define  X34ON	 X34=1	
#define  X35ON	 X35=1	
#define  X36ON	 X36=1	
#define  S1ON	 S1=1
#define  MCHPON	 S1=1	
#define  S2ON	 S2=1	
#define  S3ON	 S3=1	
#define  S4ON	 S4=1	
#define  S5ON	 S5=1	
#define  S6ON	 S6=1	
#define  S7ON	 S7=1	
#define  S8ON	 S8=1	
#define  S9ON	 S9=1	
#define  S10ON	 S10=1	
#define  S11ON	 S11=1	
#define  S12ON	 S12=1	
#define  S13ON	 S13=1
#define  VaritronixON S13=1	
#define  F1ON	 F1=1	
#define  F2ON	 F2=1	
#define  F3ON	 F3=1	
#define  F4ON	 F4=1	
#define  DP1ON	 DP1=1	
#define  DP2ON	 DP2=1	
#define  COLON	 COL=1	
#define  S1AON	 S1A=1	
#define  S1BON	 S1B=1	
#define  S1CON	 S1C=1	
#define  S1DON	 S1D=1	
#define  S1EON	 S1E=1	
#define  S1FON	 S1F=1	
#define  S1GON	 S1G=1	
#define  S2AON	 S2A=1	
#define  S2BON	 S2B=1	
#define  S2CON	 S2C=1	
#define  S2DON	 S2D=1	
#define  S2EON	 S2E=1	
#define  S2FON	 S2F=1	
#define  S2GON	 S2G=1	
#define  S3AON	 S3A=1	
#define  S3BON	 S3B=1	
#define  S3CON	 S3C=1	
#define  S3DON	 S3D=1	
#define  S3EON	 S3E=1	
#define  S3FON	 S3F=1	
#define  S3GON	 S3G=1	
#define  S4AON	 S4A=1	
#define  S4BON	 S4B=1	
#define  S4CON	 S4C=1	
#define  S4DON	 S4D=1	
#define  S4EON	 S4E=1	
#define  S4FON	 S4F=1	
#define  S4GON	 S4G=1	
#define  S5AON	 S5A=1	
#define  S5BON	 S5B=1	
#define  S5CON	 S5C=1	
#define  S5DON	 S5D=1	
#define  S5EON	 S5E=1	
#define  S5FON	 S5F=1	
#define  S5GON	 S5G=1	
#define  S6AON	 S6A=1	
#define  S6BON	 S6B=1	
#define  S6CON	 S6C=1	
#define  S6DON	 S6D=1	
#define  S6EON	 S6E=1	
#define  S6FON	 S6F=1	
#define  S6GON	 S6G=1	
#define  S7AON	 S7A=1	
#define  S7BON	 S7B=1	
#define  S7CON	 S7C=1	
#define  S7DON	 S7D=1	
#define  S7EON	 S7E=1	
#define  S7FON	 S7F=1	
#define  S7GON	 S7G=1	
#define  S8AON	 S8A=1	
#define  S8BON	 S8B=1	
#define  S8CON	 S8C=1	
#define  S8DON	 S8D=1	
#define  S8EON	 S8E=1	
#define  S8FON	 S8F=1	
#define  S8GON	 S8G=1	

//****************************************************************
// Defines to turn OFF
//****************************************************************
#define	 X1OFF	 X1=0
#define  X2OFF	 X2=0	
#define  X3OFF	 X3=0	
#define  X4OFF	 X4=0	
#define  X5OFF	 X5=0	
#define  X6OFF	 X6=0	
#define  X7OFF	 X7=0	
#define  X8OFF	 X8=0	
#define  X9OFF	 X9=0	
#define  X10OFF	 X10=0	
#define  X11OFF	 X11=0	
#define  X12OFF	 X12=0	
#define  X13OFF	 X13=0	
#define  X14OFF	 X14=0	
#define  X15OFF	 X15=0	
#define  X16OFF	 X16=0	
#define  X17OFF	 X17=0	
#define  X18OFF	 X18=0	
#define  X19OFF	 X19=0	
#define  X20OFF	 X20=0	
#define  X21OFF	 X21=0	
#define  X22OFF	 X22=0	
#define  X23OFF	 X23=0	
#define  X24OFF	 X24=0	
#define  X25OFF	 X25=0	
#define  X26OFF	 X26=0	
#define  X27OFF	 X27=0	
#define  X28OFF	 X28=0	
#define  X29OFF	 X29=0	
#define  X30OFF	 X30=0	
#define  X31OFF	 X31=0	
#define  X32OFF	 X32=0	
#define  X33OFF	 X33=0	
#define  X34OFF	 X34=0	
#define  X35OFF	 X35=0	
#define  X36OFF	 X36=0	
#define  S1OFF	 S1=0	
#define  MCHPOFF S1=0
#define  S2OFF	 S2=0	
#define  S3OFF	 S3=0	
#define  S4OFF	 S4=0	
#define  S5OFF	 S5=0	
#define  S6OFF	 S6=0	
#define  S7OFF	 S7=0	
#define  S8OFF	 S8=0	
#define  S9OFF	 S9=0	
#define  S10OFF	 S10=0	
#define  S11OFF	 S11=0	
#define  S12OFF	 S12=0	
#define  S13OFF	 S13=0	
#define  VaritronixOFF S13=0
#define  F1OFF	 F1=0	
#define  F2OFF	 F2=0	
#define  F3OFF	 F3=0	
#define  F4OFF	 F4=0	
#define  DP1OFF	 DP1=0	
#define  DP2OFF	 DP2=0	
#define  COLOFF	 COL=0	
#define  S1AOFF	 S1A=0	
#define  S1BOFF	 S1B=0	
#define  S1COFF	 S1C=0	
#define  S1DOFF	 S1D=0	
#define  S1EOFF	 S1E=0	
#define  S1FOFF	 S1F=0	
#define  S1GOFF	 S1G=0	
#define  S2AOFF	 S2A=0	
#define  S2BOFF	 S2B=0	
#define  S2COFF	 S2C=0	
#define  S2DOFF	 S2D=0	
#define  S2EOFF	 S2E=0	
#define  S2FOFF	 S2F=0	
#define  S2GOFF	 S2G=0	
#define  S3AOFF	 S3A=0	
#define  S3BOFF	 S3B=0	
#define  S3COFF	 S3C=0	
#define  S3DOFF	 S3D=0	
#define  S3EOFF	 S3E=0	
#define  S3FOFF	 S3F=0	
#define  S3GOFF	 S3G=0	
#define  S4AOFF	 S4A=0	
#define  S4BOFF	 S4B=0	
#define  S4COFF	 S4C=0	
#define  S4DOFF	 S4D=0	
#define  S4EOFF	 S4E=0	
#define  S4FOFF	 S4F=0	
#define  S4GOFF	 S4G=0	
#define  S5AOFF	 S5A=0	
#define  S5BOFF	 S5B=0	
#define  S5COFF	 S5C=0	
#define  S5DOFF	 S5D=0	
#define  S5EOFF	 S5E=0	
#define  S5FOFF	 S5F=0	
#define  S5GOFF	 S5G=0	
#define  S6AOFF	 S6A=0	
#define  S6BOFF	 S6B=0	
#define  S6COFF	 S6C=0	
#define  S6DOFF	 S6D=0	
#define  S6EOFF	 S6E=0	
#define  S6FOFF	 S6F=0	
#define  S6GOFF	 S6G=0	
#define  S7AOFF	 S7A=0	
#define  S7BOFF	 S7B=0	
#define  S7COFF	 S7C=0	
#define  S7DOFF	 S7D=0	
#define  S7EOFF	 S7E=0	
#define  S7FOFF	 S7F=0	
#define  S7GOFF	 S7G=0	
#define  S8AOFF	 S8A=0	
#define  S8BOFF	 S8B=0	
#define  S8COFF	 S8C=0	
#define  S8DOFF	 S8D=0	
#define  S8EOFF	 S8E=0	
#define  S8FOFF	 S8F=0	
#define  S8GOFF	 S8G=0	


#define BGOFF	X1OFF; X2OFF; X3OFF; X4OFF; X5OFF; \
				X6OFF; X7OFF; X8OFF; X9OFF; X10OFF;\
				X11OFF;X12OFF;X13OFF;X14OFF;X15OFF;\
				X16OFF;X17OFF;X18OFF;X19OFF;X20OFF;

#define BGON    X1ON; X2ON; X3ON; X4ON; X5ON; \
				X6ON; X7ON; X8ON; X9ON; X10ON;\
				X11ON;X12ON;X13ON;X14ON;X15ON;\
				X16ON;X17ON;X18ON;X19ON;X20ON;

#define S1NumON  S1AON; S1BON; S1CON; S1DON;\
				 S1EON; S1FON; S1GON;
#define S1NumOFF S1AOFF; S1BOFF; S1COFF; S1DOFF;\
				 S1EOFF; S1FOFF; S1GOFF;

#define S2NumON  S2AON; S2BON; S2CON; S2DON;\
				 S2EON; S2FON; S2GON;
#define S2NumOFF S2AOFF; S2BOFF; S2COFF; S2DOFF;\
				 S2EOFF; S2FOFF; S2GOFF;

#define S3NumON  S3AON; S3BON; S3CON; S3DON;\
				 S3EON; S3FON; S3GON;
#define S3NumOFF S3AOFF; S3BOFF; S3COFF; S3DOFF;\
				 S3EOFF; S3FOFF; S3GOFF;

#define S4NumON  S4AON; S4BON; S4CON; S4DON;\
				 S4EON; S4FON; S4GON;
#define S4NumOFF S4AOFF; S4BOFF; S4COFF; S4DOFF;\
				 S4EOFF; S4FOFF; S4GOFF;

#define S5NumON  S5AON; S5BON; S5CON; S5DON;\
				 S5EON; S5FON; S5GON;
#define S5NumOFF S5AOFF; S5BOFF; S5COFF; S5DOFF;\
				 S5EOFF; S5FOFF; S5GOFF;

#define S6NumON  S6AON; S6BON; S6CON; S6DON;\
				 S6EON; S6FON; S6GON;
#define S6NumOFF S6AOFF; S6BOFF; S6COFF; S6DOFF;\
				 S6EOFF; S6FOFF; S6GOFF;

#define S7NumON  S7AON; S7BON; S7CON; S7DON;\
				 S7EON; S7FON; S7GON;
#define S7NumOFF S7AOFF; S7BOFF; S7COFF; S7DOFF;\
				 S7EOFF; S7FOFF; S7GOFF;

#define S8NumON  S8AON; S8BON; S8CON; S8DON;\
				 S8EON; S8FON; S8GON;
#define S8NumOFF S8AOFF; S8BOFF; S8COFF; S8DOFF;\
				 S8EOFF; S8FOFF; S8GOFF;

#define DegCOFF	 X25OFF;
#define DegCON	 X25ON;
#define DegFOFF	 X26OFF;
#define DegFON	 X26ON;

#define ThermON 	 S5ON; S6ON; S7ON; S8ON; X25ON; X26ON;
#define ThermOFF 	 S5OFF; S6OFF; S7OFF; S8OFF; X25OFF; X26OFF;

#define TrianglesON  X21ON; X22ON; X23ON; X24ON;
#define TrianglesOFF X21OFF; X22OFF; X23OFF; X24OFF;

#define BatteryON	 S9ON; S10ON; S11ON; S12ON;
#define BatteryOFF	 S9OFF; S10OFF; S11OFF; S12OFF;

#define CGON	 X27ON; X28ON; X29ON; X30ON; X31ON;\
				 X32ON; X33ON; X34ON; X35ON; X36ON;
#define CGOFF	 X27OFF; X28OFF; X29OFF; X30OFF; X31OFF;\
				 X32OFF; X33OFF; X34OFF; X35OFF; X36OFF;

#define ArrowsON  S2ON; S3ON;
#define ArrowsOFF S2OFF; S3OFF;

#define DisplayON	BGON;\
					S1NumON; S2NumON; S3NumON; S4NumON;\
					S5NumON; S6NumON; S7NumON; S8NumON;\
					CGON;\
					DegCON;\
					DegFON;\
					MCHPON;\
					ArrowsON;\
					ThermON;\
					BatteryON;\
					TrianglesON;\
					F1ON;F2ON;F3ON;F4ON;\
					VaritronixON;\
					COLON;DP1ON;DP2ON;S4ON;
