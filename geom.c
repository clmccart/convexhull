#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const double LARGE = 9.9e7;
static const double EPSILON = 1e-10;

/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2D a, point2D b, point2D c) {

  return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x));
}
//CLAIRE 2/6 4pm


/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r) {

  return (signed_area2D(p, q, r) == 0);
}
//CLAIRE 2/6 4pm


/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left (point2D a, point2D b, point2D c) {

  return (signed_area2D(a, b, c) > 0);
}
//CLAIRE 2/6 4pm

/* swaps 2 points a and b */
void swap (point2D *a, point2D *b) {

  point2D *temp = a;
  a->x = b->x;
  a->y = b->y;
//  b = temp;
  b->x = temp->x;
  b->y = temp->y;
}

/* comparison function for sorting by ccw angle */
int compare(const void *a, const void *b) {
  point2D *p1 = (point2D *)a;
  point2D *p2 = (point2D *)b;

  double a1 = atan2((double)(p1->y - P->y), (double)(p1->x - P->x));
  double a2 = atan2((double)(p2->y - P->y), (double)(p2->x - P->x));

  // TODO take care of when they equal each other

  //if (a1 == a2) return 0;
  if (a1 > a2) return -1;
  if (a1 - a2 < EPSILON) return (p1->x < p2->x);
  return 1;
}

/* STACK IMPLEMENTATION using a linked list */
pointNode* push(pointNode* head,point2D a)
{
    pointNode* tmp = (pointNode*)malloc(sizeof(pointNode *));
    if(tmp == NULL)
    {
        exit(0);
    }
    tmp->p = a;
    tmp->next = head;
    head = tmp;
    return head;
}

pointNode* pop(pointNode *head)
{
    pointNode* tmp = head;
    head = head->next;
    free(tmp);
    return head;
}

void printer(point2D *p, int n) {
  for (int i = 0; i < n; ++i) {
    double angle = atan2((double)(p[i].y - P->y), (double)(p[i].x - P->x));
    printf("(%d, %d)   angle: %f", p[i].x, p[i].y, angle);
  }
  printf("\n");

}

/* compute the convex hull of the points in p; the points on the CH are returned as a list
*/
pointNode* graham_scan(point2D *p, int n) {
  pointNode *head = NULL;
  // handle the case where only 3 points on CH
  if (n <= 3) {
    for (int i = 0; i < n; ++i) {
      head = push(head, p[i]);
    }
    return head;
  }


  //find the point with the lowest y value
  int minY_index = 0;
  P = (point2D *)malloc(sizeof(point2D));
  P->x = 0.0;
  P->y = LARGE;

  for (int i = 0; i < n; i++) {
    if (p[i].y < P->y) {
      P = &p[i];
      minY_index = i;
    }
  }
  printf("first point: %d %d\n", p[0].x, p[0].y);
  swap(&p[0], &p[minY_index]);
  printf("first point: %d %d\n", p[0].x, p[0].y);
  // sort the points in ccw order
  qsort(&p[1], n-1, sizeof(point2D), compare);
  printer(p,n);

  // add the first 3 points onto the convex hull

  head = push(head, p[0]);
  head = push(head, p[1]);
  head = push(head, p[2]);
  int count = 3;

  // trverse all points and maintain the convex hull of all traversed points
  for (int i = 3; i < n; i++) {

    // if the point is to the left of the previous 2 points, add to the CH
    printf("next point: %d %d\n",head->p.x, head->p.y);
    if (left(head->next->p, head->p, p[i])) {
    //  printf("it's to the left\n");
      head = push(head,p[i]);
      count++;

    } else {
      // repeat popping from he stack while the point is to the right of the top
      // 2 points on the CH
      while (count > 2 && head->next != NULL && !left(head->next->p, head->p, p[i])) {
        printf("popping, to the right\n");
        head = pop(head);
        count--;
      }
      // then add to the CH
      head = push(head, p[i]);
      count++;
      //printf("now pushing\n");
    }
  }

  return head;
}
//CLAIRE 2/6 4pm
