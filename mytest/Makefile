# %.o : %.c
#             $(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
PROGRAM=/cygdrive/c/Programs/phreeqcpp/Release/phreeqcpp.exe
PROGRAM=../src/Release/phreeqcpp

%.out : % $(PROGRAM)  ../database/phreeqc.dat ../database/wateq4f.dat ../database/llnl.dat ../database/minteq.dat ../database/minteq.v4.dat ../database/iso.dat mahoney.dat tony.dat ../database/pitzer.dat
	$(PROGRAM) $< 
	mv phreeqc.log $<.log

all: alkalinity.out \
	aj1.out \
	all_llnl.out \
	as.ex.out \
	ball.out \
	brown.out \
	cdmusic_related.out \
	co2.out \
	colloid_U.out \
	comments.out \
	desc.out \
	dissolve_only.out \
	eco.out \
	evap.out \
	deb1.out \
	deb2.out \
	debye1.out \
	exch_related.out \
	exch_related_pz.out \
	exch_kin_related.out \
	exch_kin_related_pz.out \
	fiona.out \
	fixed_volume_gas.out \
	fixed_volume_gas_pz.out \
	fluorite.out \
	gas.out \
	hard.tests.out \
	homo.out \
	i2.out \
	i3.out \
	i4.out \
	i5.out \
	i6.out \
	i7.out \
	Int_vs_Don.out \
	iso_example.out \
	jgb2col.out \
	jgbcol.out \
	joe.out \
	krest.out \
	lam7.out \
	lam10.out \
	lassin.out \
	llnl.out \
	local_min_pz.out \
	mahoney.out \
	mu.out \
	np.test.out \
	opa_col2.out \
	org_gas.out \
	oxygen.out \
	phantom_surface.out \
	phreeqc.test.out \
	phrqpitz.tst.out \
	pitzalphas.out \
	prob8.out \
	s1.out \
	saver.out \
	saunders.out \
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
	surf3.out \
	surf3_density.out \
	surf_related.out \
	surf_related_pz.out \
	surf_kin_related.out \
	surf_kin_related_pz.out \
	ss_r.out \
	t922.out \
	transport.out \
	transport.dl.out \
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
	ex9.out \
	ex10.out \
	ex10_pz.out \
	ex11.out \
	ex12.out \
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
	spread_redox.out 

clean:
	rm -f *.out *.log *.pun *.prn *.flat *.sel *.dmp

revert:
	svn st | egrep ^! | cut -b 2- | xargs svn revert

diff:
	svn diff --diff-cmd diff -x -bw 

zero:
	for FILE in *.out *.sel *.pun *.prn *.flat *.sel *.dmp; \
		do \
			if [ $$FILE != make.out ]; then \
				echo $$FILE; \
				./zero.sed $$FILE; \
			fi \
		done; 
