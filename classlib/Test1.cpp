#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "list.h"
#include "assign.h"

#define FREE_ASSIGNMENT       '1'
#define NEW_PATIENT           '2'
#define SHOW_WAITING_PATIENTS '3'
#define SHOW_FREE_DOCTORS     '4'
#define SHOW_ASSIGNMENTS      '5'
#define EXIT_PROGRAM          '6'

void init_doctor_list(void);
void display_options(void);
int  get_option(void);
void free_assignment(void);
void check_status(void);
void new_patient(void);
void show_wait(void);
void show_free(void);
void show_assign(void);

List<physician> doc_list(0);
List<patient>   pat_list(0);
List<assign>    assign_list(0);

int main()
{
      int done=0;

      init_doctor_list();

      while (!done) {
         display_options();
         switch (get_option()) {
            case FREE_ASSIGNMENT:
               free_assignment();
               break;
            case NEW_PATIENT:
               new_patient();
               break;
            case SHOW_WAITING_PATIENTS:
               show_wait();
               break;
            case SHOW_FREE_DOCTORS:
               show_free();
               break;
            case SHOW_ASSIGNMENTS:
               show_assign();
               break;
            case EXIT_PROGRAM:
               done = 1;
               break;
         }
      }

      return 0;
}

void init_doctor_list(void)
{
      char buffer[50];
      int  i,num_docs;
      physician *p_doc;

      printf("How many doctors? "); get_line(buffer);
      num_docs = atoi(buffer);
      for (i=0; i<num_docs; ++i) {
		p_doc = new physician;
		p_doc->getInfo();
		doc_list.Enqueue(p_doc);
      }
}

void display_options(void)
{
      printf("\n");
      printf("1. Free Doctor from Assignment\n");
      printf("2. New Patient Arrival\n");
      printf("3. Show Waiting Patients\n");
      printf("4. Show Available Doctors\n");
      printf("5. Show Assignments\n");
      printf("6. Exit Program\n");
}

int get_option(void)
{
      fputs("Enter Choice: ",stdout); return _getche();
}

void free_assignment(void)
{
      char free_id[100];
      assign *pa;

      printf("Physician ID: ");
      get_line(free_id);

      pa = assign_list.Find(free_id);
      if (pa) {
         assign_list.Remove();
         delete pa->pat();
         doc_list.Enqueue(pa->doc());
         delete pa;
         printf("physician %s freed\n", free_id);
         check_status();
      } else {
         printf("Sorry...\n");
         printf("Can't find physician %s in assigned list\n",free_id);
      }
}

void check_status(void)
{
      int n,nend=pat_list.Count();
      patient *pp;
      physician *pdoc;
      assign *pa;

      for (n=0; n<nend; n++) {
         pp = pat_list.Get(n);
         pdoc = doc_list.Find((void *)pp->spec());
         if (pdoc) {
            pa = new assign(pdoc,pp);
            assign_list.Enqueue(pa);
            doc_list.Remove();
            pat_list.Remove();
            printf("New Assignment\n");
            pa->print(stdout);
			return;
         }
      }
}

void new_patient(void)
{
	patient *pp = new patient;
	pp->getInfo();
	pat_list.Enqueue(pp);
    check_status();
}

void show_wait(void)
{
      if (pat_list.Count()) {
         printf("Waiting Patients\n");
         pat_list.print(stdout);
      } else
         printf("No Waiting Patients\n");
}

void show_free(void)
{
      if (doc_list.Count()) {
         printf("Free Physicians\n");
         doc_list.print(stdout);
      } else
         printf("No Free Doctors\n");
}

void show_assign(void)
{
      if (assign_list.Count()) {
         printf("Assignments\n");
         assign_list.print(stdout);
      } else
         printf("No Assignments Being Processed\n");
}
