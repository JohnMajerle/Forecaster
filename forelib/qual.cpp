/********* GET_QUAL.C: estimate quality by ******************
 ********* looping over discretization points ***************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "CSample.h"
#include "qual.h"


#define NUM_SECTORS 8            /* number of search sectors */
#define START_MAX_DIST 5000L     /* initial search radius */
#define INC_DIST 2500            /* incremental addn to search limit */
#define END_MAX_DIST 10000L      /* max search limit */
#define MIN_SAMPLE 2             /* min no of est pts in ea. 1/2 plane */
#define EPSILON .1               /* add to distance to prevent divide
                                    by zero if disc. pt = est. pt. */


static void discretize_blk(int pts_wid,int pts_len,
                       double *x_coor,double *y_coor,
                       double *x_disc,double *y_disc,
                       const char *type, double *blk_length,
                       double *blk_width);

static void start_records(long *x,double y_mid,
                   unsigned long num_records,
                   long *north_rec,long *south_rec);

static int estimating_set(long *y,long *x,
               double y_mid,double x_mid,
               unsigned long num_records,int *ht_flag,
               int iter_flag,
               long north_rec,long south_rec,
               double *dist_ht,long *rec_ht,
               double *dist_z,long *rec_z,
               int *flag_sec_ht,int *flag_sec_z);

static void ck_point(int octant,double distance,
              long record_no,double *dist,
              long *rec,int *flag);

static int estimate(unsigned long offset,double *dist_ht,long *rec_ht,
                int *flag_sec_ht,double *dist_z,long *rec_z,
                int *flag_sec_z,double *quality,int num_qual);

static int sumc_int(int *int_vector,int strt_row,int fin_row);

static void sum_qual(double *qual,double *quality,const char *type);
                     
static void get_tons(double *qual,double *blk_length,
                     double *blk_width, double *recovery,
                     const char *type);

long *x_lw,*y_lw,*x_cm,*y_cm;
int *ht_flag_lw,*ht_flag_cm;
unsigned long num_lw,num_cm;
CSample *samples;
double *lw_adj,*cm_adj;
FILE *fp_out;

#pragma optimize("leg",off)

/********************************************************************/
/* initialize tables */

void init_blk_fcst(int num1, int num2, long *xP, long *yP, int *qP,
				   CSample *sP, double *lwaP, double *cmaP)

{
        
   num_lw = num1;   /* number of lw samples */
   num_cm = num2;   /* number of cm samples */
   x_lw = xP;       /* ptr to start of lw x's in sorted array of coords */
   y_lw = yP;       /* ptr to start of lw y's in sorted array of coords */
   ht_flag_lw = qP; /* ptr to start of sorted array ind. if all qual(=1)
                       available or ht only(=0)  */
   x_cm = xP+num1;  /* ptr to start of cm x's in sorted array of coords */
   y_cm = yP+num1;  /* ptr to start of cm y's in sorted array of coords */
   /* ptr to start of sorted array indicating if all quality is available(=1)
      or just ht(=0) */
   ht_flag_cm = qP+num1;  
   samples = sP;
   lw_adj = lwaP;
   cm_adj = cmaP;

}

/***************************************************************************/
/******* discretize block;call block estimator *****************************/
/***************************************************************************/
int blk_fcst(double *qual,double *x_coor,double *y_coor,const char *type,
             double recovery)

