#pragma once
#include "defs.h"
#include "errorout.h"

//#define MAX_STACK_SIZE 1000
const int MAX_STACK_SIZE = 100;

typedef struct _Stack
{
	Token* top;
	Token* base;
	int size;
}Stack;

void init_stack(Stack* stack);
void free_stack(Stack* stack);
void push(Stack* stack, Token t);
Token* pop(Stack* stack);
Token* top(Stack* stack);
void inverse(Stack* stack);


void init_stack(Stack* stack) {
	//stack->base = (Token*)malloc(MAX_STACK_SIZE * sizeof(Stack));
	//stack->base = new Token[1000];
	stack->base = new Token[1000];
	stack->size = 0;
	stack->top = NULL;
}

void free_stack(Stack* stack) {
	//free(stack->base);
	delete[] stack->base;
}

void push(Stack* stacck, Token t) {
	int size = stacck->size;
	if (size >= MAX_STACK_SIZE)
	{
		error("stack overflow",000);
	}
	stacck->base[size] = t;
	stacck->size++;
	stacck->top = stacck->base + stacck->size - 1;
}

Token* pop(Stack* stack) {
	if (stack->size == 0)
	{
		error("can't pop empty stack",000);
	}

	Token* top_token = top(stack);
	stack->top = stack->top - 1;
	stack->size--;
	return top_token;
}

Token* top(Stack* stack) {
	return stack->top;
}

void inverse(Stack* stack) {
	Stack s2;
	init_stack(&s2);

	for (size_t i = stack->size; i > 0; i--)
	{
		Token* t = pop(stack);
		push(&s2, *t);
	}
	free_stack(stack);
	stack->top = s2.top;
	stack->base = s2.base;
	stack->size = s2.size;
}
