CFG1 :=`uname`
CFG :=$(shell echo $(CFG1) | sed "s/Linux.*/Linux/")
#make -k all 1> make.out 2>&1
#TIME=time
TIME=

#VALGRIND=/home/dlpark/bin/bin/valgrind --leak-check=full --track-origins=yes
VALGRIND=

ifeq ($(CFG), Linux)
#	PHREEQC=../_debug/INSTALL/bin/phreeqcd
#	PHREEQC=../_build/INSTALL/bin/phreeqc
	PHREEQC=../src/Class_release_64/phreeqc
#	PHREEQC=../src/Class_debug_64/phreeqc
else
	PHREEQC=../_build_vs_master/Release/phreeqc.exe
endif

%.out : % $(PHREEQC)  ../database/phreeqc.dat ../database/wateq4f.dat ../database/llnl.dat ../database/minteq.dat ../database/minteq.v4.dat ../database/iso.dat mahoney.dat ../database/pitzer.dat
	$(TIME) $(VALGRIND) $(PHREEQC) $< $<.out xxx $<.log
#	mv phreeqc.log $<.log
#	gprof $(PHREEQC) > $<.gprof

all: fail succeed

succeed: \
	alkalinity.out \
	a_e_t2.out \
	aj1.out \
	all_llnl.out \
	aquia.out \
	aquia2.out \
	As.out \
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
	ch4-valid.out \
	co2.out \
	colloid_U.out \
	comments.out \
	count_database_species.out \
	cumulativetime.out \
	DanPhuong.out \
	ddl_flux.out \
	ddlsurf.out \
	de.out \
	deb1.out \
	deb2.out \
	debye1.out \
	deltas.out \
	desc.out \
	dissolve_only.out \
	donly.out \
	dump_test.out \
	eco.out \
	eq_EC.out \
	eq_phase_mod.out \
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
	Gas_FixVolume.out \
	gas_mod.out \
	gas_phase_pressure.out \
	gfw.out \
	hard.tests.out \
	homo.out \
	i2.out \
	i3.out \
	i4.out \
	i5.out \
	i6.out \
	i7.out \
	Ilulissat_diff3.out \
	is.out \
	include.out \
	initial_gas.out \
	Int_vs_Don.out \
	iso_example.out \
	iso10.out \
	jgb2col.out \
	jgbcol.out \
	joe.out \
	kin_time.out \
	kinex.out \
	kinsurf.out \
	krest.out \
	lam7.out \
	lam10.out \
	lassin.out \
	llnl.out \
	local_min_pz.out \
	low_t.out \
	mahoney.out \
	MassWater.out \
	membrane.out \
	mgcl2mgso4_pz.out \
	millero.out \
	mixes.out \
	mmb.out \
	mmb2.out \
	modifyact.out \
	modify_zero.out \
	mu.out \
	n3.out \
	netpathxl.out \
	np.test.out \
	opa_col2.out \
	org_gas.out \
	oxygen.out \
	p_dep_species.out \
	phantom_surface.out \
	phase_formula.out \
	phreeqc.test.out \
	phrqpitz.tst.out \
	pitzalphas.out \
	pitzer_redef.out \
	pitzgas.out \
	precipitate_only.out \
	pressure_si.out \
	prob8.out \
	pyrite_34s.out \
	rahn.out \
	reaction_p.out \
	run_cells.out \
	s1.out \
	salt_ec.out \
	salt_ec_chart.out \
	samemodel.out \
	saver.out \
	saunders.out \
	sit_redox.out \
	solid_solution.out \
	solid_solution_pz.out \
	solution_s.out \
	species_formula.out \
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
	surf_check.out \
	surf3_density.out \
	surf_related.out \
	surf_related_pz.out \
	surf_kin_related.out \
	surf_kin_related_pz.out \
	surf_transport.out \
	surface_punch_all.out \
	ss_r.out \
	sys_ex.out \
	sys_gas.out \
	t2.out \
	t922.out \
	test_sit.out \
	transport.out \
	transport.dl.out \
	trim.out \
	user_print.out \
	using.out \
	volume-zero.out \
	y_H2O.out \
	zn.test.out \
	zero_modify.out \
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
	ex6.cvode.out \
    ex9.cvode.out \
    ex15.cvode.out \
	colt1.out \
	colt2.out \
	colt3.out \
	colt3_mi.out \
	colt4.out \
	colt5.out \
	minteq.out \
	minteqv4.out \
	ex16mp.out \
	ex17mp.out \
	ex18mp.out \
	feb.out \
	febmp.out \
	katz.out \
	np.testmp.out \
	oxygenmp.out \
	spread_redox.out \
	ppdump.out \
	diel.out \
	H_K.out \
	Sat_P_rho.out \
	densities.out \
	CH4_conc_PR_IS.out \
	CO2_anal.out \
	CO2_conc_PR_EQ.out \
	CO2_conc_PR_GP.out \
	CO2_conc_PR_IS.out \
	CO2_K_H.out \
	CO2_4M_NaCl.out \
	CO2_Na2SO4.out \
	co2_t30.out \
	co2_tc.out \
	fix_P.out \
	fix_V.out \
	gas_fix_P.out \
	gas_fix_V.out \
	gypsum_P.out \
	gypsum_P2.out \
	N2_conc_PR_IS.out \
	N2_ideal.out \
	NaCl.out \
	O2_conc_PR_IS.out \
	P_CO2_Na2SO4.out \
	P_Vm.out \
	phi_Angus.out \
	phi_Duan_PR.out \
	phi_Duan_PR_EQ.out \
	PR_EQ.out \
	PR_EQ_GP.out \
	PR_equil.out \
	PR_equil2.out \
	PR_initial_gas.out \
	PR_initial_gas_noredox.out \
	PR_mx_new.out \
	ternary.out \
	water_vp1.out \
	Barite_MCl2.out \
	Barite_NaCl.out \
	Barite_tc.out \
	cc_P.out \
	cc_P0.out \
	cc_P2.out \
	cc_P3.out \
	evap_sea.out \
	halite.out \
	seaw_anhyd.out \
	SrSO4_P.out \
	SrSO4_tc.out \
	water_vp.out \
	Apelbl_Na2CO3.out \
	mV_SO4_rho.out \
	mV1_rho.out \
	mV2_rho.out \
	pKw.out \
	Vm_BaCl2_sol.out \
	Vm_CaCl2_sol.out \
	Vm_HCl_sol.out \
	Vm_KCl_sol.out \
	Vm_KHCO3_sol.out \
	Vm_MgCl2_sol.out \
	Vm_Na2CO3_sol.out \
	Vm_Na2SO4_sol.out \
	Vm_NaCl_sol.out \
	Vm_NaHCO3_sol.out \
	Vm_NaNO3_sol.out \
	Vm_NH4Cl_sol.out \
	Vm_SrCl2_sol.out \
	rho_CaNaCl.out \
	rho_CaKMgNaCl.out \
	co2_VP.out \
	phi_CO2-CH4_GP.out \
	rho_MgCaCl.out \
	rho_MgNaCl.out \
	Vm_salt.out \
	Vm_salt_65.out \
	Vm_salt_fig.out \
	Vm0_tc.out \
	Carnallite.out \
	K2SO4.out \
	KCl.out \
	KCl-SO4.out \
	MgCl2.out \
	MgCl-SO4.out \
	MgSO4.out \
	Na2SO4.out \
	Na2SO4_phr.out \
	NaCl_temp.out \
	NaCl-SO4.out \
	NaKCl.out \
	NaMgCl.out \
	14C_isotopes.out \
	str_f.out \
	anhy_K2SO4.out \
	anhy_Na2SO4.out \
	anhy_NaCl.out \
	gyps_NaCl.out \
	gyps_Na2SO4.out \
	gypsum.out \
	gas9.out \
	Halite_P.out \
	anh_P_NaCl.out \
	gypsum_P5.out \
	multi_punch.out \
	rho_H3PO4.out \
	soln_vol.out \
	Vm_LiCl_sol.out \
	run_cell_range.out \
	modify_ignore.out \
	connect_graph.out \
	variable_cell_diffusion.out \
	no_negative_dump.out \
	reaction_pressure_gas.out \
	pitzer_redef_2.out \
	KinSorbNp.out \
	lkspecies.out \
	Nardi.out \
	jm_trans.out \
	andsurfcomplex4.out \
	diff_c.out \
	cc_1barCO2.out \
	surf_total.out \
	peng_SO2.out \
	num_spread.out \
	sys_equi.out \
	toner.out \
	lk_phase.out \
	calcite_rate.out \
	sar.out \
	CO2_sp_IS.out \
	Hebach.out \
	erm.out \
	co2_pressure_bug.out \
	edl_species.out \
	R-04_01_PHRC.out \
	McBride_rho.out \
	Ca-As-Mahoney.out \
	SC_temp.out \
	current1.out \
	uphill_NP.out \
	SC_Ohm.out \
	adapted_minteq.out \
	sys_kin.out \
	harm_mean.out \
	unequal.out \
	phase_vm.out \
	low_p.out \
	calc_density.out \
	Ratework.out \
	salt_sc.out \
	IL_flux.out \
	SC_Ohm1.out \
	salt_sc1.out \
	McClesk.out \
	stuyfz.out \
	opa_col3.out \
	sys_equi_fix.out \
	pr_h2o.out \
	ex7_T_P.out \
	antarcticite.out \
	DonnanBug.out \
	PercentSC.out \
	ExchangeGamma.out \
	count_database_species_core10.out \
	dist_x.out \
	balonis.out \
	mmb2_pitzer.out \
	ddl_set.out \
	rotter.out \
	gas_type_switch.out \
	CalPortDiff.out \
	ss_incr.out \
	gas_phase_mix.out \
	iso_inverse.out \
	TonyLitharge2a.out \
	implicit_as.out \
	surface_mix.out \
	andra_kin_ss.out \
	ex12b.out \
	ex13_impl.out \
	Cub.out \
	ExchMixRelatedMin.out \
	ExchMixRelatedKin.out \
	SurfMixRelatedMin.out \
	SurfMixRelatedKin.out \
	MoreExchMix.out \
	notab.out \
	zeta.out \
	sys_sort.out \
	kinetic_rates.out \
	delta_h.out \
	uphill_NPa.out \
	sit_edl.out \
	Hao_wateq4fhao.out \
	TotalPlus.out \
	NaOH_density.out \
	CO2_conc_PR_GP1.out \
	H2S.out \
	H2S_71C.out \
	H2S_NaCl_Na2SO4.out \
	H2S_pz.out \
	H2S_pz_appt.out 
	
