       .ORG   $0000
        LDI    R2, SUM
        LDI    R0, HEAD
        LDR    R1, R0
LOOP    CMI    R0, $0
        BRZ    DONE
        ADD    R2, R1
        INCR   R0
        LDR    R1, R0
        MOV    R0, R1
DONE    STOP

        .ORG   $2000
HEAD    .DW    $12
        .DW    CALC

        .ORG   $2100
CALC    .DW    $10
        .DW    RUSS

        .ORG   $2200
RUSS    .DW    $9
        .DW    PHYS

        .ORG   $2300
PHYS    .DW    $12
        .DW    MODMATH

        .ORG   $2400
MODMATH .DW    $9
        .DW    $0

        .ORG   $5000
SUM     .DW    $0 
