#! /bin/bash -e
if [ "$1" != 'forward' ] && [ "$1" != 'reverse' ]
then
 	echo 'usage: ./colpack.sh option'
 	echo 'where option is "forward" or "reverse"'
 	exit 1
fi
if [ "$1" == 'forward' ]
then
	color_variant="COLUMN_PARTIAL_DISTANCE_TWO"
else
	color_variant="ROW_PARTIAL_DISTANCE_TWO"
fi
# ----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------
if [ ! -e 'build/colpack' ]
then
	echo_eval mkdir -p build/colpack
fi
echo 'create: build/colpack/colpack.cpp'
cat<< EOF > build/colpack/colpack.cpp
// Example using BipartiteGraphPartialColoringInterface 
// to generate the seed matrix for Jacobian

#include "ColPackHeaders.h"

int main()
{	using std::cout;
	using std::endl;	

	double*** dp3_Seed = new double**;
	int *ip1_SeedRowCount = new int;
	int *ip1_SeedColumnCount = new int;

	//* 32x9 matrix
	int          i_RowCount          = 32;
	int          i_ColumnCount       = 9;
	int          i_MaxNonZerosInRows = 3;

	// JP[32][9]
	unsigned int **JP                   = new unsigned int* [i_RowCount];
	for(int i = 0; i < i_RowCount; i++)
		JP[i] = new unsigned int[i_MaxNonZerosInRows + 1];
	//
	JP[0][0] = 0;
	JP[1][0] = 1;   JP[1][1] = 0;
	JP[2][0] = 1;   JP[2][1] = 1;
	JP[3][0] = 1;   JP[3][1] = 2;
	JP[4][0] = 1;   JP[4][1] = 0;
	JP[5][0] = 3;   JP[5][1] = 0;   JP[5][2] = 1;   JP[5][3] = 3;
	JP[6][0] = 3;   JP[6][1] = 1;   JP[6][2] = 2;   JP[6][3] = 4;
	JP[7][0] = 2;   JP[7][1] = 2;   JP[7][2] = 5;
	JP[8][0] = 1;   JP[8][1] = 3;
	JP[9][0] = 3;   JP[9][1] = 3;   JP[9][2] = 4;   JP[9][3] = 6;
	JP[10][0] = 3;  JP[10][1] = 4;  JP[10][2] = 5;  JP[10][3] = 7;
	JP[11][0] = 2;  JP[11][1] = 5;  JP[11][2] = 8;
	JP[12][0] = 1;  JP[12][1] = 6;
	JP[13][0] = 2;  JP[13][1] = 6;  JP[13][2] = 7;
	JP[14][0] = 2;  JP[14][1] = 7;  JP[14][2] = 8;
	JP[15][0] = 1;  JP[15][1] = 8;
	JP[16][0] = 1;  JP[16][1] = 0;
	JP[17][0] = 2;  JP[17][1] = 0;  JP[17][2] = 1;
	JP[18][0] = 2;  JP[18][1] = 1;  JP[18][2] = 2;
	JP[19][0] = 1;  JP[19][1] = 2;
	JP[20][0] = 2;  JP[20][1] = 0;  JP[20][2] = 3;
	JP[21][0] = 3;  JP[21][1] = 1;  JP[21][2] = 3;  JP[21][3] = 4;
	JP[22][0] = 3;  JP[22][1] = 2;  JP[22][2] = 4;  JP[22][3] = 5;
	JP[23][0] = 1;  JP[23][1] = 5;
	JP[24][0] = 2;  JP[24][1] = 3;  JP[24][2] = 6;
	JP[25][0] = 3;  JP[25][1] = 4;  JP[25][2] = 6;  JP[25][3] = 7;
	JP[26][0] = 3;  JP[26][1] = 5;  JP[26][2] = 7;  JP[26][3] = 8;
	JP[27][0] = 1;  JP[27][1] = 8;
	JP[28][0] = 1;  JP[28][1] = 6;
	JP[29][0] = 1;  JP[29][1] = 7;
	JP[30][0] = 1;  JP[30][1] = 8;
	JP[31][0] = 0;
	cout << endl << "Sparsity pattern of Jacobian:" << endl;
	for(int i = 0; i < i_RowCount; i++)
	{	cout << i << ": ";
		for (int j = 1; j <= (int) JP[i][0]; j++)
			cout << " " << JP[i][j];
		cout << endl;
	}


	// Step 1: Read the sparsity pattern of the given Jacobian matrix 
	// (adolc format) and create the corresponding bipartite graph
	ColPack::BipartiteGraphPartialColoringInterface * g = new 
		ColPack::BipartiteGraphPartialColoringInterface(
			SRC_MEM_ADOLC, JP, i_RowCount, i_ColumnCount
	);
	g->PrintBipartiteGraph();

	// Step 2: Do Partial-Distance-Two-Coloring 
	// of the bipartite graph with the specified ordering
	g->PartialDistanceTwoColoring(
		"SMALLEST_LAST", "$color_variant"
	);
	g->PrintColumnPartialColors();
	g->PrintColumnPartialColoringMetrics();

	// Step 3: From the coloring information, create and return seed matrix
	(*dp3_Seed) = g->GetSeedMatrix(ip1_SeedRowCount, ip1_SeedColumnCount);
	double **RSeed = *dp3_Seed;
	int rows = *ip1_SeedRowCount;
	int cols = *ip1_SeedColumnCount;
	cout << "Seed matrix: (" << rows << "," << cols << ")" << endl;
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			cout << setw(2) << int(RSeed[i][j]);
		}
		cout << endl;
	}



	//GraphColoringInterface * g = new GraphColoringInterface();
	delete g;
	g = NULL;

	//double*** dp3_Seed = new double**;
	delete dp3_Seed;
	dp3_Seed = NULL;
	RSeed = NULL;

	//int *ip1_SeedRowCount = new int;
	delete ip1_SeedRowCount;
	ip1_SeedRowCount = NULL;

	//int *ip1_SeedColumnCount = new int;
	delete ip1_SeedColumnCount;
	ip1_SeedColumnCount = NULL;

	// unsigned int **Jp = new unsigned int *[i_RowCount]; //[5][5]
	free_2DMatrix(JP, i_RowCount);
	JP = NULL;

	return 0;
}
EOF
# ----------------------------------------------------------------------------
echo_eval cd build/colpack
echo_eval g++ colpack.cpp \
	-I$HOME/prefix/colpack/include/ColPack \
	-L$HOME/prefix/colpack/lib64 \
	-l ColPack \
	-o colpack
#
echo_eval valgrind --leak-check=yes ./colpack
