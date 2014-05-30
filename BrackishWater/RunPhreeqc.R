#
# Runs PHREEQC speciation calculations on Brackish Water database
# Data frame bw is read from csv file
# Data frame df contains the selected output
# Data can be merged based on unique_id
# Uses packages phreeqc, data.table
# Package psych is useful for summary stats (describe)
# Timing: ~25 min to run 90,000 solutions
#

######################
# libraries
library(data.table)
library(phreeqc)
library(psych)  # optional


######################
# load database
phrLoadDatabaseString(phreeqc.dat)

######################
# add selected_output definition
fSelOut <- function(vec) {
    return (
        c(
            vec,
            'SELECTED_OUTPUT 1                                         ',
            '    -file                 selected_output_1.sel           ',
            '    -reset                false                           ',
            '    -solution             true                            ',
            '    -pH                   true                            ',
            '    -temperature          true                            ',
            '    -alkalinity           true                            ',
            '    -charge_balance       true                            ',
            '    -percent_error        true                            ',
            '    -totals               C(4)  Ca  K  Mg  Na  Si  Cl S(6) ',
            '    -molalities           CO2                             ',
            '    -saturation_indices   CO2(g)  Calcite  Gypsum         ', 
            '                          Anhydrite Chalcedony            ',
            'USER_PUNCH                                                ',
            '-heading density_calc sc_25C_calc unique_id tds_mg_L_calc soln_vol_calc kg_H2O_calc kg_soln_calc',
            '10 PUNCH RHO, SC/(1 + 0.021 * (TC - 25))                  ',
            '20 PUNCH trim(Description)                                ',
            '30 kgs = SOLN_VOL * RHO                                   ',
            '40 grams = (kgs - TOT("water"))*1000                      ',
            '100 PUNCH 1000*grams/SOLN_VOL                             ',
            '110 PUNCH SOLN_VOL, TOT("water"), kgs                     '
            )
        )
}
input <- vector()
input <- fSelOut(input)
phrRunString(input)

######################
# pull in data
bw <- read.csv('C:/BrackishWater/BrackishGW_majorIon_Etc_July26_2013.csv')
#bw <- read.csv('C:/BrackishWater/test.csv')
bw <- bw[,c("unique_id",
             "TEMP",
             "ph_fld",
             "ANC",
             "Ca_flt",
             "K_flt",
             "Mg_flt",
             "NA_flt",
             "Si_flt",
             "CL_flt",
             "SO4_flt",
             "SPCON_fld",
             "TDS")]
             
######################
# Censor data              
# pH/alkalinity
bw <- subset(bw, bw$ph_fld < 14.0)
bw <- subset(bw, bw$ph_fld > 0.0) 
bw$ANC[bw$ANC < 0.0] <- 0
bw <- subset(bw, ! (bw$ph_fld < 4.0 & bw$ANC > 0))
bw <- subset(bw, bw$ANC < 10000.0)
# default T
bw$TEMP[is.na(bw$TEMP)] <- 15.0  
# Si
bw$Si_flt[is.na(bw$Si_flt)] <- 0
# remove NA
bw <- na.omit(bw)

######################
bw$rho <- 1 + 0.03476 * bw$CL_flt / (35.453 * 1000.0) + 0.13593 * bw$SO4_flt / (64.064 * 1000.0)
# Format input for PHREEQC
bw$input <- paste('SOLUTION    ', rownames(bw), bw$unique_id, ';', 
            '  units mg/L;',
            '  density   ', bw$rho,    ';',
            '  temp      ', bw$TEMP,   ';',
            '  pH        ', bw$ph_fld, ';',
            '  Alkalinity', bw$ANC,    ';',
            '  Ca        ', bw$Ca,     ';',
            '  K         ', bw$K_flt,  ';',
            '  Mg        ', bw$Mg_flt, ';',
            '  Na        ', bw$NA_flt, ';',
            '  Si        ', bw$Si_flt, ';',
            '  Cl        ', bw$CL_flt, ';',
            '  S(6)      ', bw$SO4_flt,';')

######################
# Run speciation, check for errors, store result in a list (so)
so <- list(length = nrow(bw))
for (i in 1:nrow(bw)) 
{
	out <- tryCatch(
		{
			phrRunString(bw[i, "input"])
		},
		error=function(cond){
			message(paste("SOLUTION calculation FAILED for number ", i))
			message(paste(bw[i, "input"]))
		},
		finally={
			if (i%%1000 == 0) {
				message(paste("Through SOLUTION number ", i))
			}
			so[[i]] <- phrGetSelectedOutput()$n1
		}
	)
}

######################
# Convert selected-output list to a single data frame
df = rbindlist(so)

######################
# Merge columns
df$unique_id <- sub("\\s+$", "", df$unique_id)
add <- bw[c("SPCON_fld","TDS","unique_id")]
df = merge(df, add, by="unique_id", all.x=TRUE, all.y=FALSE)
plot(df$SPCON_fld,df$sc_25C_calc, xlim=c(0,2500),ylim=c(0,2500))


write.csv(df, "c:/BrackishWater/phreeqc_bw.July26_2013.csv")

#################
# obsolete code #
#################

# BrackishGW_majorIon_Etc_July26_2013.csv

