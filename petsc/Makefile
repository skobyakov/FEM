include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

fem: fem.o  chkopts
	-${CLINKER} -o fem fem.o  ${PETSC_LIB}
	${RM} fem.o