{
   double x_disc[TOT_PTS],y_disc[TOT_PTS],blk_length,blk_width;
   int i;
   int result;

   #ifdef PRINT_INTERMEDIATE
       if ((fp_out=fopen("output.out","w")) == NULL) {
           printf("CANNOT OPEN OUTPUT.OUT\n");
           exit(1);
       }
   #endif


   discretize_blk(PTS_WID,PTS_LEN,x_coor,y_coor,x_disc,y_disc,type,
                  &blk_length,&blk_width);

   #ifdef PRINT_INTERMEDIATE
       #ifdef PRINT_ADJUSTMENTS
           print_adj();
       #endif

       #ifdef PRINT_DISC_PTS
           prnt_disc_pts(x_disc,y_disc,&blk_length,&blk_width,x_coor,y_coor);
       #endif
   #endif



   for(i=0; i<NUMQUAL+3; i++) qual[i]=0.0; /* initialize qual array */

   result=get_qual(qual,type,x_disc,y_disc,recovery,blk_length,
                   blk_width);

  #ifdef PRINT_INTERMEDIATE
      #ifdef PRINT_RESULTS
          prnt_results(qual,type,recovery);
          /* if partial wash calculate product quality */
          if(fabs((float)(100.0 - pct_washed())) > eps) {
              pw_product_quality(qual);
          }
      #endif
   #endif

   #ifdef PRINT_INTERMEDIATE
       fclose(fp_out);
   #endif

    return result;

} /* end blk_fct */

/***************************************************************************/
/******* obtain x,y coordinates of discretization points *******************/
/***************************************************************************/
static void discretize_blk(int pts_wid, int pts_len,
                       double *x_coor, double *y_coor,
                       double *x_disc, double *y_disc, const char *type,
                       double *blk_length, double *blk_width)
{
  int num_samp,i,j;
  double apex_x,apex_y,long_x,long_y,short_x,short_y;
  double v1_x,v1_y,v2_x,v2_y;

  *blk_length = sqrt((x_coor[1] - x_coor[0]) * (x_coor[1] - x_coor[0])
                    +(y_coor[1] - y_coor[0]) * (y_coor[1] - y_coor[0]));
  *blk_width  = sqrt((x_coor[3] - x_coor[0]) * (x_coor[3] - x_coor[0])
                    +(y_coor[3] - y_coor[0]) * (y_coor[3] - y_coor[0]));

  if((!memcmp(type,"CM",2)) && (*blk_width > *blk_length)) {
      apex_x  = x_coor[0];
      apex_y  = y_coor[0];
      long_x  = x_coor[3];
      long_y  = y_coor[3];
      short_x = x_coor[1];
      short_y = y_coor[1];
  }
  else {
      apex_x  = x_coor[0];
      apex_y  = y_coor[0];
      long_x  = x_coor[1];
      long_y  = y_coor[1];
      short_x = x_coor[3];
      short_y = y_coor[3];
  }

  num_samp = 0;
  for(i=1;i<=pts_len;i++) {
   v1_x = (i-.5) * ((long_x-apex_x)/(pts_len));
   v1_y = (i-.5) * ((long_y-apex_y)/(pts_len));
   for(j=1;j<=pts_wid;j++) {
    v2_x = (j-.5) * ((short_x-apex_x)/(pts_wid));
    v2_y = (j-.5) * ((short_y-apex_y)/(pts_wid));
    x_disc[num_samp] = apex_x + (v1_x+v2_x);
    y_disc[num_samp] = apex_y + (v1_y+v2_y);
    num_samp++;
    }
  }
}

/********************************************************************/
/* estimate blk. quality;if there are no samples w/in END_MAX_DIST OF
   one of the disc. pts. qual is returned as NULL */

int get_qual(double *qual,const char *type,double *x_disc,double *y_disc,
             double recovery,double blk_length,
             double blk_width)