# Create Solution input and append to input vector
#!fSoln <- function(vec, soln, title, T, pH, Alk, Ca, K, Mg, Na, Si, Cl, SO4) {
#!    if (is.na(T))     T = 25.0
#!    if (is.na(pH))   pH = 7.0
#!    if (is.na(Alk)) Alk = 0.0
#!    if (is.na(Ca))   Ca = 0.0
#!    if (is.na(K))     K = 0.0
#!    if (is.na(Mg))   Mg = 0.0
#!    if (is.na(Na))   Na = 0.0
#!    if (is.na(Si))   Si = 0.0
#!    if (is.na(Cl))   Cl = 0.0
#!    if (is.na(SO4)) SO4 = 0.0
#!    
#!    return(
#!        c(
#!            vec,
#!            paste('SOLUTION    ', soln, title   ),
#!            paste('  units mg/L'                ),
#!            paste('  temp      ', T             ),
#!            paste('  pH        ', pH            ),
#!            paste('  units     ', 'mg/L'        ),
#!            paste('  Alkalinity', Alk           ),
#!            paste('  Ca        ', Ca            ),
#!            paste('  K         ', K             ),
#!            paste('  Mg        ', Mg            ),
#!            paste('  Na        ', Na            ),
#!            paste('  Si        ', Si            ),
#!            paste('  Cl        ', Cl            ),
#!            paste('  S(6)      ', SO4           )
#!            )
#!        )
#!}

# for each solution
#!for (i in 1:length(bw[,1])) {
#!    input <- fSoln(input,
#!                   soln  <- i,
#!                   title <- bw[i, "unique_id"],
#!                   T     <- bw[i, "TEMP"],
#!                   pH    <- bw[i, "ph_fld"],
#!                   Alk   <- bw[i, "ANC"],
#!                   Ca    <- bw[i, "Ca_flt"],
#!                   K     <- bw[i, "K_flt"],
#!                   Mg    <- bw[i, "Mg_flt"],
#!                   Na    <- bw[i, "NA_flt"],
#!                   Si    <- bw[i, "Si_flt"],
#!                   Cl    <- bw[i, "CL_flt"],
#!                   SO4   <- bw[i, "SO4_flt"])
#!}


#!bw <- subset(bw, ((! is.na(bw$ph_fld)) &&
#!                  (! is.na(bw$ANC))    &&
#!                  (! is.na(bw$Ca_flt)) &&
#!                  (! is.na(bw$K_flt))  &&
#!                  (! is.na(bw$Mg_flt)) &&
#!                  (! is.na(bw$NA_flt)) &&
#!                  (! is.na(bw$Si_flt)) &&
#!                  (! is.na(bw$CL_flt)) &&
#!                  (! is.na(bw$SO4_flt)) 
#!                 )
#!            )

#!bw$input <- c(input, paste('SOLUTION    ', rownames(bw), bw$unique_id, ';', 
#!            '  units mg/L;',
#!            '  temp      ', bw$TEMP,          ';',
#!            '  pH        ', bw$ph_fld,        ';',
#!            '  Alkalinity', bw$ANC,           ';',
#!            '  Ca        ', bw$Ca,            ';',
#!            '  K         ', bw$K_flt,         ';',
#!            '  Mg        ', bw$Mg_flt,        ';',
#!            '  Na        ', bw$NA_flt,        ';',
#!            '  Si        ', bw$Si_flt,        ';',
#!            '  Cl        ', bw$CL_flt,        ';',
#!            '  S(6)      ', bw$SO4_flt,       ';')
#!            )
remove(input)
input <- vector()
input <- c(input, paste('SOLUTION    ', rownames(bw), bw$unique_id, ';', 
            '  units mg/L;',
            '  temp      ', bw$TEMP,          ';',
            '  pH        ', bw$ph_fld,        ';',
            '  Alkalinity', bw$ANC,           ';',
            '  Ca        ', bw$Ca,            ';',
            '  K         ', bw$K_flt,         ';',
            '  Mg        ', bw$Mg_flt,        ';',
            '  Na        ', bw$NA_flt,        ';',
            '  Si        ', bw$Si_flt,        ';',
            '  Cl        ', bw$CL_flt,        ';',
            '  S(6)      ', bw$SO4_flt,       ';')
            )
phrRunString(input)

# get results
selout <- phrGetSelectedOutput()

df = selout$n1

plot(df$tds_calc,df$si_Chalcedony, xlim=c(0,2))


# turn on selected_output file
phrSetSelectedOutputFileOn(1, TRUE)



 bw <- subset(bw, ! ((10.0 ^ (-14.0 + bw$ph_fld) + Si_flt / 96 / 1000) > (bw$ANC / 50.0) / 1000.0))
  

 
 USGS-363727120340101
 
 df = do.call("rbind", so)
 
 Call:
 lm(formula = df$density_calc ~ df$Cl.mol.kgw. + df$S.6..mol.kgw.)
 
 Coefficients:
      (Intercept)    df$Cl.mol.kgw.  df$S.6..mol.kgw.  
         0.99945           0.03279           0.13892  
         
 Call:
lm(formula = df$density_calc ~ df$Cl.mol.kgw. + df$S.6..mol.kgw.)

Coefficients:
     (Intercept)    df$Cl.mol.kgw.  df$S.6..mol.kgw.  
         0.99944           0.03476           0.13593  
         
 rho = 1 + 0.03279 * bw$CL_flt / (35.453 * 1000.0) + 0.13892 * bw$SO4_flt / (64.064 * 1000.0)
 
'  -density  ',1 + 0.03279 * bw$CL_flt / (35.453 * 1000.0) + 0.13892 * bw$SO4_flt / (64.064 * 1000.0), ';',

Call:
lm(formula = df$density_calc ~ df$mg_L_Cl + df$mg_L_SO4)

Coefficients:
(Intercept)   df$mg_L_Cl  df$mg_L_SO4  
   0.999428     0.001362     0.012683  
is.


df$unique_id <- sub("\\s+$", "", df$unique_id)

CT = C25 [1 + 0.021 (T - 25)]