
#define PTS_WID  1  /* disc pts 90 deg from direction of mining */
#define PTS_LEN  3  /* disc pts in direction of mining */
#define TOT_PTS  PTS_WID*PTS_LEN
#define SG_WATER 62.37191
#define LBS_PER_TON 2000.

void init_blk_fcst(int num1, int num2, long *xP, long *yP, int *qP, 
				   CSample *sP, double *lwaP, double *cmaP);

int blk_fcst(double *qual,double *x_coord, double *y_coord,const char *type,
             double recovery);

int get_qual(double *qual,const char *type,double *x_disc,double *y_disc,
             double recovery,double blk_length,
             double blk_width);

/* define statements for printing intermediate results */


/* if PRINT_INTERMEDIATE defined: open output.out for printing intermediate
   results */

/* #define PRINT_INTERMEDIATE */
#define PRINT_ADJUSTMENTS     /* print adjustments */
#define PRINT_DISC_PTS        /* block length,width & coor of disc pts */
#define PRINT_ADJ_QUAL        /* compare disc pt est. before & after adj */
#define PRINT_EST_SET         /* print estimating set */
#define PRINT_PROD_QUAL       /* if partial wash print product quality */
#define PRINT_RESULTS         /* print block estimates */


/* functions associated with above define statements */
void print_adj(void);
void prnt_disc_pts(double *x_disc, double *y_disc, double *blk_length,
                   double *blk_width,double *x_coor,double *y_coor);
void compare_adj_qual(double *quality, double *adj_qual);
void print_estimators(void *buffer,double distance,int samp_no);
void prnt_prod_qual(double *prd_qual,double *qual);
void prnt_results(double *qual,char *type,double recovery);
void pw_product_quality(double *qual);
