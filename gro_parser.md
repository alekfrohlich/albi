# gro parser

## Project goal: 

Develop a parser for the gro programming language with focus on generating
equivalent smol\sbml (or other) markup representations.

## Methodology:

We're definitely going with a top-down approach here. There isn't enough time to read the entirety of the dragon book in parallel with flex & bison. In conclusion, low-level stuff is going to be an aside plate, not main course. 

## Learning Schedule

### weeks 5-8
- Learn gro syntax.
- see basic gro examples in practice (debugging simulator work).
- Begin to understand the way in which the gro simulator parses source code.
- Begin toying with flex and bison.

### weeks 9-12
- Intensive code review of the gro simulator.
- Define compiler software architecture.

### weeks 13-16
- Discuss about representability of the gro language on the smol\sbml formats.

## Release Schedule

### weeks 5-8
- Start introduction, refs and correlated work sections
- Examples of handcrafted gro -> markup syntax translations.
- Discuss article objectives.

### weeks 9-12
- Identify parsing methods based on gro simulator.
- ... to be defined.

### weeks 13-16
- Write conclusion section.
- Finish working kernel of the gro parser.

## Theory Study Schedule

### Compiler principles, techniques and tools - main topics
			- Intro : 2.1-2.5
			- Lexical analysis : 2.6, 3.1-3.4
			- Symbol tables 2.7, 7.6
			- Parsing 2.4, 4.1-4.4
			- Syntax-directed translation : 2.5, 5-1-5.5
			- Lexical analisis generators : 3.5
			- Parse generators : 4.8, 4.9
			- Code generators : 9.12
			- Techniques for compiler constructions : Chapter 11

### Week 1 and 2 - Introduction to compilers
- Introduction : 2.1 - 2.5

### Week 3 and 4 - Lexical analysis, and lexical analisis generator
- Lexical analysis : 2.6, 3.1-3.5
- Lexical analysis generator : 3.5

### Week 5 - Parsing and parse generators
- Parsing : 2.4, 4.1-4.4
- Parse generators : 4.8, 4.9

### Week 6 and 7 - Techniques for compiler construction and code generators
- Techniques for compiler construction : 11.1, 11.2, 11.4, A.3, A.4
- Code generators : 9.12

### Week 8 and 9 - Syntax-directed translation - Part I
- Syntax-directed translation : 2.5, 5.1-5.5

### Week 10 and 11 - Syntax-directed translation - Part II
- Syntax-directed translation : 5.6-5.10

### Week FREE - Symbol table
- Symbol table : 2.7, 7.6
## Learning refs:c

- [Awesome Compilers](https://github.com/aalhour/awesome-compilers)
- [The gro programming Language](https://depts.washington.edu/soslab/gro/)
- [The dragon book](https://www.amazon.com/Compilers-Principles-Techniques-Alfred-Aho/dp/0201100886/ref=sr_1_2?keywords=Compilers%3A+Principles%2C+Techniques%2C+and+Tool&qid=1567550137&s=books&sr=1-2)
- [C to HDL](https://en.wikipedia.org/wiki/C_to_HDL)
- [Flex & Bison, O'Reilly](http://web.iitd.ac.in/~sumeet/flex__bison.pdf)
- [Bison 3.4](https://www.gnu.org/software/bison/manual/bison.html)

## Scientific studies refs:

- INSERT ARTICLES HERE
