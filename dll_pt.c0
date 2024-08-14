

typedef struct dll_node dll;
struct dll_node {
  elem data; /* This is defined elsewhere (gapbuf.c0 or elem-char.c0) */
  dll* next;
  dll* prev;
};

typedef struct dll_pt_header dll_pt;
struct dll_pt_header {
  dll* start; /* the first node of the list (to the left)  */
  dll* point; /* the current node of the list (where the cursor is) */
  dll* end;   /* the last node of the list (to the right) */
};

bool is_dll_pt(dll_pt* B);

bool dll_pt_at_left(dll_pt* B);  /* Returns true if the point is first  */
                                 /*   first (non-terminal) node         */
bool dll_pt_at_right(dll_pt* B); /* Returns true if the point is last   */
                                 /*   last (non-terminal) node          */

void dll_pt_forward(dll_pt* B);  /* Moves the point forward (right)     */
void dll_pt_backward(dll_pt* B); /* Moves the point backward (left)     */
void dll_pt_delete(dll_pt* B);   /* Remove the current point            */


/*** Implementation (include contracts here) ***/

bool are_consecutive(dll* prev, dll* curr) 
{   
  if (prev == NULL || curr == NULL)
    return false;
  else 
    return prev->next == curr && curr->prev == prev;
}

bool is_dll_seg(dll* a, dll* b){
  if (a == NULL || b == NULL) 
    return false;
  dll* prev = a;
  dll* curr = a;
  while (curr != NULL)
    {
    if (!are_consecutive(prev,curr) && prev != curr)
      return false;
    if (curr == b) 
      return true;
    else{
      prev = curr;
      curr = curr->next;}
    }
  return false;
}


bool is_dll_pt(dll_pt* B){
  return ( B!=NULL &&
          (B->start != B->end) &&
          (B->start != B->point) &&
          (B->end != B->point)&& 
          is_dll_seg(B->start, B->end) &&
          is_dll_seg(B->point, B->end) &&
          is_dll_seg(B->start, B->point));
}

bool dll_pt_at_left(dll_pt* B)
{
  return B->point == B->start->next;
}

bool dll_pt_at_right(dll_pt* B)
{
  return B->point == B->end->prev;
}

void dll_pt_forward(dll_pt* B)
{
  B->point = B->point->next;
}

void dll_pt_backward(dll_pt* B)
{
  B->point = B->point->prev;
}

void dll_pt_delete(dll_pt* B)
{
  if(!dll_pt_at_right(B)){
    dll* tmp = B->point->next;
    B->point->prev->next = B->point->next;
    B->point->next->prev = B->point->prev;
    B->point = tmp;}
  else{
    dll* tmp = B->point->prev;
    B->point->prev->next = B->point->next;
    B->point->next->prev = B->point->prev;
    B->point = tmp;}
}