{

   int i,j,result,stop_flag=0;
   long north_rec,south_rec;
   double quality[NUMQUAL];
   static double dist_ht[9],dist_z[9];
   static long rec_ht[9],rec_z[9];
   static int flag_sec_ht[9],flag_sec_z[9];
           
                    
    /* loop over discretization pts */
    for(i=0;i<TOT_PTS;i++) {
       if(!memcmp(type,"LW",2)) {
         start_records(y_lw,y_disc[i],num_lw,&north_rec,&south_rec);

         j=1;
         do {
          stop_flag=estimating_set(y_lw,x_lw,y_disc[i],x_disc[i],
                         num_lw,ht_flag_lw,j,
                         north_rec,south_rec,dist_ht,rec_ht,
                        dist_z,rec_z,flag_sec_ht,flag_sec_z);
          j++;
         } while(stop_flag<2);

         if(!sumc_int(flag_sec_z,1,8)) return 1; /* no samples within END_MAX_DIST */
                                                 /* qual returned as null array */
         result=estimate(0,dist_ht,rec_ht,flag_sec_ht,dist_z,rec_z,
                         flag_sec_z,quality,NUMQUAL);
         if(result!=1) return result;

       } else {   /* CM case */

           start_records(y_cm,y_disc[i],num_cm,&north_rec,&south_rec);
           j=1;
           do {
            stop_flag=estimating_set(y_cm,x_cm,y_disc[i],x_disc[i],
                           num_cm,ht_flag_cm,j,
                           north_rec,south_rec,dist_ht,rec_ht,
                           dist_z,rec_z,flag_sec_ht,flag_sec_z);


            j++;


           } while(stop_flag<2);

           if(!sumc_int(flag_sec_z,1,8)) return 1; /* no samples within END_MAX_DIST */
                                                   /* qual returned as null array */
           result=estimate(num_lw,dist_ht,rec_ht,flag_sec_ht,dist_z,rec_z,
                           flag_sec_z,quality,NUMQUAL);
           if(result!=1) return result;
       }
       
       /* sum qualities over discretization pts. and make adjustments
          to raw qualities in minimum mining ht case */
       sum_qual(qual,quality,type);

    } /* end loop over discretization points */

    for(i=0; i<NUMQUAL+3; i++) qual[i] /= (double) TOT_PTS;
        /* average over disc pts */ 
        
    /* final tonnage calculations */
    get_tons(qual,&blk_length,&blk_width,&recovery,type);
    
    return 1;  /* estimation successful */
}  /* end get_qual function */


/********************************************************************/
/* determine input record nos. of the pts that are the closest to the disc.
   pt in the northing & southing directions */

static void start_records(long *x,double y_mid,
                          unsigned long num_records,
                          long *north_rec,long *south_rec)
{

 long mid_rec;

  *north_rec = 0;
  *south_rec = num_records-1;
  mid_rec=(*south_rec-*north_rec+1)/2 +1;
  /* integer division truncates remainder */

  do {


   if(y_mid<x[mid_rec]) {
    *north_rec=mid_rec;
    }
   else if(y_mid>x[mid_rec]) *south_rec=mid_rec;
   else {
    *north_rec=mid_rec;
    *south_rec=*north_rec+1;
    }


    mid_rec=*north_rec + (*south_rec-*north_rec+1)/2;
    } while((*south_rec-*north_rec)>1);
 }  /* end start_records function */

/***********************************************************************/
/* determine estimating set for current disc. pt */

static int estimating_set(long *y,long *x,
               double y_mid,double x_mid,
               unsigned long num_records,int *ht_flag,
               int iter_flag,long north_rec,long south_rec,
               double *dist_ht,long *rec_ht,
               double *dist_z,long *rec_z,
               int *flag_sec_ht,int *flag_sec_z)

