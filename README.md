# Boolean-Interpreter/Parser (C++)

## Synopsis

As apart of learning more about how to make programming languages I created an LL(1) grammar predictive parser for Boolean expressions that checks syntax using recursive descent parsing and
evaluates the expression. Implementation required generating first, follow, and selection sets for predictive parsing.


## Predictive Parsing

Predictive parsing is when a recursive descent interpreter does not require backtracking. Predictive parsing is possible only for LL(k) grammars, these are context-free grammars for which there exists some positive integer k that allows the interpreter to decide which production to use by examining only the next k tokens of input. This particular predictive parser is utilized for only Boolean functions rather than an entire programming language, but the method of parsing is still the same as any major programming language.


## Input

This program prompts the user to input a Boolean string that ends in a "." and is made up of the following symbols:

```
T		: TRUE
F		: FALSE
^		: AND
v		: OR
~		: NOT
->		: IMPLICATION
```

Expressions may contain white spaces and white spaces are considered to be delimiters(i.e. a white space between the - and > of the implication symbol would be a syntax error). This program checks if the expression is of valid syntax and (if valid) computes the value of the expression. The output will either be an error message or a message that gives the value of the expression.

Although the operations results are simple, it is the method of syntax checking that was the motivation for this project. Below is the syntax used to create this interpreter. 

## Syntax 

This syntax and the corresponding selection sets allow predictive parsing to be possible (boy that a lot of 'Ps'),

```
<B> 		::= <IT>. 							{~,T,F,(}
<IT> 		::= <OT> <IT_Tail>	 				{~,T,F,(}
<IT_Tail> 	::= -> <OT><IT_Tail>				{->}
			::= NULL							{.,)}
<OT> 		::= <AT> <OT_Tail>					{~,T,F,(}
<OT_Tail> 	::= v <AT> <OT_Tail>				{v}
			::= NULL							{->,.,)}
<AT> 		::= <L> <AT_Tail> 					{~,T,F,(}
<AT_Tail>	::= ^ <L> <AT_Tail> 				{^}
			::= NULL							{V,->,.,)}
<L> 		::= <A>								{T,F,(}
			::= ~<L>							{~}
<A>			::= T								{T}
			::= F								{F}
			::=(<IT>)							{(}
```

## Syntactic Domains

```
<B> : Bool_stmt
<IT> : Imply_term
<OT> : Or_term
<AT> : And_term
<IT Tail> : Imply_tail
<OT Tail> : Or_tail
<AT Tail> : And_tail
<L> : Literal
<A> : Atom
```

## Semantic Equations

![Semantic Equations](https://github.com/DauntlessDash/boolean-Interpreter/blob/master/Semantic_Equations.png)



