#include "qhullTest.h"
#include "qhull_ra.h"
#include <iostream>

//Code from user_eg_r.c

#define DIM 3     /* dimension of points, must be < 31 for SIZEcube */
#define SIZEcube (1<<DIM)
#define SIZEdiamond (2*DIM)
#define TOTpoints (SIZEcube + SIZEdiamond)

void print_summary(qhT *qh) {
	facetT *facet;
	int k;

	printf("\n%d vertices and %d facets with normals:\n",
		qh->num_vertices, qh->num_facets);
	FORALLfacets{
		for (k = 0; k < qh->hull_dim; k++)
		printf("%6.2g ", facet->normal[k]);
	printf("\n");
	}
}

/*--------------------------------------------------
-makehalf- set points to halfspaces for a (dim)-dimensional diamond
points is numpoints X dim+1

each halfspace consists of dim coefficients followed by an offset
*/
void findDelaunay(qhT *qh, int dim) {
	int k;
	coordT point[100];
	boolT isoutside;
	realT bestdist;
	facetT *facet;
	vertexT *vertex, **vertexp;

	for (k = 0; k < dim; k++)
		point[k] = 0.5;
	qh_setdelaunay(qh, dim + 1, 1, point);
	facet = qh_findbestfacet(qh, point, qh_ALL, &bestdist, &isoutside);
	if (facet->tricoplanar) {
		fprintf(stderr, "findDelaunay: not implemented for triangulated, non-simplicial Delaunay regions (tricoplanar facet, f%d).\n",
			facet->id);
		qh_errexit(qh, qh_ERRqhull, facet, NULL);
	}
	FOREACHvertex_(facet->vertices) {
		for (k = 0; k < dim; k++)
			printf("%5.2f ", vertex->point[k]);
		printf("\n");
	}
}

//used to test if qhull included properly. Also used to experiement with qhull
qhullTest::qhullTest()
{
	int dim = DIM;             /* dimension of points */
	int numpoints;            /* number of points */

							  /* array of coordinates for each point */
	coordT points[] = { 0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f
	};
	coordT *rows[8];
	boolT ismalloc = False;    /* True if qhull should free points in qh_freeqhull() or reallocation */
	char flags[250];          /* option flags for qhull, see qh-quick.htm */
	FILE *outfile = stdout;    /* output from qh_produce_output()
							   use NULL to skip qh_produce_output() */
	FILE *errfile = stderr;    /* error messages from qhull code */
	int exitcode;             /* 0 if no error from qhull */
	facetT *facet;            /* set by FORALLfacets */
	int curlong, totlong;     /* memory remaining after qh_memfreeshort */
	int i;

	qhT qh_qh;                /* Qhull's data structure.  First argument of most calls */
	qhT *qh = &qh_qh;

	std::cout << "qhull test called" << std::endl;

	printf("\ncompute %d-d Delaunay triangulation\n", dim);
	numpoints = SIZEcube;
	for (i = numpoints; i--; )
		rows[i] = points + dim*i;
	exitcode = qh_new_qhull(qh, dim, numpoints, points, ismalloc,
		flags, outfile, errfile);
	if (!exitcode) {                  /* if no error */
									  /* 'qh->facet_list' contains the convex hull */
									  /* If you want a Voronoi diagram ('v') and do not request output (i.e., outfile=NULL),
									  call qh_setvoronoi_all() after qh_new_qhull(). */
		print_summary(qh);
		FORALLfacets{
			/* ... your code ... */
		}
		printf("\nfind %d-d Delaunay triangle closest to [0.5, 0.5, ...]\n", dim);
		exitcode = setjmp(qh->errexit);
		if (!exitcode) {
			/* Trap Qhull errors in findDelaunay().  Without the setjmp(), Qhull
			will exit() after reporting an error */
			qh->NOerrexit = False;
			findDelaunay(qh, DIM);
		}
		qh->NOerrexit = True;
	}
	{
		coordT pointsB[DIM*TOTpoints]; /* array of coordinates for each point */

		qhT qh_qhB;    /* Create a new instance of Qhull (qhB) */
		qhT *qhB = &qh_qhB;
		qh_zero(qhB, errfile);

		printf("\nCompute a new triangulation as a separate instance of Qhull\n");
		numpoints = SIZEcube;
		for (i = numpoints; i--; )
			rows[i] = pointsB + dim*i;
		exitcode = qh_new_qhull(qhB, dim, numpoints, pointsB, ismalloc,
			flags, outfile, errfile);
		if (!exitcode)
			print_summary(qhB);
		printf("\nFree memory allocated by the new instance of Qhull, and redisplay the old results.\n");
		qh_freeqhull(qhB, !qh_ALL);                 /* free long memory */
		qh_memfreeshort(qhB, &curlong, &totlong);  /* free short memory and memory allocator */
		if (curlong || totlong)
			fprintf(errfile, "qhull internal warning (user_eg, #4): did not free %d bytes of long memory (%d pieces)\n", totlong, curlong);

		printf("\n\n");
		print_summary(qh);  /* The other instance is unchanged */
							/* Exiting the block frees qh_qhB */
	}
	qh_freeqhull(qh, !qh_ALL);                 /* free long memory */
	qh_memfreeshort(qh, &curlong, &totlong);  /* free short memory and memory allocator */
	if (curlong || totlong)
		fprintf(errfile, "qhull internal warning (user_eg, #2): did not free %d bytes of long memory (%d pieces)\n", totlong, curlong);


	system("pause");
}


qhullTest::~qhullTest()
{
}
