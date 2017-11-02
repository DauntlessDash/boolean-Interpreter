    //////////////////////////////////////////////////////////////////////////////////////////////
   //                    CS 3361 Interpreter Programming Project                                 
  //                            By: Cody Otterbine												
 //          THE METHOD FOR INPUT IS VIA A STRING INPUTTED VIA CONSOLE						   
//////////////////////////////////////////////////////////////////////////////////////////////


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stack>

bool result;

class BooleanEval {
private:

	bool opone; //Operator 1 for doing boolean operations
	bool optwo; //Operator 2 for doing boolean operations
	std::stack <bool> stack; //Stack for holding boolean operators and result
	
//Parse <A>: Atom
	bool A(char*& expr) {

		//Skip spaces
		while (*expr == ' ')
			expr++;

		if (*expr == 'T'){ 
			expr++; //Remove T from expr
			stack.push(true); //BOOLEAN TRUE ADDED TO STACK
			return true;
		}
		else if (*expr == 'F'){ 
			expr++; //Remove F from expr
			stack.push(false); //BOOLEAN FALSE ADDED TO STACK
			return true;
		}
		else if (*expr == '('){
			expr++; //Remove left parenthesis from expr
			if (Implyt(expr)){ //(<IT>) Imply Term
				expr++; //Remove right parenthesis from expr
				return true;
			}
			else{
				return false;
			}
		}
		else{
			printf("A ERROR: expecting a T, F, or (\n");
			return false;
		}
	}

//Parse <L>: Literal
	bool L(char*& expr) {

		//Skip spaces
		while (*expr == ' ')
			expr++;
		if (*expr == 'T' || *expr == 'F' || *expr == '('){ //Selection set for <A>
			if (A(expr)){
				return true;
			}
			else{
				return false;
			}

		}
		else if (*expr == '~'){ 
			expr++; //Remove ~ from expr
			while (*expr == ' ') //Skip spaces
				expr++;
			if (L(expr)){ //Right precedence operation
				opone = stack.top(); //Removes top element from stack and replaces it with NOT of that value
				stack.pop();
				stack.push(!opone); //BOOLEAN NOT OPERATION
				return true;
			}
			else{
				return false;
			}
		}
		else{
			printf("L ERROR: expecting a T, F, or (\n");
			return false;
		}
	}

//Parse <AT_tail>: And Tail
	bool AtTail(char*& expr) {

		//Skip spaces
		while (*expr == ' ')
			expr++;

		if (*expr == '^'){ 
			expr++; //Removes ^ from expr
			if (L(expr)){
				optwo = stack.top(); //Removes the previously parsed operations from stack to be evaluated
				stack.pop();
				opone = stack.top();
				stack.pop();
				stack.push(opone && optwo); //BOOLEAN AND OPERATION
				//Debug Message: printf("%s", opone ? "T" : "F");
				//Debug Message: printf("^%s\n", optwo ? "T" : "F");

				//skip spaces
				while (*expr == ' ')
					expr++;

				if (AtTail(expr)){ 
					return true;
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
		}
		else if (*expr == 'v' || *expr == '-' || *expr == '.' || *expr == ')'){ //End case
			return true;
		}
		else{
			printf("AtTail ERROR: expecting a ^, v, ->, ., or )\n");
			return false;
		}
	}

//Parse <AT>: And Term
	bool AndT(char*& expr) {

		//Skip spaces
		while (*expr == ' ')
			expr++;

		if (*expr == '~' || *expr == 'T' || *expr == 'F' || *expr == '('){ //Selection set for <L><AT_Tail>
			if (L(expr)){
				if (AtTail(expr)){
					return true;
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}

		}
		else{
			printf("AndT ERROR: expecting a ~, T, F, or (\n");
			return false;
		}
	}

//Parse <OT_tail>: Or Tail
	bool OtTail(char*& expr) {

		//Skip spaces
		while (*expr == ' ')
			expr++;

		if (*expr == 'v'){ //Selection set for V<AT><OT_Tail>
			expr++;
			if (AndT(expr)){ 
				optwo = stack.top(); //Removes the previously parsed operations from stack to be evaluated
				stack.pop();
				opone = stack.top(); 
				stack.pop(); 
				stack.push(opone || optwo); //BOOLEAN OR OPERATION
				//Debug Message: printf("%s", opone ? "T" : "F");
				//Debug Message: printf("v%s\n", optwo ? "T" : "F");
				
				//skip spaces
				while (*expr == ' ')
					expr++;

				if (OtTail(expr)){ 
					return true;
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
		}
		else if (*expr == '-' || *expr == '.' || *expr == ')'){ //End case
			return true;
		}
		else{
			printf("OtTail ERROR: expecting a v,->, ., or )\n");
			return false;
		}
	}

//Parse <OT>: Or Term
	bool OrT(char*& expr) {

		//Skip spaces
		while (*expr == ' ')
			expr++;

		if (*expr == '~' || *expr == 'T' || *expr == 'F' || *expr == '('){ //Selection set for <AT><OT_Tail>
			if (AndT(expr)){
				if (OtTail(expr)){
					return true;
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}

		}
		else{
			printf("OrT ERROR: expecting a ~, T, F, or (\n");
			return false;
		}
	}

//Parse <IT_tail>: Imply Tail
	bool ImplyTail(char*& expr) {

		//Skip spaces
		while (*expr == ' ')
			expr++;
		if (*expr == '-'){
			expr++;
			if (*expr != '>'){
				printf("ImplyTail ERROR: expecting a >\n");
				return false;
			}
			expr++;
			//Skip spaces
			while (*expr == ' ')
				expr++;
			if (OrT(expr)){
				optwo = stack.top();
				stack.pop();
				opone = stack.top();
				stack.pop();
				stack.push((!opone) || optwo); //BOOLEAN IMPLY OPERATION
				//Debug Message: printf("%s", opone ? "T" : "F");
				//Debug Message: printf("->%s\n", optwo ? "T" : "F");
				
				if (ImplyTail(expr)){
					return true;
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
		}
		else if (*expr == '.' || *expr == ')'){ //End case
			return true;
		}
		else{
			printf("ImplyTail ERROR: expecting a ->, ., or )\n");
			return false;
		}
	}

//Parse <IT>: Imply Term 
	bool Implyt(char*& expr) {

		//Skip spaces
		while (*expr == ' ')
			expr++;
		if (*expr == '~' || *expr == 'T' || *expr == 'F' || *expr == '('){ //Selection set for <OT><IT_Tail>
			if (OrT(expr)){
				if (ImplyTail(expr)){
					return true;
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
			
		}
		else{
			printf("Implyt ERROR: expecting a ~, T, F, or (\n");
			return false;
		}
	}

// Parse <B>: Bool_stmt
	bool BoolSt(char*& expr) {

		//Skip spaces
		while (*expr == ' ')
			expr++;
		if (*expr == '~' || *expr == 'T' || *expr == 'F' || *expr == '('){ //Selection set for <IT>.
			if (Implyt(expr)){
				return true;
			}
			else{
				return false;
			}
		}
		else{
			printf("BoolSt ERROR: expecting a ~, T, F, or (\n");
			return false;
		}
	}

public: //Public Driver Function that Evaluates Boolean Expression
	bool Eval(char* expr) {
		
		bool res = BoolSt(expr); //Evaluates Boolean Expression
		if (res != false){
			result = stack.top(); //Gets result from stack
		}
		return res;
	};
	
};

int main() {

	puts("Insert Boolean Expression (Or input nothing to exit):");
	for(;;) { 

		static char buffer[150]; //Buffer for string input
		char *expr = gets_s(buffer, sizeof(buffer)); //Getting user input

		//Exit on empty string
		if(*expr == '\0')
			return 0;

		//Evaluate the expression
		BooleanEval eval;
		bool res = eval.Eval(expr);
		if (res == false){
			puts("Syntax Error: Syntax Not Valid");
		}
		else{
			puts("Syntax Check: Syntax Valid");
			if (result == true){
				puts("Result: True");
			}
			else if (result == false){
				puts("Result: False");
			}
		}
	}
}
