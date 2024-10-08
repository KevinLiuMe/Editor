

typedef dll_pt tbuf;

bool is_tbuf(tbuf* B);

tbuf* tbuf_new();                  /* Creates an empty text buffer with   */
                                   /* gap buffers of size 16              */
void tbuf_split_pt(tbuf* B);       /* Splits a full point into two nodes  */
                                   /* so that the new point is not full   */
void tbuf_forward(tbuf* B);        /* Move the cursor forward/right by 1  */
void tbuf_backward(tbuf* B);       /* Move the cursor backward/left by 1  */
void tbuf_insert(tbuf* B, char c); /* Insert c before the cursor          */
void tbuf_delete(tbuf* B);         /* Delete the char before the cursor   */
                                   /* and delete point if it is empty     */

bool gap_buf_check(dll* A, dll* B) 
{
    gapbuf* a = A->data;
    gapbuf* b = B->data;
    if (!is_gapbuf(a))
        return false;
    else if((a == b) && (is_gapbuf(a)) && is_gapbuf(b)
                && a->limit == 16 && b->limit == 16)
        return true;
    else if ((a==b) &&((!is_gapbuf(a) || !is_gapbuf(b)) ||
                a->limit != 16))
        return false;
    else if (is_gapbuf(a)) {
        if (a->limit == 16) {
            if (A->next != NULL)
                return gap_buf_check(A->next, B);
            else 
                return false;
        } else 
            return false;
    }
    return false;
}

bool no_empty_gapbuf_check(dll*A, dll*B)
{   
    gapbuf* a = A->data;
    gapbuf* b = B->data;
    if((a == b) && !gapbuf_empty(a) && !gapbuf_empty(b))
        return true;
    else if ((a == b) && (gapbuf_empty(a) || gapbuf_empty(b)))
        return false;
    else if (gapbuf_empty(a))
        return false;
    else if (!gapbuf_empty(a))
        return no_empty_gapbuf_check(A->next, B);
    else
        return false;
}

bool only_one_empty_gapbuf_check(tbuf* B)
{
    return(gapbuf_empty(B->start->next->data) && 
            B->start->next->next == B->end);
}

bool alignment_check(tbuf* B)
{
    dll* curr = B->start->next;
    int direction = 1; // 1 right ; 0 left
    while(curr != B->end){
        if(curr == B->point)
            direction = 0;
        else if (!((direction == 1 && gapbuf_at_right(curr->data)) ||
                 (direction == 0 && gapbuf_at_left(curr->data))))
            return false;
        curr = curr->next;
        }
    return true;
}

bool is_tbuf(tbuf* B)
{
    return( B!= NULL  && is_dll_pt(B) && 
            gap_buf_check(B->start->next, B->end->prev) &&
            (no_empty_gapbuf_check(B->start->next, B->end->prev) || 
              only_one_empty_gapbuf_check(B)) &&
              alignment_check(B));
} 

dll* new_node()
{
    dll* node = alloc(dll);
    node->prev = NULL;
    node->next = NULL;
    node->data = gapbuf_new(16);
    return node;
}

tbuf* tbuf_new()
{
    dll_pt* header = alloc(struct dll_pt_header);
    header->start = new_node();
    dll* first = header->start;

    first->next = new_node();
    first->next->prev = first;
    dll* second = first->next;

    second->next = new_node();
    second->next->prev = second;
    dll* third = second->next;

    header->point =  second;
    header->end = third;

    return header;
}

gapbuf* gapbuf_copy_left(gapbuf* point_node)
{
    gapbuf* left = gapbuf_new(16);
    if(point_node -> gap_start <= 7){
        left->gap_start = point_node->gap_start;
        left->gap_end = left->gap_start + 8;
        for(int i=0; i<left->gap_start; i++)
        //@loop_invariant i<= left->gap_start;
        {
            left->buffer[i] = point_node->buffer[i];
        }
        for(int i=0; i < 8 - point_node->gap_end; i++)
        //@loop_invariant i <= 8 - point_node->gap_end;
        {
            left->buffer[left->gap_end + i] = 
                point_node->buffer[point_node->gap_end + i];
        }
    }
    else if (point_node -> gap_start > 7){
        left->gap_start = 8;
        left->gap_end = 16;
        for(int i = 0; i<8; i++)
        //@loop_invariant i<= 8;
        {
            left->buffer[i] = point_node->buffer[i];
        }
    }
    return left;
}

gapbuf* gapbuf_copy_right(gapbuf* point_node)
{
    gapbuf* right = gapbuf_new(16);
    if(point_node -> gap_start > 7){
        right->gap_start = point_node->gap_start-8;
        right->gap_end = right->gap_start + 8;
        for(int i=0; i<point_node->gap_end-8; i++)
        //@loop_invariant i<= point_node->gap_start-8;
        {
            right->buffer[i] = point_node->buffer[8+i];}
        for(int i=0; i < 16 - point_node->gap_end; i++)
        //@loop_invariant i <= 16 - point_node->gap_end;
        {
            right->buffer[right->gap_end + i] = 
                point_node->buffer[point_node->gap_end + i];
        }
    }
    else if (point_node -> gap_start <= 7){
        right->gap_start = 0;
        right->gap_end = 8;
        for(int i = 8; i<16; i++)
        //@loop_invariant i<= 16;
        {
            right->buffer[i] = point_node->buffer[i];
        }
    }
    return right;
}

void tbuf_split_pt(tbuf* B)
{
    dll* left_node = new_node();
    dll* right_node = new_node();
    gapbuf* left_gapbuf =  gapbuf_copy_left(B->point->data);
    gapbuf* right_gapbuf =  gapbuf_copy_right(B->point->data);

    left_node -> data = left_gapbuf;
    right_node -> data = right_gapbuf;
    left_node->next = right_node;
    right_node->prev = left_node;

    dll* tmp = B->point;
    left_node->prev = tmp->prev;
    right_node->next = tmp->next;
    tmp->prev->next = left_node;
    tmp->next->prev = right_node;

    if(gapbuf_at_right(left_gapbuf))
        B->point = right_node;
    if(gapbuf_at_left(right_gapbuf))
        B->point = left_node;
}

void tbuf_forward(tbuf* B)
{  
    if(!dll_pt_at_right(B))
    {
        B->point = B->point->next;
        gapbuf_forward(B->point->data);
    }
    else if(!gapbuf_at_right(B->point->data))
        gapbuf_forward(B->point->data);
}

void tbuf_backward(tbuf* B)
{   
    if (!dll_pt_at_left(B))
    {
        B->point = B->point->prev;
        gapbuf_backward(B->point->data);
    }
    else if(!gapbuf_at_left(B->point->data))
         gapbuf_backward(B->point->data);
}

void tbuf_insert(tbuf* B, char c)
{   
    if (!gapbuf_full(B->point->data))
        gapbuf_insert(B->point->data, c);
    if (gapbuf_full(B->point->data))
        tbuf_split_pt(B);
}

void tbuf_delete(tbuf* B)
{
    if(!gapbuf_empty(B->point->data) && is_gapbuf(B->point->data))
        gapbuf_delete(B->point->data);
    if (gapbuf_empty(B->point->data) &&
                !only_one_empty_gapbuf_check(B)){
        dll* tmp = B->point;
        B->point = tmp->prev;
        B->point->next = tmp->next;
        tmp->next->prev = B->point;}
}
