SPICE LIBRARY  ,  ,         TS95X               �%  YTsH�C                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  5  .SUBCKT TS95X 1 2 3 4 5 
*
.MODEL MDTH D IS=1E-8 KF=2.664234E-16 CJO=10F
*
* INPUT STAGE
CIP 2 5 1.000000E-12
CIN 1 5 1.000000E-12
EIP 10 5 2 5 1
EIN 16 5 1 5 1
RIP 10 11 8.125000E+00
RIN 15 16 8.125000E+00
RIS 11 15 2.238465E+02
DIP 11 12 MDTH 400E-12
DIN 15 14 MDTH 400E-12
VOFP 12 13 DC 153.5U
VOFN 13 14 DC 0
IPOL 13 5 3.200000E-05
CPS 11 15 1E-9
DINN 17 13 MDTH 400E-12
VIN 17 5 -0.100000E+00
DINR 15 18 MDTH 400E-12
VIP 4 18 0.400000E+00
FCP 4 5 VOFP 1.865000E+02
FCN 5 4 VOFN 1.865000E+02
FIBP 2 5 VOFP 6.250000E-03
FIBN 5 1 VOFN 6.250000E-03
* GM1 STAGE ***************
FGM1P 119 5 VOFP 1.1
FGM1N 119 5 VOFN 1.1
RAP 119 4 2.6E+06
RAN 119 5 2.6E+06
* GM2 STAGE *************** 
G2P 19 5 119 5 1.92E-02
G2N 19 5 119 4 1.92E-02
R2P 19 4 1E+07
R2N 19 5 1E+07
**************************
VINT1 500 0 5
GCONVP 500 501 119 4 19.38 
VP 501 0 0
GCONVN 500 502 119 5 19.38 
VN 502 0 0
********* ORIENTATION ISINK ISOURCE *******
VINT2 503 0 5
FCOPY 503 504 VOUT 1
DCOPYP 504 505 MDTH 400E-9
VCOPYP 505 0 0
DCOPYN 506 504 MDTH 400E-9
VCOPYN 0 506 0
***************************
F2PP 19 5 POLY(2) VCOPYP VP 0 0 0 0 0.5 
F2PN 19 5 POLY(2) VCOPYP VN 0 0 0 0 0.5 
F2NP 19 5 POLY(2) VCOPYN VP 0 0 0 0 1.75 
F2NN 19 5 POLY(2) VCOPYN VN 0 0 0 0 1.75 
* COMPENSATION ************
CC 19 119 25P
* OUTPUT ***********
DOPM 19 22 MDTH 400E-12
DONM 21 19 MDTH 400E-12
HOPM 22 28 VOUT 6.250000E+02
VIPM 28 4 5.000000E+01
HONM 21 27 VOUT 6.250000E+02
VINM 5 27 5.000000E+01
VOUT 3 23 0
ROUT 23 19 6
COUT 3 5 1.300000E-10
DOP 19 25 MDTH 400E-12
VOP 4 25 1.052
DON 24 19 MDTH 400E-12
VON 24 5 1.052
.ENDS 
 