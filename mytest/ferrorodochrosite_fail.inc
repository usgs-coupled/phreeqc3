
    -reset                false
    -distance             true
    -totals               C  Ca  Cl  Fe_di  Mn_di  Na  S
    -molalities           Fetri_sOFe_di+  Fetri_sOHCa+2  Fetri_sOH2+  Fetri_sOMn_di+
                          Fetri_sOFe_diOH  Fetri_sOH  Fetri_sOHSr+2  Fetri_sO-
                          Fetri_wOH2+  Fetri_wCO3H  Fetri_wOFe_di+  Fetri_wOFe_diOH
                          Fetri_wOH  Fetri_wCO3-  Fetri_wOMn_di+  Fetri_wO-
                          Fetri_wOCa+  Fetri_wSO4-  Fetri_wOHSO4-2  Fetri_wH2SiO4-
                          Fetri_wOH3SiO4-2  Fetri_wOSr+  Fetri_wOSrOH  CaX2
                          Fe_diX2  NaX  Mn_diX2  SrX2
                          MgX2  KX
    -saturation_indices   Sr
USER_PUNCH 1
    -headings hours Si pH Mn(2) Fe(tot) Fe(2) MnX2 FeX2 Fe(3) O2 Fe(OH)3(a) Katalsurf eqFe_goethite Ferrorhodochrosite Manganite
    -start
10 PUNCH SIM_TIME/(60*60), 28000*TOT("Si"), -LA("H+"), 55000*TOT("Mn_di"), 56000*TOT("Fe_di")+ 56000*TOT("Fe_tri"), 56000*TOT("Fe_di"), 55000*MOL("Mn_diX2"), 56000*MOL("Fe_diX2"), 56000*TOT("Fe_tri"), 32000*MOL("O2"), 107000*EQUI("Fe_tri(OH)3(a)") , 107000*EQUI("Katalsurf"),  89000*EQUI("Fe_goethite"), 231000*EQUI("Ferrorhodochrosite"), 88000*EQUI("Manganite")
    -end