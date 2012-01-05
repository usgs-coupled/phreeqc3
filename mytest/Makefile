CFG1 :=`uname`
CFG :=$(shell echo $(CFG1) | sed "s/CYGWIN.*/CYGWIN/")

PHREEQC=../src/Class_release/phreeqcpp
#PHREEQC=../src/Class_debug/phreeqcpp
ifeq ($(CFG), CYGWIN)
   PHREEQC=/cygdrive/c/Programs/phreeqcpp-trunk/ErrorHandling/Class_release/phreeqcpp.exe
endif

%.out : % $(PHREEQC)  ../database/phreeqc.dat ../database/wateq4f.dat ../database/llnl.dat ../database/minteq.dat ../database/minteq.v4.dat ../database/iso.dat mahoney.dat ../database/pitzer.dat
	time $(PHREEQC) $< 
	mv phreeqc.log $<.log
#	gprof $(PHREEQC) > $<.gprof

all: alkalinity.out \
	a_e_t2.out \
	aj1.out \
	all_llnl.out \
	aquia.out \
	as.ex.out \
	asf.out \
	ball.out \
	basic_test.out \
	brown.out \
	calc_SC.out \
	calc_values.out \
	cdmus3.out \
	cdmusic_related.out \
	cd_ddl.out \
	cell_operations.out \
	co2.out \
	colloid_U.out \
	comments.out \
	count_database_species.out \
	cumulativetime.out \
	ddl_flux.out \
	ddlsurf.out \
	de.out \
	deb1.out \
	deb2.out \
	debye1.out \
	desc.out \
	dissolve_only.out \
	donly.out \
	dump.out \
	eco.out \
	eq_EC.out \
	evap.out \
	exch_related.out \
	exch_related_pz.out \
	exch_kin_related.out \
	exch_kin_related_pz.out \
	ex_ild.out \
	ferri_surface.out \
	fiona.out \
	fixed_volume_gas.out \
	fixed_volume_gas_pz.out \
	fluorite.out \
	gas.out \
	hard.tests.out \
	high_mu.out \
	homo.out \
	i2.out \
	i3.out \
	i4.out \
	i5.out \
	i6.out \
	i7.out \
	include.out \
	initial_gas.out \
	Int_vs_Don.out \
	iso_example.out \
	iso10.out \
	jgb2col.out \
	jgbcol.out \
	joe.out \
	kinex.out \
	kinsurf.out \
	krest.out \
	lam7.out \
	lam10.out \
	lassin.out \
	llnl.out \
	local_min_pz.out \
	mahoney.out \
	membrane.out \
	mgcl2mgso4_pz.out \
	millero.out \
	mmb.out \
	mmb2.out \
	modifyact.out \
	mu.out \
	n3.out \
	netpathxl.out \
	negexp_totmol.out \
	np.test.out \
	opa_col2.out \
	org_gas.out \
	oxygen.out \
	phantom_surface.out \
	phase_formula.out \
	phreeqc.test.out \
	phrqpitz.tst.out \
	pitzalphas.out \
	pitzer_fail.out \
	pitzgas.out \
	precipitate_only.out \
	prob8.out \
	pyrite_34s.out \
	rahn.out \
	reaction_p.out \
	run_cells.out \
	s1.out \
	salt_ec.out \
	samemodel.out \
	saver.out \
	saunders.out \
	sit_redox.out \
	solid_solution.out \
	solid_solution_pz.out \
	solution_s.out \
	spread_solution.out \
	sc1.out \
	sc2.out \
	sc3.out \
	sc4.out \
	sc5.out \
	sc6.out \
	sc6a.out \
	sc6b.out \
	sc7.out \
	stag_diss_only.out \
	surf3.out \
	surf3_density.out \
	surf_related.out \
	surf_related_pz.out \
	surf_kin_related.out \
	surf_kin_related_pz.out \
	surf_transport.out \
	ss_r.out \
	sys_ex.out \
	t922.out \
	test_sit.out \
	transport.out \
	transport.dl.out \
	trim.out \
	user_print.out \
	using.out \
	zn.test.out \
	ex1.out \
	ex2.out \
	ex3.out \
	ex4.out \
	ex5.out \
	ex6.out \
	ex7.out \
	ex8.out \
	ex8_tail_biter.out \
	ex9.out \
	ex10.out \
	ex10_pz.out \
	ex11.out \
	ex12.out \
	ex20_debug.out \
	ex6.cvode.out ex9.cvode.out ex15.cvode.out \
	colt1.out \
	colt2.out \
	colt3.out \
	colt3_mi.out \
	colt4.out \
	colt5.out \
	minteq.out \
	minteqv4.out \
	ex16mp.out ex17mp.out ex18mp.out \
	feb.out febmp.out \
	katz.out \
	np.testmp.out \
	oxygenmp.out \
	spread_redox.out \
	raw.out \
	ppdump.out \
	errorinp.out \
	cc_P.out \
	CO2_conc_PR_EQ.out \
	CO2_conc_PR_GP.out \
	CO2_conc_PR_IS.out \
	fix_P.out \
	fix_V.out \
	gas_fix_P.out \
	gas_fix_V.out \
	NaCl.out \
	gypsum_P.out \
	P_Vm.out \
	phi_Duan_PR.out \
	phi_Duan_PR_EQ.out \
	PR_EQ.out \
	PR_EQ_GP.out \
	PR_equil.out \
	PR_equil2.out \
	PR_initial_gas.out \
	PR_initial_gas_noredox.out \
	PR_mx_new.out \
	CO2_anal.out \
	gypsum_P \
	gypsum_P2 
# NaX.out

clean:
	rm -f *.out *.log *.pun *.prn *.flat *.sel *.dmp *.lon *.pat *.mod *.txt *.csv

revert:
	svn st | egrep ^! | cut -b 2- | xargs svn revert

diff:
	svn diff --diff-cmd diff -x -bw 

ndiff:
	svn diff --diff-cmd /home/dlpark/bin/ndiff -x "--relative-error 1e-7"

zero:
	for FILE in *.out *.sel *.pun *.prn *.flat *.sel *.dmp; \
		do \
			if [ $$FILE != make.out ]; then \
				echo $$FILE; \
				./zero.sed $$FILE; \
			fi \
		done; 
diff_phreeqc:
	for FILE in *.out *.pun *.prn *.flat *.sel *.dmp *.lon *.pat *.mod *.txt; \
		do \
			if [ $$FILE != make.out ]; then \
				echo $$FILE =-=-=-=-=-; \
				diff -bw $$FILE ../../phreeqc/mytest; \
			fi \
		done; 	
