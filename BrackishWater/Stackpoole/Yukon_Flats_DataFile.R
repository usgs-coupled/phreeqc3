# Create Solution input and append to input vector
fSoln <- function(vec, soln, title, T, pH, Alk, Ca, K, Mg, NO3, NH4, Na, SO4, P) {

    if (is.na(T))     T = 25.0
    if (is.na(pH))   pH = 7.0
    if (is.na(Alk)) Alk = 0.0
    
    if (is.na(Ca))   Ca = 0.0
    if (is.na(K))     K = 0.0
    if (is.na(Mg))   Mg = 0.0
    if (is.na(NO3)) NO3 = 0.0
    if (is.na(NH4)) NH4 = 0.0
    if (is.na(Na))   Na = 0.0
    if (is.na(SO4)) SO4 = 0.0
    if (is.na(P))     P = 0.0
    
    return(
        c(
            vec,
            paste('SOLUTION    ', soln, title   ),
            paste('  temp      ', T             ),
            paste('  pH        ', pH            ),
            paste('  units     ', 'mg/L'        ),
            paste('  Alkalinity', Alk           ),
            paste('  Ca        ', Ca            ),
            paste('  K         ', K             ),
            paste('  Mg        ', Mg            ),
            paste('  N(5)      ', NO3           ),
            paste('  N(-3)     ', NH4           ),
            paste('  Na        ', Na            ),
            paste('  S(6)      ', SO4           ),
            paste('  P         ', P             )
            )
        )
}

# Create selected_output definition and append to given vector
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
            '    -totals               C(4)  Ca  K  Mg  N(5)  N(-3)  Na',
            '                          S(6)  P                         ',
            '    -molalities           CO2                             ',
            '    -saturation_indices   CO2(g)  Calcite                 ',
            'USER_PUNCH                                                ',
            '-heading CO2_ppm Description                              ',
            '10 PUNCH MOL("CO2") * GFW("CO2") * 1000                   ',
            '20 PUNCH Description                                      '
            )
        )
}

library(phreeqc)

# load database
phrLoadDatabaseString(phreeqc.dat)

# pull in data
yf <- read.csv('Yukon_Flats_DataFile.csv')

# create input (as a character vector)
input <- vector()

# for each solution
for (i in 1:length(yf[,1])) {
    input <- fSoln(input,
                   soln  <- yf[i, 1],
                   title <- yf[i, 2],
                   T     <- yf[i, 4],
                   pH    <- yf[i, 5],
                   Alk   <- yf[i, 10],
                   Ca    <- yf[i, 15],
                   K     <- yf[i, 16],
                   Mg    <- yf[i, 17],
                   NO3   <- yf[i, 22],
                   NH4   <- yf[i, 21],
                   Na    <- yf[i, 18],
                   SO4   <- yf[i, 20],
                   P     <- yf[i, 23])
}

# add selected_output definition
input <- fSelOut(input)

# turn on selected_output file
phrSetSelectedOutputFileOn(TRUE)

# run
phrRunString(input)

# get results
selout <- phrGetSelectedOutput()

