#include <stdio.h>
#include <stdlib.h>

// Constants used by PHP
const int a1 = 40014;
const int a2 = 40692;
const int m1 = 2147483563;
const int m2 = 2147483399;

// error term. We watch within +- err of the target
const int err = 100;


typedef struct pair {
        int a;
        int b;
} pair;

// Taken from the PHP source
#define MODMULT(a, b, c, m, s) q = s/a;s=b*(s-a*q)-c*q;if(s<0)s+=m


/* Assuming pr is a pair (a,b) such that d == a-b mod m1-1. Update pr
 * to two new values, satisfying this relation. Will run through all
 * possible combinations for a in [0,m1-1], b in [0, m2-1]. Will
 * always return such a valid pair.
 */

#define update_pair(d, p)				  \
	do {						  \
	if(p.a >= m1 - 1) { p.a = 0; p.b = -d + (m1 + 1); \
	}else if(p.b >= m2 - 1){ p.a = d; p.b = 0;	  \
	}else{ p.a++; p.b++; }} while(0)

/* static inline void update_pair(int d, pair *p) */
/* { */
/*         if(p->a >= m1 - 1) { */
/*                 p->a = 0; */
/*                 p->b = -d + (m1 - 1); */
/*         } */
/*         else if(p->b >= m2 - 1) { */
/*                 p->a = d; */
/*                 p->b = 0; */
/*         } */
/*         else { */
/*                 p->a++; */
/*                 p->b++; */
/*         } */

/*         return; */
/* } */

int main(int argc, char **argv)
{
        int x1, x2, x3, i, x2a, x2b, x3a, x3b, x2ab, x3ab;
        /* int a, b, q, z, found_pair; */
        FILE *fp;

        if(argc < 4) {
                printf("Usage: %s [X1] [X2] [X3]\n", argv[0]);
                printf("  Where X1,X2,X3 are 3 consectutive values of the php CLGC\n");
                exit(EXIT_FAILURE);
        }

        if(!(fp = fopen("output", "w+"))) {
                printf("Could not open output file...\n");
                exit(EXIT_FAILURE);
        }

        x1 = atoi(argv[1]);
        x2 = atoi(argv[2]);
        x3 = atoi(argv[3]);

        // Used to calculate if z is in [a, b] mod (m1 - 1)
        // taken from: https://fgiesen.wordpress.com/2015/09/24/intervals-in-modular-arithmetic/
        x2a = x2 - err; x2b = x2 + err; x2ab = x2b < x2a ? (x2b - x2a) + (m1 - 1) : (x2b - x2a);
        x3a = x3 - err; x3b = x3 + err; x3ab = x3b < x3a ? (x3b - x3a) + (m1 - 1) : (x3b - x3a);

        printf("a1 = %d, m1 = %d\na2 = %d, m2 = %d\n", a1, m1, a2, m2);
        printf("x1 = %d +/- %d\nx2 = %d +/- %d\nx3 = %d +/- %d\n", x1, err, x2, err, x3, err);

        setbuf(stdout, NULL);
        setbuf(fp, NULL);

#pragma omp parallel for num_threads(6)

        for(i = -err; i < err + 1; i++) {

                pair p;
                int d, z, q, found_pair, a, b;

                d = x1 + i;

                p.a = d;
                p.b = 0;

                found_pair = 0;

                while(1) {
                        a = p.a;
                        b = p.b;

                        MODMULT(53668, a1, 12211, m1, a);
                        MODMULT(52774, a2, 3791, m2, b);

                        z = a - b;

                        if(z < 1) {
                                z += m1 - 1;
                        }

                        if((z < x2a ? (z - x2a) + (m1 - 1) : (z - x2a)) <= x2ab) {

                                /* printf("."); */

                                MODMULT(53668, a1, 12211, m1, a);
                                MODMULT(52774, a2, 3791, m2, b);

                                z = a - b;

                                if(z < 1) {
                                        z += m1 - 1;
                                }

                                if((z < x3a ? (z - x3a) + (m1 - 1) : (z - x3a)) <= x3ab) {
                                        found_pair = 1;
                                        break;
                                }

                        }

                        update_pair(d, p);

                        if(p.a == d && p.b == 0) {
                                break;
                        }
                }

                if(found_pair){
                        printf("#");
                        fprintf(fp, ":%d:%d:%d\n", p.a, p.b, i);
                } else {
                        printf("x");
                }

        }

        printf("\n");

        fclose(fp);

        return EXIT_SUCCESS;
}
