# CS4280 Code Generation (p4)

The final installment of a 4 project approach at an iterative build of a machine language compiler built in C for a fictitious language for which we were provided a CFG. This installment adds the generation of a machine language to be run on a supplied virtual machine. As a part of machine language, temporary storage is also developed to support math operations defined in our CFG.

## Compilation:

Creating comp executable
	
	Run the "make" command from a terminal while in the project root directory 
	containing all of the project source and header files as well as the Makefile. 
	Compilation will create the comp executable.
	
	"make clean" will remove the comp executable.
	"make realclean" will remove the comp executable as well as any files with the .4280E01
	or .asm extension.


## Invocation:
comp [filename]:

	comp				Invokes program and user is prompted for input.
					Keyboard simulated EOF terminates input.
	comp < filename			Invokes program and input is read via stdin file redirection.
					filename must be precise including extension (if any).
	comp filename			Invokes program and input is read directly from file.
					filename should not include implicit extension (.4280E01)
	
	Improper invocation will cause the above usage information to be displayed.

## Output
Expected Output

	Output is in the form of error message on scanner, parser or semantic error, or a message 
	indicating success and the .asm (output) file name. Additionally, memory/pointer errors may 
	also cause error messages to be output.

## Project Detail:
* In this project, code generation was simply added to the semantics checking functions, so no new
.c or .h files were introduced for this submission. 
* Since no new files were produced, there were
some new variables and functions introduced in the semantics files to enable code generation.

* Those new additions are:
	* Variables:
		* counters for temp variables and labels
		* an array for holding temporary variables
	* Functions:
		* temporary variable constructor
		* Label constructor

	* The remainder of the changes were file print statements to produce the .asm file.

* Several files were also REMOVED:
	* treeprint.c/.h and utils.c/.h were removed due to obsolescence.

## Project Testing:

	Project was tested using all of the provided test files as well as using my own bash script 
	that runs against files similar to those tested against in project 3. 
	Output from the provided testfiles was as expected.


* Unanticipated behavior:
	* Nothing major.
	* When a bad semantic check is encountered, the corresponding .asm file is still created and 
	will contain any target language generated before the error was encountered.
	I actually found this VERY helpful for troubleshooting one error that I did not expect, so I left it.
	Incomplete/bad .asm files could easily be programmatically removed by passing the output file name to 
	the semantics/codegen function and making a system call to remove the file on error before the program 
	is terminated. I actually employed a similar "cleanup" function in P0 to remove temporary files.



## Project Specification:

### Storage allocation:
 * All storage is 2-byte signed 
 * Storage needed for  
 	* program variables  
	* temporaries (e.g., if accumulator needs to be saved for later use)  
	* temporaries can be added to to global variables pool or allocated locally if using local scoping.   
	I would suggest global. We can assume not to use variables named T# or V# in the source, reserving   
	such names for temporary variables.  
	* there is no need to optimize reducing the number of temporaries
 * Global option  
 	* storage allocation should follow static semantics and code generation  
	* issue storage directive for every global variable and every temporary, using the global storage   
	directive in the virtual machine, after the STOP instruction
 * Local option  
 	* global variables and temporaries can be generated as in the global option, temporaries could also   
	be local, or all could be local  
	* local variable should be allocated on the virtual machine's system stack during the single pass   
	which performs static semantics and code generation  
	* modify the static semantics by adding code generation in the same pass  
	* code generation discussed separately  
	* storage allocation
		* every push() must be accompanied by PUSH in the target  
		* every pop() must be accompanied by POP in the target  
		* every find() returning n>=0 (when used for data use, this means this is local variable) should be   
		accompanied by 
			* STACKR n if this is reading access  
			* STACKW n if this is writing access


### Code generation: 

* The parse tree is equivalent to the program in left to right traversal (skipping syntactic tokens if not
stored in the tree). Therefore, perform left to right traversal 	
* When visiting a node, generate appropriate code at appropriate time if the node is code-generating  
	* a node with no children and no token probably needs no code generated  
	* a node with only one child and no tokens probably needs no code generated unless it is action node 
	such as negation  
	* a node always generates the same code except for possible different tokens and/or different storage   
	used. Therefore, the code generator can be a set of functions, one function per each node kind, that   
	is one per each parser function. Instead of a set of functions, could use a switch in a single function 
	(in recursive traversal)  	
	* every code-generating node generates code and the same code regardless of its location in the tree  	
	* some nodes need to generate some code preorder, some in-order, some post-order, based on the semantics 
	of the code corresponding to this node
* at the end of the traversal, print STOP to target (to be followed by global variables+temporaries in 
storage allocation)

### Useful assumptions:  
* assume and enforce that every subtree generating some value will leave the result in the accumulator  
* global option: separate traversal after static semantics is recommended  
* local option: a suggested approach is to perform static semantics, code generation, and storage allocation   
on the stack in a single pass - start with static semantics traversal and then modify the code### 

### Variables will require:  
* variable creation upon definition 
* see storage allocation  
* access upon use   
	* examples in class, such as R node or assignment node, were for global option  
	* for local option, the access for local variables needs to be changed to stack access 
	* see storage allocation.   Global variables can be processed as in teh global option.

## Author
* **Tim M. Lael**