H2S_files: \
	H2S.out \
	H2S_71C.out \
	H2S_NaCl_Na2SO4.out \
	H2S_pz.out \
	H2S_pz_appt.out \
	CO2_conc_PR_GP1.out 

H2S_clean: 
	rm 	H2S.out \
	H2S_71C.out \
	H2S_NaCl_Na2SO4.out \
	H2S_pz.out \
	H2S_pz_appt.out \
	CO2_conc_PR_GP1.out 

# NaX.out
	
fail: \
	high_mu.out \
	modify_test.out \
	negexp_totmol.out \
	pitzer_fail.out \
	surf_related_warnings.out \
	raw.out \
	negative_user_number.out \
	punch_stream.out \
	CFAscmtest.out \
	missing_surf_related_equi.out \
	missing_surf_related_kin.out \
	feedwater.out \
	Syn1_acidbase.out \
	IllitisationCEC.out \
	Hao.out \
	wjAs2.out 

current: current2.out \
	Clogging.out \
	current_analytical.out

pitzer: \
	anh_P_NaCl.out \
	anhy_K2SO4.out \
	anhy_Na2SO4.out \
	anhy_NaCl.out \
	Barite_MCl2.out \
	Barite_NaCl.out \
	Carnallite.out \
	cc_1barCO2.out \
	CO2_4M_NaCl.out \
	CO2_Na2SO4.out \
	evap.out \
	evap_sea.out \
	ex10_pz.out \
	exch_kin_related_pz.out \
	exch_related_pz.out \
	fixed_volume_gas_pz.out \
	gyps_Na2SO4.out \
	gyps_NaCl.out \
	gypsum.out \
	gypsum_P2.out \
	gypsum_P5.out \
	halite.out \
	Halite_P.out \
	K2SO4.out \
	KCl.out \
	KCl-SO4.out \
	local_min_pz.out \
	MgCl-SO4.out \
	MgCl2.out \
	mgcl2mgso4_pz.out \
	MgSO4.out \
	Na2SO4.out \
	NaCl.out \
	NaCl-SO4.out \
	NaCl_temp.out \
	NaKCl.out \
	NaMgCl.out \
	fail1.out \
	fail2.out \
	fail3.out

