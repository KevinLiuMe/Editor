
#use <string>

typedef struct gapbuf_header gapbuf;
typedef gapbuf* elem; 

struct gapbuf_header {
  int limit;     /* limit > 0                      */
  char[] buffer; /* \length(buffer) == limit       */
  int gap_start; /* 0 <= gap_start                 */
  int gap_end;   /* gap_start  <= gap_end <= limit */
};

bool is_gapbuf(gapbuf* G);

bool gapbuf_empty(gapbuf* G);    /* Returns true if the buffer is empty */
bool gapbuf_full(gapbuf* G);     /* Returns true if the buffer is full  */
bool gapbuf_at_left(gapbuf* G);  /* Returns true if the gap is at the   */
                                 /*   left end of the buffer            */
bool gapbuf_at_right(gapbuf* G); /* Returns true if the gap is at the   */
                                 /*   right end of the buffer           */

gapbuf* gapbuf_new(int limit);   /* Create a new gapbuf of size limit   */
void gapbuf_forward(gapbuf* G);  /* Move the gap forward, to the right  */
void gapbuf_backward(gapbuf* G); /* Move the gap backward, to the left  */
void gapbuf_insert(gapbuf* G, char c); /* Insert char c before the gap  */
void gapbuf_delete(gapbuf* G);   /* Delete the char before the gap      */


bool is_gap_expected_length(char[] A, int limit){
  return true;
}

bool is_gapbuf(gapbuf* G){
  return(G != NULL && is_gap_expected_length(G->buffer, G->limit) &&
          G->limit > 0 && 0 <= G->gap_start && 
          G->gap_start <= G->gap_end &&
          G->gap_end <= G->limit);
}

bool gapbuf_empty(gapbuf* G)
{
  return G->gap_start == 0 && G->gap_end == G->limit;
}

bool gapbuf_full(gapbuf* G) 
{
  return G->gap_start == G->gap_end;
}

bool gapbuf_at_left(gapbuf* G) 
{
  return G->gap_start == 0;
}

bool gapbuf_at_right(gapbuf* G) 
{
  return G->gap_end == G->limit;
}

gapbuf* gapbuf_new(int limit)
{
  struct gapbuf_header* new_gapbuf = alloc(struct gapbuf_header);
  new_gapbuf->limit = limit;
  new_gapbuf->buffer = alloc_array(char, limit);
  new_gapbuf->gap_start = 0;
  new_gapbuf->gap_end = limit;
  return new_gapbuf;
}

void gapbuf_forward(gapbuf* G)
{
  int start = G->gap_start;
  int end = G->gap_end;
  G->buffer[start] = G->buffer[end];
  G->gap_start = G->gap_start + 1;
  G->gap_end = G->gap_end + 1;
}

void gapbuf_backward(gapbuf* G) 
{
  G->gap_start = G->gap_start - 1;
  G->gap_end = G->gap_end - 1;
  int start = G->gap_start;
  int end = G->gap_end;
  G->buffer[end] = G->buffer[start];
}

void gapbuf_insert(gapbuf* G, char c)
{
  int idx = G->gap_start;
  G->buffer[idx] = c;
  G->gap_start = G->gap_start + 1;
}

void gapbuf_delete(gapbuf* G)
{
  G->gap_start = G->gap_start - 1;
}

