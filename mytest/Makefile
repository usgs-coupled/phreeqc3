# %.o : %.c
#             $(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
PROGRAM=../bin/phreeqc

%.out : % ../bin/phreeqc ../database/phreeqc.dat ../database/wateq4f.dat ../database/llnl.dat ../database/minteq.dat ../database/minteq.v4.dat ../database/iso.dat mahoney.dat tony.dat ../database/pitzer.dat
	../bin/phreeqc $< 
	mv phreeqc.log $<.log

all: alkalinity.out \
	aj1.out \
	all_llnl.out \
	as.ex.out \
	ball.out \
	brown.out \
	co2.out \
	comments.out \
	dissolve_only.out \
	evap.out \
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
	jgb2col.out \
	jgbcol.out \
	joe.out \
	krest.out \
	lam7.out \
	lam10.out \
	llnl.out \
	local_min_pz.out \
	mahoney.out \
	mu.out \
	np.test.out \
	org_gas.out \
	oxygen.out \
	phantom_surface.out \
	phreeqc.test.out \
	phrqpitz.tst.out \
	prob8.out \
	saver.out \
	saunders.out \
	solid_solution.out \
	solid_solution_pz.out \
	solution_s.out \
	spread_solution.out \
	iso_example.out \
	surf3.out \
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
	np.testmp.out \
	oxygenmp.out \
	spread_redox.out 
#	svn diff .

clean:
	rm -f *.out *.log *.pun *.prn *.flat *.sel *.dmp