{
 int north_flag,octant,i,stop_north,stop_south,flag_done;
 long record_no,record_lim;
 double distance;
 static int maxed_north,maxed_south;
 long ns_dist,ew_dist,max_dist;



 if(iter_flag==1){
   /* initialize vectors to contain flags which indicate a sample found
      in the octant corresponding to the row of the vector(unit offset) */
  for(i=1;i<=NUM_SECTORS;i++) flag_sec_ht[i]=0;
  for(i=1;i<=NUM_SECTORS;i++) flag_sec_z[i]=0;
  maxed_north=maxed_south=0;

 }

 max_dist=START_MAX_DIST+(long)(iter_flag-1)*INC_DIST;
 if(max_dist>END_MAX_DIST) return 3;
 /* TWO HALF PLANE SEARCHES: 1ST NORTH then SOUTH */

 for(north_flag=1;north_flag>=0;north_flag--){
  if(north_flag==1){
   record_no=north_rec;
   record_lim=-1;
   flag_done = maxed_north + (sumc_int(flag_sec_z,1,4)==4);
    /* flag_done>0 then either already searched all north samples
       or there is a sample for ea. of quadrants 1-4 */


  }
  else{
   record_no=south_rec;
   record_lim=num_records;
   flag_done = maxed_south + (sumc_int(flag_sec_z,5,8)==4);
    /* flag_done>0 then either already searched all south samples
       or there is a sample for ea. of quadrants 5-8 */

  }


  /* do loop goes in here */
  if(flag_done<1){
   do {
    ns_dist = y[record_no] - (long) y_mid;
    ew_dist = x[record_no] - (long) x_mid;


    if((labs(ew_dist)<max_dist) && (labs(ns_dist)<max_dist)) {
     distance = sqrt(((double)ns_dist * (double)ns_dist)+
                     ((double)ew_dist * (double)ew_dist));

     if(north_flag==1){
        if(ew_dist>=0){
           if(ns_dist<=ew_dist) octant=1;
           else(octant=2);
        }
        else{
           if(ns_dist+ew_dist>=0)octant=3;
           else(octant=4);
        }
     }  /* finish assign. of octant for pts north of disc pt */
     else{
        if(ew_dist>=0){
           if(ew_dist+ns_dist>=0)octant=8;
           else(octant=7);
        }
        else{
           if(ns_dist<=ew_dist)octant=6;
           else(octant=5);
        }
     }

     /* check if point closer than any other in current octant */
     /* if it is enter in appropriate vector */
     /* ht_flag[record_no]==1 then quality & ht available for sample */
     if(ht_flag[record_no]) {
      ck_point(octant,distance,record_no,dist_z,rec_z,
               flag_sec_z);
      ck_point(octant,distance,record_no,dist_ht,rec_ht,
               flag_sec_ht);
      }
      else ck_point(octant,distance,record_no,dist_ht,rec_ht,
               flag_sec_ht);
    }  /* only determine octant if satisfy ew_dist limit */

    if(north_flag==1) record_no--;
    else record_no++;


   } while((record_no != record_lim) && (labs(ns_dist)<max_dist));
    if((north_flag==1) & (record_no==record_lim)) maxed_north=1;
     /* =1 then searched all pts. north */
    if((north_flag==0) & (record_no==record_lim)) maxed_south=1;
     /* =1 then searched all pts. south */
     /* end of do */
  }  /* end if statement for flag_done test */
 }   /* end for loop to do north & south separately */

  stop_north = (sumc_int(flag_sec_z,1,4) >= MIN_SAMPLE) ||
                maxed_north;
  stop_south = (sumc_int(flag_sec_z,5,8) >= MIN_SAMPLE) ||
                maxed_south;
  return stop_north+stop_south;
} /* end estimating set function */


/********************************************************************/
/* determine current pt should replace existing pt in specified octant */

static void ck_point(int octant,double distance,
              long record_no,double *dist,
              long *rec,int *flag)
{

  if(flag[octant]==0) {
   dist[octant]=distance;
   rec[octant]=record_no;
   flag[octant]=1;
  }
  else {
   if(distance<dist[octant]) {
    dist[octant]=distance;
    rec[octant]=record_no;
   }
  }
 } /* end check point function */

/********************************************************************/
/* estimate disc. pt. quality by inverse distance weighting(power=1) */

