pcf 
* control data 
restart estimation 
     22   90      1      0     5 
     1     1   single   point   1   0   0 
10.0  -3.0    0.3    0.03     -9  999  LAMFORGIVE DERFORGIVE 
0.2   2.0   1.0e-3 
0.1  noaui 
30   .005  4   4  .005   4 
1    1    1  
* singular value decomposition 
1 
9 5e-7 
1 
* parameter groups 
phq_k        relative      1.00000E-02  0.0000    switch     2.0000      parabolic
* parameter data 
palb		fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
pan		fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
pclinop-na	fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
pclinop   	fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
pash		fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
pclinop-k	fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
pclinop-sr	fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
poliv		fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
ppyrox		fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
pqz		fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
pchl		fixed	relative	10.0	9.0	11.0	phq_k	1.0	-10.0	1
aalb		none	relative	10.2430	5.0	15.0	phq_k	1.0	-20.0	1
aan		none	relative	9.74620	5.0	15.0	phq_k	1.0	-20.0	1
aclinop-na	none	relative	8.19206	5.0	15.0	phq_k	1.0	-20.0	1
aclinop   	fixed	relative	10.0000	5.0	15.0	phq_k	1.0	-20.0	1
aash		none	relative	10.0177	5.0	15.0	phq_k	1.0	-20.0	1
aclinop-k	none	relative	9.60000	5.0	15.0	phq_k	1.0	-20.0	1
aclinop-sr	fixed	relative	10.0000	5.0	15.0	phq_k	1.0	-20.0	1
aoliv		none	relative	9.65364	5.0	15.0	phq_k	1.0	-20.0	1
apyrox		none	relative	10.0411	5.0	15.0	phq_k	1.0	-20.0	1
aqz		none	relative	9.89100	5.0	15.0	phq_k	1.0	-20.0	1
achl		none	relative	7.86517	5.0	15.0	phq_k	1.0	-20.0	1


* observation groups 
na 
mg
k
ca
si
* observation data 
na_290		4.8520E-01	2.1732		na
na_1310		4.9030E-01	2.1732		na
na_3210		4.9060E-01	2.1732		na
na_6060		4.8940E-01	2.1732		na
na_8910		4.8420E-01	2.1732		na
na_11755	4.6700E-01	2.1732		na
na_15705	4.4990E-01	2.1732		na
na_21315	4.6930E-01	2.1732		na
na_27831	4.6140E-01	2.1732		na
na_33769	4.4680E-01	2.1732		na
na_38892	4.0990E-01	2.1732		na
na_46593	3.7320E-01	2.1732		na
na_55143	3.0230E-01	2.1732		na
na_60804	2.6280E-01	2.1732		na
na_69792	2.1640E-01	2.1732		na
na_77946	2.1730E-01	2.1732		na
na_90226	1.2830E-01	2.1732		na
na_94899	1.4500E-01	2.1732		na
mg_290		5.3190E-02	4.4529		mg
mg_1310		5.2310E-02	4.4529		mg
mg_3210		4.9470E-02	4.4529		mg
mg_6060		4.5870E-02	4.4529		mg
mg_8910		4.5420E-02	4.4529		mg
mg_11755	5.3380E-02	4.4529		mg
mg_15705	3.8700E-02	4.4529		mg
mg_21315	2.8630E-02	4.4529		mg
mg_27831	2.2230E-02	4.4529		mg
mg_33769	1.4150E-02	4.4529		mg
mg_38892	3.1250E-03	4.4529		mg
mg_46593	3.4380E-04	4.4529		mg
mg_55143	1.9200E-04	4.4529		mg
mg_60804	1.6380E-03	4.4529		mg
mg_69792	1.4750E-03	4.4529		mg
mg_77946	5.3560E-04	4.4529		mg
mg_90226	1.8270E-03	4.4529		mg
mg_94899	7.4150E-04	4.4529		mg
k_290		2.4270E-02	10.8346		k
k_1310		2.3410E-02	10.8346		k
k_3210		2.3910E-02	10.8346		k
k_6060		2.2600E-02	10.8346		k
k_8910		1.8660E-02	10.8346		k
k_11755		1.5210E-02	10.8346		k
k_15705		1.3640E-02	10.8346		k
k_21315		1.6290E-02	10.8346		k
k_27831		1.3210E-02	10.8346		k
k_33769		1.1180E-02	10.8346		k
k_38892		6.3200E-03	10.8346		k
k_46593		2.6760E-03	10.8346		k
k_55143		7.6000E-04	10.8346		k
k_60804		8.7030E-04	10.8346		k
k_69792		7.3250E-04	10.8346		k
k_77946		8.2260E-04	10.8346		k
k_90226		1.4030E-03	10.8346		k
k_94899		9.5560E-04	10.8346		k
ca_290		1.1650E-02	2.8280		ca
ca_1310		1.4270E-02	2.8280		ca
ca_3210		1.7740E-02	2.8280		ca
ca_6060		2.1770E-02	2.8280		ca
ca_8910		2.5640E-02	2.8280		ca
ca_11755	3.2470E-02	2.8280		ca
ca_15705	4.1220E-02	2.8280		ca
ca_21315	5.2570E-02	2.8280		ca
ca_27831	6.4140E-02	2.8280		ca
ca_33769	8.3000E-02	2.8280		ca
ca_38892	1.2190E-01	2.8280		ca
ca_46593	1.5260E-01	2.8280		ca
ca_55143	1.9530E-01	2.8280		ca
ca_60804	2.1540E-01	2.8280		ca
ca_69792	2.4580E-01	2.8280		ca
ca_77946	2.4710E-01	2.8280		ca
ca_90226	2.7490E-01	2.8280		ca
ca_94899	2.6270E-01	2.8280		ca
si_290		4.8910E-04	1015.6297	si
si_1310		4.4360E-04	1015.6297	si
si_3210		4.8250E-04	1015.6297	si
si_6060		7.0310E-04	1015.6297	si
si_8910		3.2610E-04	1015.6297	si
si_11755	3.3580E-04	1015.6297	si
si_15705	5.3900E-04	1015.6297	si
si_21315	6.6400E-04	1015.6297	si
si_27831	5.1890E-04	1015.6297	si
si_33769	5.6050E-04	1015.6297	si
si_38892	5.3170E-04	1015.6297	si
si_46593	2.1740E-04	1015.6297	si
si_55143	1.2100E-04	1015.6297	si
si_60804	8.7050E-05	1015.6297	si
si_69792	9.3000E-05	1015.6297	si
si_77946	1.1240E-04	1015.6297	si
si_90226	1.0560E-04	1015.6297	si
si_94899	9.3410E-05	1015.6297	si


* model command line 
@PROJECT_DIR@\01_1D.bat

* model input/output 
@PROJECT_DIR@\compaction_definitions.phr.tpl		compaction_definitions.phr
@PROJECT_DIR@\01_1D.txt.ins		01_1D.txt 
