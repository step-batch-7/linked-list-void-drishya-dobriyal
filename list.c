#include "linkedlist.h"

Status clear_list( List_ptr list){
  Node_ptr p_walk = list->first;
  Node_ptr element_ptr = NULL;
  while (p_walk != NULL)
  {
    element_ptr = p_walk;
    p_walk = p_walk->next;
    free(element_ptr);
  }
  list->first = NULL;
  list->last = NULL;
  list->length = 0;
  return Success;
}

void destroy_list(List_ptr list){
  Node_ptr p_walk = list->first;
  Node_ptr eliminate_ptr = NULL;
  while (p_walk != NULL)
  {
    eliminate_ptr = p_walk;
    p_walk = p_walk->next;
    free(eliminate_ptr);
  }
  free(list);
}

void display_list(List_ptr list){
  Node_ptr p_walk = list->first;
  while (p_walk != NULL)
  {
    printf("%d\n",*(int *)p_walk->element);
    p_walk = p_walk->next;
  }
}

Node_ptr create_node(Element element){
  Node_ptr new_node = malloc(sizeof(Node));
  new_node->element = element;
  new_node->next = NULL;
  return new_node;
}

List_ptr create_list(void){
  List_ptr list = malloc(sizeof(LinkedList));
  list->first = NULL;
  list->last = NULL;
  list->length = 0;
  return list;
}

Prev_Curr_Pair get_pair(List_ptr list, int pos){
  Prev_Curr_Pair node_ptrs;
  node_ptrs.prev = NULL;
  node_ptrs.curr = list->first;
  while(pos > 0){
    node_ptrs.prev = node_ptrs.curr;
    node_ptrs.curr = node_ptrs.curr->next;
    pos--;
  } 
  return node_ptrs; 
}

Status add_to_start( List_ptr list, Element value) {
  return insert_at(list, value, 0);
}

Status add_to_list( List_ptr list, Element value) {
  return insert_at(list, value, list->length);
}

Status insert_at(List_ptr list, Element element, int position){
  if( position > list->length || position < 0) return Failure;
  Node_ptr new_node = create_node(element);
  Prev_Curr_Pair node_ptrs = get_pair(list, position);
  Node_ptr *ptr_to_set = &list->first;
  if( node_ptrs.prev != NULL) ptr_to_set = &node_ptrs.prev->next;
  *ptr_to_set = new_node;
  new_node->next = node_ptrs.curr;
  if(node_ptrs.curr == NULL ) list->last = new_node;
  list->length++;
  return Success;
}

Status is_equal( Element value1, Element value2){
  return *( int *)value1 == *(int *)value2;
}

Status is_value_present( List_ptr list, Element value, Matcher matcher){
   Node_ptr p_walk = list->first;
  while (p_walk != NULL)
  {
    if((*is_equal)( p_walk->element, value)) return Success;
    p_walk = p_walk->next;
  }
  return Failure;
}

Status add_unique(List_ptr list, Element value, Matcher matcher){
  if(is_value_present(list, value, matcher)) return Failure;
  return insert_at(list,value,list->length);
}

Element remove_from_start(List_ptr list) {
  return remove_at(list, 0);
}

Element remove_from_end(List_ptr list){
  return remove_at(list, list->length - 1);
};

Element remove_at(List_ptr list, int position){
  if(position >= list->length || position < 0) return NULL;
 Prev_Curr_Pair node_ptrs = get_pair(list, position);
  Node_ptr node_to_remove = node_ptrs.curr;
  Node_ptr *ptr_to_set = &list->first;
  if (node_ptrs.prev != NULL) ptr_to_set = &node_ptrs.prev->next;
  *ptr_to_set = node_ptrs.curr->next;
  if(node_ptrs.curr->next==NULL) list->last = node_ptrs.prev;
  Element removed_element = node_to_remove->element;
  free(node_to_remove);
  list->length--;
  return removed_element;
}

Element remove_first_occurrence(List_ptr list, Element value, Matcher  matcher){
  Node_ptr p_walk = list->first;
  int pos = 0;
  while (p_walk != NULL)
  {
    if((*matcher)(p_walk->element, value)) return remove_at(list, pos);
    p_walk = p_walk->next;
    pos++;
  }
  return NULL;
}

List_ptr remove_all_occurrences(List_ptr list, Element value, Matcher matcher) {
	List_ptr new_list = create_list();
	int pos = 0;
	while (1)
	{
	  Element new_element= remove_first_occurrence(list, value, matcher);
	  if(new_element == NULL) break;
	  insert_at(new_list, new_element, pos);
	  pos += 1;
	}
	return new_list;
}	

Element increment( Element value ){
  int *number = malloc(sizeof(Element));
  *number = *(int *)value + 1;
  return number;
}

List_ptr map( List_ptr list, Mapper mapper){
  Node_ptr p_walk = list->first;
  List_ptr new_list = create_list();
  int pos = 0;
  while ( p_walk != NULL)
  {
    Element value = (*mapper)(p_walk->element);
    insert_at( new_list, value, pos);
    p_walk = p_walk->next;
    pos++;
  }
  return new_list;
}

Status is_even( void *value) {
  return *(int *)value % 2 == 0;
}

List_ptr filter( List_ptr list, Predicate predicate){
  Node_ptr p_walk = list->first;
  List_ptr  new_list = create_list();
  while ( p_walk != NULL)
  {
    if((*predicate)(p_walk->element)) add_to_list( new_list, p_walk->element);
    p_walk = p_walk->next;
  }
  return new_list;
}

Element sum( Element num1, Element num2){
  int *sum = malloc(sizeof(int));
  *sum = *(int *)num1 + *(int *)num2;
  return sum;
}

Element reduce( List_ptr list,Element context, Reducer reducer ){
  Node_ptr p_walk = list->first;
  while ( p_walk != NULL)
  {
    context = (*reducer)( context, p_walk->element);
    p_walk = p_walk->next;
  }
  return context;
}

void increment_by_one( Element value){
  *(int *)value += 1;
}

void forEach(List_ptr list, ElementProcessor processor) {
 Node_ptr p_walk = list->first;
  while ( p_walk != NULL)
  {
    (*processor)( p_walk->element);
    p_walk = p_walk->next;
  }
}

List_ptr reverse( List_ptr list){
  List_ptr new_list = create_list();
  Node_ptr p_walk = list->first;
  while (p_walk != NULL)
  {
    add_to_start(new_list, p_walk->element);
    p_walk = p_walk->next;
  }
  return new_list;
}