static int estimate(unsigned long offset,double *dist_ht,long *rec_ht,
              int *flag_sec_ht,double *dist_z,long *rec_z,
              int *flag_sec_z,double *quality,int num_qual)
{
 int i,j;
 double sum_ht_wts=0,sum_z_wts=0,wt_ht,wt_z;

  /* initialize quality vector */
 for(i=0; i<num_qual; i++) quality[i]=0.0;

 for(i=1;i<=NUM_SECTORS;i++) {
  if(flag_sec_ht[i]==1){
   wt_ht=1/(dist_ht[i]+EPSILON);
   sum_ht_wts+=wt_ht;
                                                             
// memcpy(&record_buffer,sampleIndex(offset+rec_ht[i]),sizeof(SAMPLE)); 
   CSample record_buffer = samples[offset+rec_ht[i]];

   quality[0]+=wt_ht*(record_buffer.Quality(0));
   if(flag_sec_z[i]==1) {
    j=1;
    wt_z=1/(dist_z[i]+EPSILON);
    sum_z_wts+=wt_z;
          
    #ifdef PRINT_INTERMEDIATE      
        #ifdef PRINT_EST_SET
            print_estimators(&record_buffer,dist_z[i],i);
        #endif    
    #endif                                                            
//  memcpy(&record_buffer,sampleIndex(offset+rec_z[i]),sizeof(SAMPLE)); 
	CSample record_buffer = samples[offset+rec_z[i]];

    do {
        quality[j]+=wt_z*(record_buffer.Quality(j));
        j++;
      } while(j<num_qual);
   }
  } /* end if condition for ht flag */
 }  /* end for loop over sections */
  for(i=0;i<num_qual;i++) {
   if(i==0) quality[i]*=(1/sum_ht_wts);
   else quality[i]*=(1/sum_z_wts);
  }

  return 1;
 } /* end estimate function */

/********************************************************************/
/* determine no. of estimating pts in half plane */
/********************************************************************/

static int sumc_int(int *int_vector,int strt_row,int fin_row)

{
 int i,sum=0;

 for(i=strt_row;i<=fin_row;i++) sum += int_vector[i];

 return sum;
}
/********************************************************************/
/************* sum qualities over discretization pts ****************/
/******** make adjustments to raw in minimum mining ht case *********/
/********************************************************************/
static void sum_qual(double *qual,double *quality,const char *type)
                     
