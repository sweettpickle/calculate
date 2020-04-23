#ifndef CALCULATE_HEADER_H
# define CALCULATE_HEADER_H

# define SIZE 128

typedef struct	s_stack
{
    int			stk[SIZE];
    int			top;
}				t_stack;

void			push(int x, t_stack *s);
int				pop(t_stack *s);
int				is_empty(t_stack *s);

#endif //CALCULATE_HEADER_H
