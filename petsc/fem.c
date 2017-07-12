#include <petscmat.h>
#include <petscksp.h>
#include <petscvec.h>
#include <petscsys.h>
#include <math.h>

static char help[] = "Help string\n";

int main (int argc,char **args)
{
	Mat A;
	Vec b;
	KSP ksp;
	PetscViewer fd;
	PC pc;
	char file[2][PETSC_MAX_PATH_LEN];
	PetscBool flg = PETSC_FALSE;
	
	PetscInitialize(&argc,&args,(char*)0,help);
	
	PetscOptionsGetString(NULL,NULL,"-f0",file[0],PETSC_MAX_PATH_LEN,&flg);
	if (!flg)
	    SETERRQ(PETSC_COMM_WORLD,1,"Must indicate binary file with the -f0 option");
	    
	    
	PetscOptionsGetString(NULL,NULL,"-f1",file[1],PETSC_MAX_PATH_LEN,&flg);
	
	
	PetscViewerBinaryOpen(PETSC_COMM_WORLD,file[0],FILE_MODE_READ,&fd);
	
	MatCreate(PETSC_COMM_WORLD,&A);
	MatSetType(A,MATSEQAIJ);
	MatLoad(A,fd);

	
	VecCreate(PETSC_COMM_WORLD,&b);
	PetscViewerBinaryOpen(PETSC_COMM_WORLD,file[1],FILE_MODE_READ,&fd);
	VecSetFromOptions(b);
	VecLoad(b,fd);
	
	
	KSPCreate(PETSC_COMM_WORLD,&ksp);
	KSPSetOperators(ksp,A,A);
	KSPSetType(ksp, KSPCG);
	KSPSetTolerances(ksp, 1.e-6, PETSC_DEFAULT, PETSC_DEFAULT, PETSC_DEFAULT);
	KSPGetPC(ksp, &pc);
    PCSetType(pc, PCNONE);
    
    double ti = MPI_Wtime();
	KSPSolve(ksp,b,b);
	PetscPrintf(MPI_COMM_WORLD, "Time is %f\n", MPI_Wtime() - ti);
	
	//VecView(b, PETSC_VIEWER_STDOUT_SELF);
	
	
	PetscViewerDestroy(&fd);
	PetscViewerDestroy(&fd);
	KSPDestroy(&ksp);
	VecDestroy(&b);
	MatDestroy(&A);
	PetscFinalize();
	return 0;
}