{

    int i;
    double *p_adj; 
    double eps = 1e-5; 
    double dil_thick,adj_qual[NUMQUAL];                   
    
    p_adj = (memcmp(type,"LW",2)==0)? lw_adj: cm_adj;
                                
    /* initialize adj_qual.. for comparing before & after
       adjustments for minimum mining ht case */
    /* quality contains estimate before adjustments; adj_qual with adjs */   
    for(i=0; i<NUMQUAL; i++) adj_qual[i] = quality[i];
                                
    /* clean qualities..not adjusted..*/
    /* qual[i] is array in which qualities are summed..
       it will contain the final block estimate */
    for (i=11; i<NUMQUAL; i++)
        qual[i] += quality[i]; 
                              
    /* raw qualities */                          
    if(p_adj[10] > eps) {  /* minimum mining ht. case */
        adj_qual[0] = quality[0] + p_adj[0];
        dil_thick = p_adj[0];
        /* check if diluted thickness exceeds minimum mining ht */
        if (adj_qual[0] < p_adj[10]) {
            dil_thick = p_adj[10] - quality[0];
            adj_qual[0] = p_adj[10];
        }   
        qual[0] += adj_qual[0];        
                
        /* adjusted raw s.g. */
        if (quality[1] > eps) {
            adj_qual[1] = (quality[0]*quality[1] + dil_thick*p_adj[1])
                          / adj_qual[0];
            qual[1] += adj_qual[1];               
        }    
                       
        /* adjust raw ash,sul,btu */                 
        for (i=2; i<=4; ++i) {
            if (quality[i] > eps)
                adj_qual[i] = (quality[0]*quality[1]*quality[i] +
                              dil_thick*p_adj[1]*p_adj[i]) /
                              (quality[0]*quality[1] + dil_thick*p_adj[1]);
        }                     
                         
        /* adjusted raw vol */
        if (quality[2] > eps)
            adj_qual[5] = (quality[5] * (100-adj_qual[2]))
                          / (100.-quality[2]);
    
        
        /* adjusted raw ash fusion temperatures */
        for (i=6; i<9; i++) 
            qual[i] += quality[i];
            
        /* raw moisture adjustments-ash,sul,btu,vol */
        for (i=2; i<=5; i++) {
            adj_qual[i] = adj_qual[i] * (100-p_adj[5]) / 100.;
            qual[i] += adj_qual[i];
        }
            
        /* adjusted yield */
        adj_qual[10] = quality[0]*quality[1]*quality[10] /
                       (quality[0]*quality[1] + dil_thick*p_adj[1]);
        adj_qual[10] = adj_qual[10] * (p_adj[8]/100.);           
        qual[10] += adj_qual[10];
        
        /* tonnage calculations */
        qual[NUMQUAL]   += adj_qual[0] * adj_qual[1];
        qual[NUMQUAL+1] += adj_qual[0] * adj_qual[1] * adj_qual[10]; 
        
        /* seam height */
        qual[NUMQUAL+2] += quality[0];
    }                            
    
    else {
        for(i=0; i<11; i++)
            qual[i] += quality[i];  
            
        /* tonnage calculations */
        qual[NUMQUAL]   += quality[0] * quality[1];
        qual[NUMQUAL+1] += quality[0] * quality[1] * quality[10];
            
        /* seam height */
        qual[NUMQUAL+2] += quality[0] - p_adj[0];
    }         
    
    #ifdef PRINT_INTERMEDIATE  
        #ifdef PRINT_ADJ_QUAL
            compare_adj_qual(quality,adj_qual);
        #endif    
    #endif    
 }                                   
/********************************************************************/
/*********************** calculate tonnages *************************/
/********************************************************************/
static void get_tons(double *qual,double *blk_length,
                     double *blk_width, double *recovery,
                     const char *type)
{                     

    double *p_adj; 
    double eps = 1e-5; 
    double raw_tons,clean_tons,raw_fraction_tons,clean_fraction_tons;
    
    p_adj = (memcmp(type,"LW",2)==0)? lw_adj: cm_adj;
                                                    
    #ifdef PRINT_INTERMEDIATE  
        #ifdef PRINT_ADJ_QUAL
            fprintf(fp_out,"FROM GET_TONS \n");
            fprintf(fp_out,"AVG HT*SG=%.4lf\n",*(qual+NUMQUAL));                                                
            fprintf(fp_out,"AVG HT*SG*YIELD=%.4lf\n",*(qual+NUMQUAL+1));                                                
        #endif    
    #endif    
                                                    
    raw_tons = (*(qual+NUMQUAL) * SG_WATER * (*blk_length) *
               (*blk_width) ) / LBS_PER_TON;
    raw_tons = raw_tons * (*recovery) / 100.;  /* recovery in %: so /100. */
    clean_tons = (*(qual+NUMQUAL+1) * SG_WATER * (*blk_length) *
                 (*blk_width)) / (LBS_PER_TON * 100.);  
                 /* yield in %: so /100. */
    clean_tons = clean_tons * (*recovery) / 100.;
    
    
    if(fabs((float)(100.0 - cm_adj[11])) > eps) { /* partial wash */
        raw_fraction_tons = raw_tons * ((100. - p_adj[11]) / 100.);
        clean_fraction_tons = clean_tons * (p_adj[11] / 100.);
        *(qual+NUMQUAL)   = raw_fraction_tons;      /* raw product tons */
        *(qual+NUMQUAL+1) = clean_fraction_tons;    /* clean product tons */
     }
     else {  
        *(qual+NUMQUAL)   = raw_tons;
        *(qual+NUMQUAL+1) = clean_tons;
     }
}

#pragma optimize("",on)