frezchem: \
	anh_P_NaCl.out \
	anhy_K2SO4.out \
	anhy_Na2SO4.out \
	anhy_NaCl.out \
	Carnallite.out \
	evap_sea.out \
	ex10_pz.out \
	exch_kin_related_pz.out \
	exch_related_pz.out \
	fixed_volume_gas_pz.out \
	gyps_Na2SO4.out \
	gyps_NaCl.out \
	gypsum.out \
	gypsum_P2.out \
	gypsum_P5.out \
	halite.out \
	Halite_P.out \
	K2SO4.out \
	KCl-SO4.out \
	MgCl-SO4.out \
	mgcl2mgso4_pz.out \
	Na2SO4.out \
	NaCl-SO4.out \
	NaKCl.out \
	NaMgCl.out

ss: \
	basic_test.out \
	cell_operations.out \
	ex20_debug.out \
	iso_example.out \
	iso10.out \
	kin_time.out \
	mixes.out \
	modify_ignore.out \
	multi_punch.out \
	no_negative_dump.out \
	phase_formula.out \
	ppdump.out \
	pyrite_34s.out \
	solid_solution.out \
	solid_solution_pz.out \
	ss_incr.out \
	ss_r.out \
	surface_punch_all.out

clean: 
	rm -f *.out *.log *.pun *.prn *.flat *.sel *.dmp *.lon *.pat *.mod *.diff *.expected  millero.csv 

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
